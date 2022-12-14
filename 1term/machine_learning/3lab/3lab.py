import numpy as np
numChannels = 3
width = 2
height = 3
channel_weights = [0.5, 0.9, 0.4]
print(f"Channel weights: {channel_weights}")
image = np.random.random((width, height, numChannels))
result = np.zeros((width,height))
print("Original 3d array")
print(image)
for i, array in enumerate(image):
    for j, array2 in enumerate(array):
        element_sum = np.sum(array2*channel_weights)
        result[i,j] = element_sum
print("Result:")
print(result)

