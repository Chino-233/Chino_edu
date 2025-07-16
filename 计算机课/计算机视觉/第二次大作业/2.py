import cv2
import numpy as np

def process_frame(frame):
    # 转换为灰度图像
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    # 图像均值化处理
    #mean = cv2.equalizeHist(gray)
    # 应用高斯模糊以减少噪声
    blur = cv2.GaussianBlur(gray, (7, 7), 0)
    # 使用Canny算法进行边缘检测
    edges = cv2.Canny(blur, 25, 150)
    return gray,blur,edges

def draw_lines(frame, edges):
    # 使用Hough变换检测直线
    lines = cv2.HoughLines(edges, 1, np.pi / 180, 220)
    # 如果检测到直线，则绘制它们
    if lines is not None:
        # 遍历lines中的每一行
        for rho, theta in lines[:, 0, :]:
            # 如果theta小于2，则跳过该行
            if theta < 2 :
                continue
            # 计算cos(theta)和sin(theta)
            a = np.cos(theta)
            b = np.sin(theta)
            # 计算rho对应的x和y坐标
            x0 = a * rho
            y0 = b * rho
            # 计算x1和y1的坐标
            x1 = int(x0 - 350 * (-b))
            y1 = int(y0 - 350 * a)
            # 计算x2和y2的坐标
            x2 = int(x0 - 700 * (-b))
            y2 = int(y0 - 700 * a)
            # 在图像上绘制直线
            cv2.line(frame, (x1, y1), (x2, y2), (0, 255, 0), 4)
    
    return frame

def save_intermediate_frames(gray,blur,edges,output_prefix):
    # 保存灰度图像
    cv2.imwrite(f"{output_prefix}_gray.jpg", gray)
    # 保存均值化图像
    #cv2.imwrite(f"{output_prefix}_mean.jpg", mean)
    noblur=cv2.Canny(gray,25,150)
    cv2.imwrite(f"{output_prefix}_nobluredges.jpg", noblur)
    # 保存高模糊图像
    cv2.imwrite(f"{output_prefix}_blur.jpg", blur)
    # 保存边缘检测图像
    cv2.imwrite(f"{output_prefix}_edges.jpg", edges)

def process_video(input_path, output_path, frame_output_prefix):
    # 打开视频文件
    cap = cv2.VideoCapture(input_path)
    # 定义视频编解码器并创建VideoWriter对象
    fourcc = cv2.VideoWriter_fourcc(*'XVID')
    out = cv2.VideoWriter(output_path, fourcc, 20.0, (int(cap.get(3)), int(cap.get(4))))
    
    frame_count = 0
    while cap.isOpened():
        # 逐帧读取视频
        ret, frame = cap.read()
        if not ret:
            break
        
        frame_count += 1
        # 处理每一帧
        gray,blur,edges, = process_frame(frame)
        
        # 如果是第十帧，保存每一步处理后的图像
        if frame_count == 10:
            cv2.imwrite(f"{frame_output_prefix}_color.jpg", frame)
            save_intermediate_frames(gray, blur, edges, frame_output_prefix)
        
        # 在处理后的帧上绘制直线
        processed_frame = draw_lines(frame, edges)
        # 将处理后的帧写入输出视频
        out.write(processed_frame)
    
    # 释放视频捕获和写入对象
    cap.release()
    out.release()

# 处理视频文件列表
video_files = ["E:\\nitian\\shijue\\2\\01.avi", "E:\\nitian\\shijue\\2\\02.avi", "E:\\nitian\\shijue\\2\\03.avi"]
for video_file in video_files:
    # 为每个输入视频生成输出文件路径
    output_file = video_file.replace(".avi", "_processed.avi")
    # 为每个输入视频生成第十帧的输出图像前缀
    frame_output_prefix = video_file.replace(".avi", "_frame10")
    # 处理视频并保存结果
    process_video(video_file, output_file, frame_output_prefix)