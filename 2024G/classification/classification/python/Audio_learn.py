import pandas as pd
import torch
import torch.nn as nn
import torch.optim as optim
from sklearn.preprocessing import StandardScaler
from sklearn.model_selection import train_test_split
from torch.utils.data import DataLoader, TensorDataset
import numpy as np

# Constants
BATCH_SIZE = 64
LEARNING_RATE = 0.001
NUM_EPOCHS = 50

def log_scaling(features):
    # 对数据取对数，避免对数的负无穷大，使用np.clip保证最小值大于0
    return np.log1p(np.clip(features, a_min=1e-10, a_max=None))

def power_spectral_density(features):
    # 计算功率谱密度
    return features ** 2

# Function to load and preprocess data
# Function to load and preprocess data
def load_data():
    # Read data
    music_features = pd.read_csv('music_10240.csv', header=None, skiprows=0, usecols=range(0, 511))
    voice_features = pd.read_csv('voice_10240.csv', header=None, skiprows=0, usecols=range(0, 511))

    # Check shapes
    print("Music features shape:", music_features.shape)
    print("Voice features shape:", voice_features.shape)

    assert music_features.shape[1] == 511, "Music data should have 511 columns"
    assert voice_features.shape[1] == 511, "Voice data should have 511 columns"

    # Add labels
    music_labels = pd.Series([0] * len(music_features), dtype='int64')  # music = 0
    voice_labels = pd.Series([1] * len(voice_features), dtype='int64')  # voice = 1

    # Ensure no NaN values in features and labels
    music_features.dropna(inplace=True)
    voice_features.dropna(inplace=True)

    # Recreate labels after dropping NaN
    music_labels = pd.Series([0] * len(music_features), dtype='int64')
    voice_labels = pd.Series([1] * len(voice_features), dtype='int64')

    # Check label lengths
    assert len(music_features) == len(music_labels), "Mismatch in music features and labels after dropping NaNs"
    assert len(voice_features) == len(voice_labels), "Mismatch in voice features and labels after dropping NaNs"

    # Concatenate data
    features = pd.concat([music_features, voice_features], ignore_index=True)
    labels = pd.concat([music_labels, voice_labels], ignore_index=True)

    # Ensure no NaN values
    features.dropna(inplace=True)
    labels.dropna(inplace=True)

    # Remove mismatched entries (just in case)
    min_length = min(len(features), len(labels))
    features = features.iloc[:min_length]
    labels = labels.iloc[:min_length]

    # Check concatenated shapes
    print("Concatenated features shape:", features.shape)
    print("Concatenated labels shape:", labels.shape)

    # Convert to PyTorch tensors
    features_tensor = torch.tensor(features.values, dtype=torch.float32)
    labels_tensor = torch.tensor(labels.values, dtype=torch.long)

    return features_tensor, labels_tensor
#     # Read data
#     music_features = pd.read_csv('music_10240.csv', header=None, skiprows=0, usecols=range(0, 511))
#     voice_features = pd.read_csv('voice_10240.csv', header=None, skiprows=0, usecols=range(0, 511))

#     assert music_features.shape[1] == 511, "Music data should have 512 columns"
#     assert voice_features.shape[1] == 511, "Voice data should have 512 columns"


#     # Add labels
#     music_labels = pd.Series([0] * len(music_features), dtype='int64')  # music = 0
#     voice_labels = pd.Series([1] * len(voice_features), dtype='int64')  # voice = 1

#     # Concatenate data
#     features = pd.concat([music_features, voice_features], ignore_index=True)
#     labels = pd.concat([music_labels, voice_labels], ignore_index=True)

#     # # Standardize features
#     scaler = StandardScaler()
#     features = scaler.fit_transform(features)

#     # Convert to PyTorch tensors
#     features_tensor = torch.tensor(features, dtype=torch.float32)
#     labels_tensor = torch.tensor(labels.values, dtype=torch.long)

#     return features_tensor, labels_tensor

# Load data
features_tensor, labels_tensor = load_data()

# Split dataset
X_train, X_test, y_train, y_test = train_test_split(features_tensor, labels_tensor, test_size=0.2, random_state=42)

# Create DataLoaders
train_dataset = TensorDataset(X_train, y_train)
test_dataset = TensorDataset(X_test, y_test)
train_loader = DataLoader(train_dataset, batch_size=BATCH_SIZE, shuffle=True)
test_loader = DataLoader(test_dataset, batch_size=BATCH_SIZE, shuffle=False)

# Define the neural network model
class ImprovedNN(nn.Module):
    def __init__(self):
        super(ImprovedNN, self).__init__()
        self.fc1 = nn.Linear(511, 32)
        self.fc2 = nn.Linear(32, 2)
        self.relu = nn.ReLU()

    def forward(self, x):
        x = self.relu(self.fc1(x))
        x = self.fc2(x)
        return x

# Initialize model, loss function, and optimizer
model = ImprovedNN()
criterion = nn.CrossEntropyLoss()
optimizer = optim.Adam(model.parameters(), lr=LEARNING_RATE)

# Training function
def train(model, dataloader, criterion, optimizer, num_epochs=200):
    model.train()
    for epoch in range(num_epochs):
        running_loss = 0.0
        for inputs, labels in dataloader:
            optimizer.zero_grad()  # Zero the gradients
            outputs = model(inputs)
            loss = criterion(outputs, labels)
            loss.backward()  # Backpropagation
            optimizer.step()  # Update weights
            running_loss += loss.item() * inputs.size(0)
        epoch_loss = running_loss / len(dataloader.dataset)
        print(f'Epoch {epoch+1}/{num_epochs}, Loss: {epoch_loss:.4f}')

# Train the model
train(model, train_loader, criterion, optimizer, num_epochs=NUM_EPOCHS)

# Evaluation function
def evaluate(model, dataloader):
    model.eval()
    correct = 0
    total = 0
    with torch.no_grad():  # Disable gradient tracking
        for inputs, labels in dataloader:
            outputs = model(inputs)
            _, predicted = torch.max(outputs, 1)
            total += labels.size(0)
            correct += (predicted == labels).sum().item()
    accuracy = correct / total
    return accuracy

# Evaluate the model
accuracy_test = evaluate(model, test_loader)
print(f'Accuracy test_loader: {accuracy_test:.4f}')
accuracy_train = evaluate(model, train_loader)
print(f'Accuracy train_loader: {accuracy_train:.4f}')

# 提取权重
fc1_weights = model.fc1.weight.detach().numpy()
fc1_bias = model.fc1.bias.detach().numpy()
fc2_weights = model.fc2.weight.detach().numpy()
fc2_bias = model.fc2.bias.detach().numpy()

# 保存为C头文件格式
def save_to_c_header(weights, bias, layer_name):
    with open(f"{layer_name}.h", "w") as f:
        f.write(f"float {layer_name}_weights[] = {{")
        for w in weights.flatten():
            f.write(f"{w:.6f}f, ")
        f.write("};\n")

        f.write(f"float {layer_name}_bias[] = {{")
        for b in bias:
            f.write(f"{b:.6f}f, ")
        f.write("};\n")

save_to_c_header(fc1_weights, fc1_bias, "fc1")
save_to_c_header(fc2_weights, fc2_bias, "fc2")

# 权重量化
def quantize_tensor(tensor, num_bits=8):
    qmin = -128
    qmax = 127
    
    # 将张量转换为 NumPy 数组
    tensor_np = tensor.detach().cpu().numpy()
    
    min_val = np.min(tensor_np)
    max_val = np.max(tensor_np)
    
    # 计算 scale 和 zero point
    scale = (max_val - min_val) / (qmax - qmin)
    zero_point = int(round(qmin - min_val / scale))
    
    # 应用量化
    quantized_tensor = np.clip(np.round(tensor_np / scale) + zero_point, qmin, qmax).astype(np.int8)
    
    return torch.from_numpy(quantized_tensor), scale, zero_point

# 量化权重和偏置
fc1_weights_q, fc1_scale, fc1_zero_point = quantize_tensor(model.fc1.weight)
fc1_bias_q, _, _ = quantize_tensor(model.fc1.bias)
fc2_weights_q, fc2_scale, fc2_zero_point = quantize_tensor(model.fc2.weight)
fc2_bias_q, _, _ = quantize_tensor(model.fc2.bias)

# 打印量化后的权重
print("FC1 Weights Quantized:\n", fc1_weights_q)
print("FC1 Bias Quantized:\n", fc1_bias_q)
print("FC2 Weights Quantized:\n", fc2_weights_q)
print("FC2 Bias Quantized:\n", fc2_bias_q)

# 保存量化权重到C头文件
def save_quantized_weights(weights, bias, scale, zero_point, layer_name):
    with open(f"{layer_name}_quant.h", "w") as f:
        f.write(f"int8_t {layer_name}_weights[] = {{")
        for w in weights.flatten():
            f.write(f"{w}, ")
        f.write("};\n")
        
        f.write(f"int8_t {layer_name}_bias[] = {{")
        for b in bias:
            f.write(f"{b}, ")
        f.write("};\n")
        
        f.write(f"float {layer_name}_scale = {scale:.6f}f;\n")
        f.write(f"int {layer_name}_zero_point = {zero_point};\n")

save_quantized_weights(fc1_weights_q, fc1_bias_q, fc1_scale, fc1_zero_point, "fc1")
save_quantized_weights(fc2_weights_q, fc2_bias_q, fc2_scale, fc2_zero_point, "fc2")
