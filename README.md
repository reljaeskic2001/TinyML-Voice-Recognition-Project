# TinyML Voice Command Recognition System

A TinyML-based speech recognition system that classifies spoken voice commands ("yes" and "no") on an Arduino Nano 33 BLE Sense using TensorFlow Lite Micro.

## Overview

This project demonstrates the complete machine learning workflow for deploying a neural network on embedded hardware. A custom dataset of voice recordings was collected and processed to train a lightweight classification model capable of recognizing spoken commands in real time.

The trained model was converted to TensorFlow Lite format and deployed directly onto an Arduino Nano 33 BLE Sense, enabling on-device inference without requiring cloud connectivity.

## Features

* Custom voice dataset collection
* Audio preprocessing and feature extraction
* Neural network training using TensorFlow
* TensorFlow Lite model conversion
* Embedded deployment on Arduino Nano 33 BLE Sense
* Real-time voice command classification
* Edge AI inference without internet connectivity

## Technologies Used

* Python
* TensorFlow
* TensorFlow Lite Micro
* Arduino Nano 33 BLE Sense
* NumPy
* Machine Learning
* TinyML
* Embedded Systems

## Repository Structure

```text
arduino/
├── model.cc
└── Arduino deployment files

dataset/
├── yes/
├── no/
└── noise/

save_audio.py
train.py
train_model.ipynb
txt_to_wav.py
tflite_to_cc.py
voice_model.tflite
README.md
```

## Machine Learning Pipeline

1. Collect and label voice recordings for each command.
2. Preprocess audio samples and extract features.
3. Train a neural network using TensorFlow.
4. Convert the trained model to TensorFlow Lite format.
5. Convert the model into a C array for embedded deployment.
6. Deploy the model to the Arduino Nano 33 BLE Sense.
7. Perform real-time inference on incoming microphone data.

## Results

The final model successfully recognizes spoken voice commands directly on the microcontroller while operating within the memory and processing constraints of embedded hardware.

This project demonstrates experience with:

* Machine Learning
* Data Collection and Preprocessing
* Model Training and Evaluation
* TensorFlow Lite Deployment
* Embedded AI Systems
* End-to-End Software Development

## Future Improvements

* Support for additional voice commands
* Improved dataset diversity
* Bluetooth Low Energy (BLE) integration
* Raspberry Pi dashboard integration
* Enhanced model accuracy and robustness

## Author

Relja Eskic

M.S. Computer Science

University of Denver
