input_file = "voice_model.tflite"
output_file = "model.cc"

with open(input_file, "rb") as f:
    data = f.read()

with open(output_file, "w") as f:

    f.write("const unsigned char voice_model_tflite[] = {\n")

    for i, byte in enumerate(data):

        if i % 12 == 0:
            f.write("  ")

        f.write(f"0x{byte:02x},")

        if i % 12 == 11:
            f.write("\n")

    f.write("\n};\n")

    f.write(f"const unsigned int voice_model_tflite_len = {len(data)};\n")

print("model.cc created successfully")