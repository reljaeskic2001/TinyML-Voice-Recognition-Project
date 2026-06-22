import numpy as np
import os
import tensorflow as tf

# --------------------------------
# SETTINGS
# --------------------------------

DATASET_PATH = "dataset"
CLASSES = ["yes", "no", "noice"]

X = []
y = []

print("Loading dataset...")

# --------------------------------
# LOAD DATA
# --------------------------------

for label_index, label in enumerate(CLASSES):

    folder = os.path.join(DATASET_PATH, label)

    for file in os.listdir(folder):

        if not file.endswith(".txt"):
            continue

        path = os.path.join(folder, file)

        audio = np.loadtxt(path)

        # ensure 16000 samples
        if len(audio) < 16000:
            audio = np.pad(audio, (0, 16000 - len(audio)))
        else:
            audio = audio[:16000]

        # match Arduino feature extraction
        features = audio[::80][:200]

        # normalize
        features = features / np.max(np.abs(features))

        X.append(features)
        y.append(label_index)

X = np.array(X)
y = np.array(y)

print("Dataset shape:", X.shape)

# --------------------------------
# SHUFFLE DATASET
# --------------------------------

indices = np.arange(len(X))
np.random.shuffle(indices)

X = X[indices]
y = y[indices]

# --------------------------------
# MODEL
# --------------------------------

model = tf.keras.Sequential([
    tf.keras.layers.Input(shape=(200,)),

    tf.keras.layers.Dense(64, activation="relu"),
    tf.keras.layers.Dense(64, activation="relu"),
    tf.keras.layers.Dense(32, activation="relu"),

    tf.keras.layers.Dense(3, activation="softmax")
])

model.compile(
    optimizer="adam",
    loss="sparse_categorical_crossentropy",
    metrics=["accuracy"]
)

# --------------------------------
# TRAIN
# --------------------------------

print("\nTraining model...\n")

model.fit(
    X,
    y,
    epochs=400,
    validation_split=0.2,
    batch_size=8
)

loss, acc = model.evaluate(X, y)

print("\nTraining accuracy:", acc)

# --------------------------------
# CONVERT TO TFLITE
# --------------------------------

print("\nConverting to TensorFlow Lite...")

converter = tf.lite.TFLiteConverter.from_keras_model(model)
tflite_model = converter.convert()

with open("voice_model.tflite", "wb") as f:
    f.write(tflite_model)

print("Saved voice_model.tflite")