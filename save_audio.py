import serial
import os

PORT = "COM3"   # change if needed
BAUD = 115200

label = "yes"

save_dir = f"dataset/{label}"
os.makedirs(save_dir, exist_ok=True)

ser = serial.Serial(PORT, BAUD)

recording = False
data = []
count = 0

print("Press ENTER to record")

while True:

    input()  # wait for ENTER key
    ser.write(b"r")
    print("\nRecording triggered")

    while True:

        line = ser.readline().decode().strip()

        if line == "READY":
            print("Get ready...")
            continue

        if line == "SPEAK_NOW":
            print(f"SAY '{label.upper()}' NOW!")
            continue

        if line == "DATA_BEGIN":
            recording = True
            data = []
            continue

        if line == "DATA_END":

            recording = False

            filename = f"{save_dir}/{label}_{count}.txt"

            with open(filename, "w") as f:
                for value in data:
                    f.write(value + "\n")

            print("Saved", filename)

            count += 1
            break

        if recording:
            data.append(line)