import cv2
import numpy as np
import os

# 创建输出文件夹
output_folder = 'F:\\python\\Computer_Vision\\output_image'
if not os.path.exists(output_folder):
    os.makedirs(output_folder)

# 对比度增强
def contrast_enhance(image, alpha):
    enhanced_image = np.clip(alpha * image, 0, 255).astype(np.uint8)
    return enhanced_image

# 亮度增强
def brightness_enhance(image, beta):
    # 确保处理彩色图像时每个通道都进行亮度增强
    if len(image.shape) == 3:  # 彩色图像
        # 将图像从RGB转换为HSV
        hsv = np.zeros_like(image, dtype=np.float32)
        for i in range(image.shape[0]):
            for j in range(image.shape[1]):
                r, g, b = image[i, j] / 255.0
                mx = max(r, g, b)
                mn = min(r, g, b)
                diff = mx - mn
                if mx == mn:
                    h = 0
                elif mx == r:
                    h = (60 * ((g - b) / diff) + 360) % 360
                elif mx == g:
                    h = (60 * ((b - r) / diff) + 120) % 360
                elif mx == b:
                    h = (60 * ((r - g) / diff) + 240) % 360
                if mx == 0:
                    s = 0
                else:
                    s = diff / mx
                v = mx
                hsv[i, j] = [h, s, v]
        
        # 增强亮度
        hsv[:, :, 2] = np.clip(hsv[:, :, 2] * 255 + beta, 0, 255) / 255.0
        
        # 将图像从HSV转换回RGB
        enhanced_image = np.zeros_like(image, dtype=np.uint8)
        for i in range(hsv.shape[0]):
            for j in range(hsv.shape[1]):
                h, s, v = hsv[i, j]
                c = v * s
                x = c * (1 - abs((h / 60) % 2 - 1))
                m = v - c
                if 0 <= h < 60:
                    r, g, b = c, x, 0
                elif 60 <= h < 120:
                    r, g, b = x, c, 0
                elif 120 <= h < 180:
                    r, g, b = 0, c, x
                elif 180 <= h < 240:
                    r, g, b = 0, x, 
                elif 240 <= h < 300:
                    r, g, b = x, 0, c
                elif 300 <= h < 360:
                    r, g, b = c, 0, x
                enhanced_image[i, j] = [(r + m) * 255, (g + m) * 255, (b + m) * 255]
    else:  #如果没彩色通道就是灰度图像
        enhanced_image = np.clip(image + beta, 0, 255).astype(np.uint8)
    return enhanced_image

def equalize_hist(image):
    # 计算直方图
    hist, bins = np.histogram(image.flatten(), 256, [0, 256])
    # 计算累积分布函数
    cdf = hist.cumsum()
    # 归一化
    cdf_normalized = cdf * (255 / cdf[-1])
    # 使用累积分布函数进行直方图均衡化
    equalized_image = np.interp(image.flatten(), bins[:-1], cdf_normalized)
    return equalized_image.reshape(image.shape).astype(np.uint8)

def process_image(image_path, output_path):
    # 读取图像
    image = cv2.imread(image_path)

    # 自行进行图像增强操作并输出
    # 对比度增强
    contrast_image = contrast_enhance(image, 1.5)  # alpha > 1 增强对比度
    cv2.imwrite(os.path.join(output_path, 'contrast_image.png'), contrast_image)
    # 亮度增强
    brightness_image = brightness_enhance(image, 50)  # beta > 0 增强亮度
    cv2.imwrite(os.path.join(output_path, 'brightness_image.png'), brightness_image)
    # 直方图均衡化
    gray_image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    equalized_image = equalize_hist(gray_image)
    cv2.imwrite(os.path.join(output_path, 'equalized_image.png'), equalized_image)
    
    # 使用OpenCV库函数进行图像增强操作并输出以对比
    # 对比度增强
    contrast_image_cv = cv2.addWeighted(image, 1.5, np.zeros(image.shape, image.dtype), 0, 0)
    cv2.imwrite(os.path.join(output_path, 'contrast_image_cv.png'), contrast_image_cv)
    # 亮度增强
    brightness_image_cv = cv2.add(image, 50)
    cv2.imwrite(os.path.join(output_path, 'brightness_image_cv.png'), brightness_image_cv)
    # 直方图均衡化
    equalized_image_cv = cv2.equalizeHist(gray_image)
    cv2.imwrite(os.path.join(output_path, 'equalized_image_cv.png'), equalized_image_cv)

# 获取“测试图像”文件夹中的所有图像文件
folder_path = 'F:\\python\\Computer_Vision\\test_image'
image_files = [f for f in os.listdir(folder_path) if os.path.isfile(os.path.join(folder_path, f))]

# 对每个图像文件进行处理
for image_file in image_files:
    image_path = os.path.join(folder_path, image_file)
    output_path = os.path.join(output_folder, os.path.splitext(image_file)[0])
    if not os.path.exists(output_path):
        os.makedirs(output_path)
    process_image(image_path, output_path)
