#include <PDM.h>

#include "TensorFlowLite.h"
#include "model.cc"

#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/all_ops_resolver.h"

static const char channels = 1;
static const int frequency = 16000;

short sampleBuffer[256];
volatile int samplesRead;

#define RECORD_SIZE 16000

int16_t fullBuffer[RECORD_SIZE];
int recorded = 0;

/* TensorFlow */

tflite::MicroErrorReporter micro_error_reporter;
tflite::ErrorReporter* error_reporter = &micro_error_reporter;

const tflite::Model* model;
tflite::AllOpsResolver resolver;

constexpr int tensorArenaSize = 10 * 1024;
uint8_t tensorArena[tensorArenaSize];

tflite::MicroInterpreter* interpreter;

TfLiteTensor* input;
TfLiteTensor* output;

/* command cooldown */

unsigned long lastCommandTime = 0;

/* microphone callback */

void onPDMdata() {

  int bytesAvailable = PDM.available();
  PDM.read(sampleBuffer, bytesAvailable);

  samplesRead = bytesAvailable / 2;
}

void setup() {

  Serial.begin(115200);
  while (!Serial);

  Serial.println("Voice command inference starting...");

  /* Load model */

  model = tflite::GetModel(voice_model_tflite);

  interpreter = new tflite::MicroInterpreter(
    model,
    resolver,
    tensorArena,
    tensorArenaSize,
    error_reporter
  );

  interpreter->AllocateTensors();

  input = interpreter->input(0);
  output = interpreter->output(0);

  /* Start microphone */

  PDM.onReceive(onPDMdata);

  if (!PDM.begin(channels, frequency)) {
    Serial.println("Failed to start PDM!");
    while (1);
  }

  Serial.println("Microphone ready");
}

void loop() {

  if (samplesRead) {

    for (int i = 0; i < samplesRead; i++) {

      if (recorded < RECORD_SIZE) {
        fullBuffer[recorded] = sampleBuffer[i];
        recorded++;
      }

    }

    samplesRead = 0;
  }

  /* once 1 second of audio collected */

  if (recorded >= RECORD_SIZE) {

    /* silence filter */

    float energy = 0;

    for (int i = 0; i < RECORD_SIZE; i++) {
      energy += abs(fullBuffer[i]);
    }

    energy /= RECORD_SIZE;

    if (energy < 600) {
      recorded = 0;
      return;
    }

    /* feature extraction (200 energy windows) */

    for (int i = 0; i < 200; i++) {

      int start = i * 80;
      int end = start + 80;

      float window_energy = 0;

      for (int j = start; j < end; j++) {
        window_energy += abs(fullBuffer[j]);
      }

      window_energy /= 80.0;

      input->data.f[i] = window_energy / 32768.0;
    }

    /* run inference */

    if (interpreter->Invoke() != kTfLiteOk) {
      Serial.println("Inference failed");
      recorded = 0;
      return;
    }

    float yes_score = output->data.f[1];
    float no_score = output->data.f[0];
    float noise_score = output->data.f[2];

    /* print debug scores */

    Serial.print("YES: ");
    Serial.print(yes_score);

    Serial.print(" | NO: ");
    Serial.print(no_score);

    Serial.print(" | NOISE: ");
    Serial.println(noise_score);

    /* command detection */

    if (millis() - lastCommandTime > 2000) {

      if (yes_score > 0.45 && yes_score > noise_score) {
        Serial.println("COMMAND: YES");
        lastCommandTime = millis();
      }

      if (no_score > 0.70 && no_score > noise_score) {
        Serial.println("COMMAND: NO");
        lastCommandTime = millis();
      }

    }

    recorded = 0;
  }
}
