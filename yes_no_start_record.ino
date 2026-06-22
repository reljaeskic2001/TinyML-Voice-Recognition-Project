#include <PDM.h>

static const char channels = 1;
static const int frequency = 16000;

short sampleBuffer[256];
volatile int samplesRead;

#define RECORD_SIZE 16000

int16_t fullBuffer[RECORD_SIZE];
int recorded = 0;

bool recording = false;

void onPDMdata() {

  int bytesAvailable = PDM.available();
  PDM.read(sampleBuffer, bytesAvailable);

  samplesRead = bytesAvailable / 2;
}

void setup() {

  Serial.begin(115200);
  while (!Serial);

  Serial.println("READY_TO_RECORD");

  PDM.onReceive(onPDMdata);

  if (!PDM.begin(channels, frequency)) {
    Serial.println("MIC_FAILED");
    while (1);
  }
}

void loop() {

  if (!recording && Serial.available()) {

    char cmd = Serial.read();

    if (cmd == 'r') {

      Serial.println("READY");

      delay(2000);

      Serial.println("SPEAK_NOW");

      recording = true;
      recorded = 0;
    }
  }

  if (!recording) return;

  if (samplesRead) {

    for (int i = 0; i < samplesRead; i++) {

      if (recorded < RECORD_SIZE) {
        fullBuffer[recorded] = sampleBuffer[i];
        recorded++;
      }

    }

    samplesRead = 0;
  }

  if (recorded >= RECORD_SIZE) {

    Serial.println("DATA_BEGIN");

    for (int i = 0; i < RECORD_SIZE; i++) {
      Serial.println(fullBuffer[i]);
    }

    Serial.println("DATA_END");

    recording = false;
    recorded = 0;
  }
}