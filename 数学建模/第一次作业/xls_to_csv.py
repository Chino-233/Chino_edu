import pandas as pd

# 读取Excel文件
# 请将"crab_data.xls"替换为您实际的Excel文件路径
df = pd.read_excel("数学建模\第一次作业\梭子蟹数据.xls")

# 将DataFrame保存为CSV
df.to_csv("data.csv", index=False, encoding='utf-8-sig')

print("Excel文件已成功转换为CSV: crab_data.csv")