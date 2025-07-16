import cv2
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.font_manager import FontProperties

# 设置字体以避免中文乱码
font = FontProperties(fname=r"c:\windows\fonts\simsun.ttc", size=14)

# 读取图像
img1 = cv2.imread('E:/nitian/python/homework/test1.bmp', cv2.IMREAD_GRAYSCALE)
img2 = cv2.imread('E:/nitian/python/homework/test2.bmp', cv2.IMREAD_GRAYSCALE)

def sobel_edge_detection(img, ksize=3):
    # 对图像进行Sobel边缘检测
    sobelx = cv2.Sobel(img, cv2.CV_64F, 1, 0, ksize=ksize)
    sobely = cv2.Sobel(img, cv2.CV_64F, 0, 1, ksize=ksize)
    sobel = cv2.magnitude(sobelx, sobely)
    return sobel

def canny_edge_detection(img, threshold1, threshold2):
    # 对图像进行Canny边缘检测
    return cv2.Canny(img, threshold1, threshold2)

def hough_line_detection(img, edges):
    # 对边缘检测结果进行霍夫线变换
    lines = cv2.HoughLinesP(edges, 1, np.pi / 180, threshold=100, minLineLength=50, maxLineGap=10)
    line_img = np.copy(img)
    if lines is not None:
        for line in lines:
            x1, y1, x2, y2 = line[0]
            cv2.line(line_img, (x1, y1), (x2, y2), (255, 0, 0), 2)
    return line_img

# 调整Sobel边缘检测算法的参数
sobel_params = [1, 3, 5]
sobel_results_img1 = [sobel_edge_detection(img1, ksize) for ksize in sobel_params]
sobel_results_img2 = [sobel_edge_detection(img2, ksize) for ksize in sobel_params]

# 调整Canny边缘检测算法的参数
canny_params = [(50, 150), (100, 200), (150, 250)]
canny_results_img1 = [canny_edge_detection(img1, t1, t2) for t1, t2 in canny_params]
canny_results_img2 = [canny_edge_detection(img2, t1, t2) for t1, t2 in canny_params]

# 保存Sobel边缘检测结果
for i, ksize in enumerate(sobel_params):
    cv2.imwrite(f'E:/nitian/python/homework/sobel_img1_ksize_{ksize}.bmp', sobel_results_img1[i])
    cv2.imwrite(f'E:/nitian/python/homework/sobel_img2_ksize_{ksize}.bmp', sobel_results_img2[i])

# 保存Canny边缘检测结果
for i, (t1, t2) in enumerate(canny_params):
    cv2.imwrite(f'E:/nitian/python/homework/canny_img1_t1_{t1}_t2_{t2}.bmp', canny_results_img1[i])
    cv2.imwrite(f'E:/nitian/python/homework/canny_img2_t1_{t1}_t2_{t2}.bmp', canny_results_img2[i])

# 显示Sobel边缘检测结果
fig, axs = plt.subplots(3, 3, figsize=(15, 15))

# 原图
axs[0, 0].imshow(img1, cmap='gray')
axs[0, 0].set_title('原图1', fontproperties=font)
axs[0, 1].imshow(img2, cmap='gray')
axs[0, 1].set_title('原图2', fontproperties=font)

# Sobel边缘检测结果
for i, ksize in enumerate(sobel_params):
    axs[1, i].imshow(sobel_results_img1[i], cmap='gray')
    axs[1, i].set_title(f'Sobel边缘检测1 (ksize={ksize})', fontproperties=font)
    axs[2, i].imshow(sobel_results_img2[i], cmap='gray')
    axs[2, i].set_title(f'Sobel边缘检测2 (ksize={ksize})', fontproperties=font)

plt.show()

# 显示Canny边缘检测结果
fig, axs = plt.subplots(3, 3, figsize=(15, 15))

# 原图
axs[0, 0].imshow(img1, cmap='gray')
axs[0, 0].set_title('原图1', fontproperties=font)
axs[0, 1].imshow(img2, cmap='gray')
axs[0, 1].set_title('原图2', fontproperties=font)

# Canny边缘检测结果
for i, (t1, t2) in enumerate(canny_params):
    axs[1, i].imshow(canny_results_img1[i], cmap='gray')
    axs[1, i].set_title(f'Canny边缘检测1 (t1={t1}, t2={t2})', fontproperties=font)
    axs[2, i].imshow(canny_results_img2[i], cmap='gray')
    axs[2, i].set_title(f'Canny边缘检测2 (t1={t1}, t2={t2})', fontproperties=font)

plt.show()