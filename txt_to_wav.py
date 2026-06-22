import numpy as np
import wave
import os

SAMPLE_RATE = 16000

dataset_path = "dataset"

for label in os.listdir(dataset_path):

    label_path = os.path.join(dataset_path, label)

    if not os.path.isdir(label_path):
        continue

    for file in os.listdir(label_path):

        if file.endswith(".txt"):

            txt_path = os.path.join(label_path, file)

            data = np.loadtxt(txt_path, dtype=np.int16)

            wav_path = txt_path.replace(".txt", ".wav")

            with wave.open(wav_path, 'w') as wf:
                wf.setnchannels(1)
                wf.setsampwidth(2)
                wf.setframerate(SAMPLE_RATE)
                wf.writeframes(data.tobytes())

            print("Created", wav_path)