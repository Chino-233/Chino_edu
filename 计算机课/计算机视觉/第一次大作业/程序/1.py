import cv2
import numpy as np
import os

# 创建输出文件夹
output_folder = 'F:\\python\\Computer_Vision\\output_image'
if not os.path.exists(output_folder):
    os.makedirs(output_folder)
    
def process_image(image_path, output_path):
    # 读取图像
    image = cv2.imread(image_path)
    # 提取RGB颜色空间各通道图像
    b_channel, g_channel, r_channel = cv2.split(image)
    # 保存各通道图像
    cv2.imwrite(os.path.join(output_path, 'channel_1_R.png'), r_channel)
    cv2.imwrite(os.path.join(output_path, 'channel_3_B.png'), b_channel)
    cv2.imwrite(os.path.join(output_path, 'channel_2_G.png'), g_channel)
    # 转换为HSV颜色空间
    hsv_image = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
    h_channel, s_channel, v_channel = cv2.split(hsv_image)
    # 保存各通道图像
    cv2.imwrite(os.path.join(output_path, 'channel_1_H.png'), h_channel)
    cv2.imwrite(os.path.join(output_path, 'channel_2_S.png'), s_channel)
    cv2.imwrite(os.path.join(output_path, 'channel_3_V.png'), v_channel)
    # 合并RGB颜色空间各通道图像生成彩色图像
    merged_rgb = cv2.merge([b_channel, g_channel, r_channel])
    # 合并HSV颜色空间各通道图像生成彩色图像
    merged_hsv = cv2.cvtColor(cv2.merge([h_channel, s_channel, v_channel]), cv2.COLOR_HSV2BGR)
    # 保存合并后的图像
    cv2.imwrite(os.path.join(output_path, 'merged_rgb.png'), merged_rgb)
    cv2.imwrite(os.path.join(output_path, 'merged_hsv.png'), merged_hsv)
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