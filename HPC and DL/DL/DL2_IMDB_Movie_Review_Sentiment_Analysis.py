import numpy as np
import matplotlib.pyplot as plt

# Load IMDB Dataset
from tensorflow.keras.datasets import imdb

(train_data, train_labels), (test_data, test_labels) = imdb.load_data(num_words=10000)

print("Training Samples:", len(train_data))
print("Testing Samples:", len(test_data))

# Decode Review Example
word_index = imdb.get_word_index()

reverse_word_index = dict(
    [(value, key) for (key, value) in word_index.items()]
)

decoded_review = ' '.join(
    [reverse_word_index.get(i - 3, '?') for i in train_data[0]]
)

print(decoded_review)

# Convert Reviews into Vectors
def vectorize_sequences(sequences, dimension=10000):

    results = np.zeros((len(sequences), dimension))

    for i, sequence in enumerate(sequences):
        results[i, sequence] = 1

    return results

x_train = vectorize_sequences(train_data)
x_test = vectorize_sequences(test_data)

# Convert Labels
y_train = np.asarray(train_labels).astype('float32')
y_test = np.asarray(test_labels).astype('float32')

# Build Model
from tensorflow.keras import models
from tensorflow.keras import layers

model = models.Sequential()

model.add(layers.Dense(
    16,
    activation='relu',
    input_shape=(10000,)
))

model.add(layers.Dense(
    16,
    activation='relu'
))

model.add(layers.Dense(
    1,
    activation='sigmoid'
))

# Compile Model
from tensorflow.keras import optimizers
from tensorflow.keras import losses
from tensorflow.keras import metrics

model.compile(
    optimizer=optimizers.RMSprop(learning_rate=0.001),
    loss=losses.binary_crossentropy,
    metrics=['accuracy']
)

# Validation Data
x_val = x_train[:10000]
partial_x_train = x_train[10000:]

y_val = y_train[:10000]
partial_y_train = y_train[10000:]

# Train Model
history = model.fit(
    partial_x_train,
    partial_y_train,
    epochs=20,
    batch_size=512,
    validation_data=(x_val, y_val)
)

# Training History
history_dict = history.history

# Plot Loss Graph
loss_values = history_dict['loss']
val_loss_values = history_dict['val_loss']

epochs = range(1, len(loss_values) + 1)

plt.plot(
    epochs,
    loss_values,
    'bo',
    label='Training Loss'
)

plt.plot(
    epochs,
    val_loss_values,
    'b',
    label='Validation Loss'
)

plt.title('Training and Validation Loss')

plt.xlabel('Epochs')
plt.ylabel('Loss')

plt.legend()

plt.show()

# Plot Accuracy Graph
acc_values = history_dict['accuracy']
val_acc_values = history_dict['val_accuracy']

plt.plot(
    epochs,
    acc_values,
    'ro',
    label='Training Accuracy'
)

plt.plot(
    epochs,
    val_acc_values,
    'r',
    label='Validation Accuracy'
)

plt.title('Training and Validation Accuracy')

plt.xlabel('Epochs')
plt.ylabel('Accuracy')

plt.legend()

plt.show()

# Train Again for Final Model
model.fit(
    partial_x_train,
    partial_y_train,
    epochs=3,
    batch_size=512,
    validation_data=(x_val, y_val)
)

# Predictions
result = model.predict(x_test)

print(result)

# Convert Probabilities into 0 or 1
y_pred = np.zeros(len(result))

for i, score in enumerate(result):

    y_pred[i] = 1 if score > 0.5 else 0

# Calculate Error
from sklearn.metrics import mean_absolute_error

mae = mean_absolute_error(y_pred, y_test)

print("Mean Absolute Error:", mae)