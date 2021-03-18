!#/usr/bin/python3
import numpy as np
import cv2

# Load an color image in grayscale
print("Hello world")
img = cv2.imread('arroz.bmp',0)
cv2.imshow('image',img)
cv2.waitKey(0)
cv2.destroyAllWindows()