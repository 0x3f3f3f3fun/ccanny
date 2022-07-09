import cv2
import numpy as np

image = np.concatenate([
    np.full(shape=(480, 640, 1), fill_value=0, dtype="uint8"),
    np.full(shape=(480, 640, 1), fill_value=1, dtype="uint8"),  
    np.full(shape=(480, 640, 1), fill_value=2, dtype="uint8")
], axis=-1)
cv2.imwrite("./data/test.jpg", image)