import os
import numpy as np
import cv2
import joblib
import json

# 定义Gamma变换函数
def adjust_gamma(image, gamma=1.0):
    invGamma = 1.0 / gamma
    table = np.array([((i / 255.0) ** invGamma) * 255 for i in np.arange(0, 256)]).astype("uint8")
    return cv2.LUT(image, table)

# 定义Gabor特征提取函数
def extract_gabor_features(image):
    gabor_kernels = []
    for theta in np.arange(0, np.pi, np.pi / 4):
        for sigma in (1, 3):
            for frequency in (0.05, 0.25):
                kernel = cv2.getGaborKernel((9, 9), sigma, theta, frequency, 0.5, 0, ktype=cv2.CV_32F)
                gabor_kernels.append(kernel)
    
    features = []
    for kernel in gabor_kernels:
        filtered = cv2.filter2D(image, cv2.CV_8UC3, kernel)
        features.append(filtered)
    
    return np.array(features).flatten()

# 图像文件夹路径
image_folder = r'E:\nitian\shijue\3\code\testvideophotosmall'

# 加载SVM模型
model_path = r'E:\nitian\shijue\3\code\nowmodel\svm_model.pkl'
svm_model = joblib.load(model_path)

# 加载标签与整数编码的对应关系
label_to_index_path = r'E:\nitian\shijue\3\code\nowmodel\label_to_index.json'
with open(label_to_index_path, 'r') as f:
    label_to_index = json.load(f)
index_to_label = {v: k for k, v in label_to_index.items()}

# 加载Haar级联分类器
haar_cascade_path = cv2.data.haarcascades + 'haarcascade_frontalface_default.xml'
face_cascade = cv2.CascadeClassifier(haar_cascade_path)

# 准备数据
data = []
image_names = []

# 遍历文件夹中的所有图像文件
for filename in os.listdir(image_folder):
    if filename.endswith('.jpg') or filename.endswith('.png') or filename.endswith('.tif') or filename.endswith('.bmp'):
        # 读取图像
        image_path = os.path.join(image_folder, filename)
        image = cv2.imread(image_path)
        
        # 转为灰度图
        gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
        
        # 检测人脸
        faces = face_cascade.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=5, minSize=(30, 30))
        
        for (x, y, w, h) in faces:
            # 提取人脸区域
            face = gray[y:y+h, x:x+w]
            
            # 进行Gamma变换
            gamma_corrected = adjust_gamma(face, gamma=1.8)
            
            # 进行高斯模糊处理
            blurred = cv2.GaussianBlur(gamma_corrected, (5, 5), 0)
            
            # 使用Gabor进行特征提取
            gabor_features = extract_gabor_features(blurred)
            
            # 确保特征向量的形状一致
            max_length = svm_model.n_features_in_
            if len(gabor_features) < max_length:
                gabor_features = np.pad(gabor_features, (0, max_length - len(gabor_features)), 'constant')
            else:
                gabor_features = gabor_features[:max_length]
            
            # 保存特征和图像名称
            data.append(gabor_features)
            image_names.append(filename)

# 进行预测
X = np.array(data)
predictions = svm_model.predict(X)
predicted_labels = predictions

# 将预测结果保存到out.txt
output_path = r'E:\nitian\shijue\3\code\videomodel\out.txt'
with open(output_path, 'w') as f:
    for i, pred in enumerate(predicted_labels):
        original_label = index_to_label[pred]
        f.write(f"Image {image_names[i]}: Predicted Label: {pred} ({original_label})\n")

print("人物识别结果已保存到 out.txt")