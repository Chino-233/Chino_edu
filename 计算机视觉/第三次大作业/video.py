# 导入操作系统模块
import os
# 导入NumPy库
import numpy as np
# 导入OpenCV库
import cv2
# 导入joblib库，用于加载模型
import joblib
# 导入json模块，用于处理JSON数据
import json
# 导入time模块
import time

# 定义Gamma变换函数，用于调整图像的亮度
def adjust_gamma(image, gamma=1.0):
    # 计算逆Gamma值
    invGamma = 1.0 / gamma
    # 创建查找表，将每个像素值映射到新的值
    table = np.array([((i / 255.0) ** invGamma) * 255 for i in np.arange(0, 256)]).astype("uint8")
    # 使用查找表对图像进行Gamma变换
    return cv2.LUT(image, table)

# 定义Gabor特征提取函数
def extract_gabor_features(image):
    # 创建一个空列表，用于存储Gabor核
    gabor_kernels = []
    # 遍历不同的方向
    for theta in np.arange(0, np.pi, np.pi / 4):
        # 遍历不同的sigma值
        for sigma in (1, 3):
            # 遍历不同的频率
            for frequency in (0.05, 0.25):
                # 创建Gabor核
                kernel = cv2.getGaborKernel((9, 9), sigma, theta, frequency, 0.5, 0, ktype=cv2.CV_32F)
                # 将Gabor核添加到列表中
                gabor_kernels.append(kernel)
    
    # 创建一个空列表，用于存储特征
    features = []
    # 遍历每个Gabor核
    for kernel in gabor_kernels:
        # 使用Gabor核对图像进行滤波
        filtered = cv2.filter2D(image, cv2.CV_8UC3, kernel)
        # 将滤波后的图像添加到特征列表中
        features.append(filtered)
    
    # 将特征列表展平为一维数组并返回
    return np.array(features).flatten()

# 加载预训练的SVM模型
model_path = r'E:\nitian\shijue\3\code\nowmodel\svm_model.pkl'
svm_model = joblib.load(model_path)

# 加载标签与整数编码的对应关系
label_to_index_path = r'E:\nitian\shijue\3\code\nowmodel\label_to_index.json'
with open(label_to_index_path, 'r') as f:
    label_to_index = json.load(f)
# 将标签与整数编码的对应关系反转，方便后续使用
index_to_label = {v: k for k, v in label_to_index.items()}

# 加载Haar级联分类器，用于人脸检测
haar_cascade_path = cv2.data.haarcascades + 'haarcascade_frontalface_default.xml'
face_cascade = cv2.CascadeClassifier(haar_cascade_path)

# 加载一个字体，用于在图像上标注识别结果
font = cv2.FONT_HERSHEY_SIMPLEX

# 打开摄像头
cam = cv2.VideoCapture(0)
# 设置检测人脸的最小宽度和高度
minW = 0.1 * cam.get(3)
minH = 0.1 * cam.get(4)

while True:
    # 从摄像头读取一帧图像
    ret, img = cam.read()
    # 将图像转换为灰度图
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    
    # 使用Haar级联分类器检测人脸
    faces = face_cascade.detectMultiScale(
        gray,
        scaleFactor=1.2,
        minNeighbors=5,
        minSize=(int(minW), int(minH))
    )
    
    # 遍历检测到的人脸
    for (x, y, w, h) in faces:
        # 提取人脸区域并进行预处理
        face = gray[y:y + h, x:x + w]
        # 进行Gamma变换
        gamma_corrected = adjust_gamma(face, gamma=1.8)
        # 进行高斯模糊
        blurred = cv2.GaussianBlur(gamma_corrected, (5, 5), 0)
        # 使用Gabor进行特征提取
        gabor_features = extract_gabor_features(blurred)
        
        # 确保特征向量的形状一致
        max_length = svm_model.n_features_in_
        if len(gabor_features) < max_length:
            gabor_features = np.pad(gabor_features, (0, max_length - len(gabor_features)), 'constant')
        else:
            gabor_features = gabor_features[:max_length]
        
        # 进行预测
        X = np.array([gabor_features])
        predictions = svm_model.predict_proba(X)
        predicted_label = np.argmax(predictions, axis=1)[0]
        confidence = np.max(predictions) * 100  # 计算置信度
        
        if confidence > 40: 
            original_label = index_to_label[predicted_label]
            # 在图像上绘制绿色矩形框标记人脸
            cv2.rectangle(img, (x, y), (x + w, y + h), (0, 255, 0), 2)
            cv2.putText(img, f"{original_label}", (x + 5, y - 5), font, 1, (0, 255, 0), 2)
        else:
            # 在图像上绘制红色矩形框标记未识别的人脸
            cv2.rectangle(img, (x, y), (x + w, y + h), (0, 0, 255), 2)
            cv2.putText(img, "Unknown", (x + 5, y - 5), font, 1, (0, 0, 255), 2)

    # 展示结果
    cv2.imshow('press esc to exit', img)
    k = cv2.waitKey(1)
    if k == 27:  # 按下ESC键退出
        break

# 释放资源
cam.release()
cv2.destroyAllWindows()