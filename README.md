# TinyML Voice Command Recognition System

## Overview

This project implements a real-time voice command recognition system on an Arduino Nano 33 BLE Sense using TensorFlow Lite Micro.

The model recognizes spoken commands such as:

- Yes
- No
- Noise

without requiring cloud connectivity.

## Features

- Custom audio dataset collection
- Audio preprocessing and feature extraction
- Neural network training using TensorFlow
- TensorFlow Lite model conversion
- Real-time inference on embedded hardware
- Edge AI deployment

## Technologies

- Python
- TensorFlow Lite Micro
- Arduino Nano 33 BLE Sense
- NumPy
- Machine Learning
- Embedded Systems

## Results

The deployed model successfully classifies spoken commands in real time while operating within the memory constraints of the microcontroller.

## Future Work

- Additional commands
- BLE integration
- Raspberry Pi dashboard
- Continuous learning pipeline
