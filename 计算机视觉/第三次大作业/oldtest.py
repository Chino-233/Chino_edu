# 导入操作系统模块
import os
# 禁用TensorFlow的OneDNN优化
os.environ['TF_ENABLE_ONEDNN_OPTS'] = '0'

# 导入OpenCV库
import cv2
# 导入NumPy库
import numpy as np
# 导入sklearn库中的train_test_split函数，用于划分训练集和测试集
from sklearn.model_selection import train_test_split
# 导入sklearn库中的SVC类，用于构建支持向量机模型
from sklearn.svm import SVC
# 导入sklearn库中的accuracy_score函数，用于计算模型的准确率
from sklearn.metrics import accuracy_score
# 导入json模块，用于处理JSON数据
import json
# 导入joblib库，用于保存模型
import joblib

# 定义Gamma变换函数，用于调整图像的亮度
def adjust_gamma(image, gamma=1.0):
    # 计算逆Gamma值
    invGamma = 1.0 / gamma
    # 创建查找表，用于快速进行Gamma变换
    table = np.array([((i / 255.0) ** invGamma) * 255 for i in np.arange(0, 256)]).astype("uint8")
    # 使用查找表对图像进行Gamma变换
    return cv2.LUT(image, table)

# 定义Gabor特征提取函数，用于提取图像的Gabor特征
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
                # 将·核添加到列表中
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

# 加载Haar级联分类器，用于人脸检测
face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')

# 图像文件夹路径
image_folder = r'E:\nitian\shijue\3\code\nowphoto'
# 输出文件夹路径
output_folder = r'E:\nitian\shijue\3\code\faceofhaar'

# 创建输出文件夹，如果不存在的话
if not os.path.exists(output_folder):
    os.makedirs(output_folder)

# 准备数据列表和标签列表
data = []
labels = []

# 遍历文件夹中的所有图像文件
for filename in os.listdir(image_folder):
    if filename.endswith('.jpg') or filename.endswith('.png') or filename.endswith('.tif') or filename.endswith('.bmp'):
        # 读取图像
        image_path = os.path.join(image_folder, filename)
        image = cv2.imread(image_path)
        
        # 将图像转换为灰度图
        gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
        
        # 检测人脸
        faces = face_cascade.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=5, minSize=(30, 30))
        
        # 如果检测到人脸，使用人脸区域
        for (x, y, w, h) in faces:
            face = gray[y:y+h, x:x+w]
            
            # 保存人脸图像到输出文件夹
            face_filename = os.path.join(output_folder, f"{filename.split('.')[0]}_face.jpg")
            cv2.imwrite(face_filename, face)
            
            # 进行Gamma变换
            gamma_corrected = adjust_gamma(face, gamma=1.8)
            
            # 进行高斯模糊处理
            blurred = cv2.GaussianBlur(gamma_corrected, (5, 5), 0)
            
            # 使用Gabor进行特征提取
            gabor_features = extract_gabor_features(blurred)
            
            # 从文件名中提取标签
            label = filename.split('_')[0]
            
            # 保存特征和标签
            data.append(gabor_features)
            labels.append(label)

# 确保所有特征向量的形状一致
max_length = max(len(features) for features in data)
data = [np.pad(features, (0, max_length - len(features)), 'constant') for features in data]

# 将标签转换为整数编码
unique_labels = list(set(labels))
label_to_index = {label: index for index, label in enumerate(unique_labels)}
y = np.array([label_to_index[label] for label in labels])

# 保存标签与整数编码的对应关系
with open(r'E:\nitian\shijue\3\code\nowmodel\label_to_index.json', 'w') as f:
    json.dump(label_to_index, f)

# 划分训练集和测试集
X_train, X_test, y_train, y_test = train_test_split(data, y, test_size=0.2, random_state=42)

# 构建SVM模型
svm_model = SVC(kernel='linear', probability=True)

# 训练SVM模型
svm_model.fit(X_train, y_train)

# 评估模型
y_pred = svm_model.predict(X_test)
accuracy = accuracy_score(y_test, y_pred)
print(f"SVM Accuracy: {accuracy}")

# 保存模型
joblib.dump(svm_model, r'E:\nitian\shijue\3\code\nowmodel\svm_model.pkl')

# 将数据和标签保存为.npy文件
np.save(r'E:\nitian\shijue\3\code\nowmodel\features.npy', np.array(data))
np.save(r'E:\nitian\shijue\3\code\nowmodel\labels.npy', y)