import os
import shutil
import tkinter as tk
from tkinter import filedialog, messagebox

# 设置上传文件夹路径
UPLOAD_FOLDER = r'E:\nitian\shijue\3\code\nowphoto'

# 允许的文件扩展名
ALLOWED_EXTENSIONS = {'png', 'jpg', 'jpeg', 'bmp', 'tif'}

def allowed_file(filename):
    return '.' in filename and filename.rsplit('.', 1)[1].lower() in ALLOWED_EXTENSIONS

def upload_file():
    # 打开文件选择对话框
    file_path = filedialog.askopenfilename()
    if not file_path:
        return
    
    # 获取文件名和扩展名
    file_name = os.path.basename(file_path)
    file_ext = file_name.rsplit('.', 1)[1].lower()
    
    if not allowed_file(file_name):
        messagebox.showerror("错误", "文件类型不允许")
        return
    
    # 获取用户输入的文件前缀
    prefix = prefix_entry.get()
    if not prefix:
        messagebox.showerror("错误", "文件前缀不能为空")
        return
    
    # 构建新的文件名，默认添加 _1
    new_file_name = f"{prefix}_1.{file_ext}"
    new_file_path = os.path.join(UPLOAD_FOLDER, new_file_name)
    
    # 如果文件已存在，添加编号
    counter = 2
    while os.path.exists(new_file_path):
        new_file_name = f"{prefix}_{counter}.{file_ext}"
        new_file_path = os.path.join(UPLOAD_FOLDER, new_file_name)
        counter += 1
    
    # 保存文件到目标文件夹
    shutil.move(file_path, new_file_path)
    
    messagebox.showinfo("成功", "文件上传成功")

# 创建主窗口
root = tk.Tk()
root.title("文件上传")
root.geometry("600x400")  # 设置窗口大小为600x400

# 创建并放置标签和输入框
tk.Label(root, text="用户名:", font=("Arial", 14)).pack(pady=10)
prefix_entry = tk.Entry(root, font=("Arial", 14))
prefix_entry.pack(pady=10)

# 创建并放置上传按钮
upload_button = tk.Button(root, text="上传文件", command=upload_file, font=("Arial", 14))
upload_button.pack(pady=20)

# 运行主循环
root.mainloop()