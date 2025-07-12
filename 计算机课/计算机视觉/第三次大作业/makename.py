import os

def rename_files_in_directory(directory):
    for filename in os.listdir(directory):
        if filename.startswith('db_'):
            new_filename = filename[3:]  # 去掉前缀 'db_'
            os.rename(os.path.join(directory, filename), os.path.join(directory, new_filename))
            print(f'Renamed: {filename} -> {new_filename}')

# 使用示例
directory_path = 'E:\\nitian\\shijue\\3\\code\\testvideophotosmall'
rename_files_in_directory(directory_path)