import matplotlib.pyplot as plt
import numpy as np
plt.rcParams['font.sans-serif'] = ['SimHei']  
plt.rcParams['axes.unicode_minus'] = False  
# 数据
matrix_sizes = [100, 500, 1000, 1500, 2000, 2500, 3000]
regular_times = [0.002140, 0.313913, 2.935889, 14.021960, 26.693109, 75.844532, 125.75506]
optimized_times = [0.001730, 0.263278, 2.039237, 6.683319, 11.911616, 33.672514, 52.438825]
speedups = [1.2374, 1.5324, 1.4397, 2.0981, 2.2409, 2.2524, 2.3981]

# 创建图表
plt.figure(figsize=(10, 6))

# 绘制加速比折线图
plt.plot(matrix_sizes, speedups, 'o-', color='blue', linewidth=2, markersize=8, label='加速比')
for i, (size, speedup) in enumerate(zip(matrix_sizes, speedups)):
    plt.annotate(f'{speedup:.2f}', (size, speedup), textcoords="offset points", 
                 xytext=(0,10), ha='center', fontsize=9)

# 设置图表标题和轴标签
plt.title('矩阵大小与加速比关系', fontsize=16)
plt.xlabel('矩阵大小', fontsize=14)
plt.ylabel('加速比(Speedup)', fontsize=14)
plt.grid(True, linestyle='--', alpha=0.7)
plt.xticks(matrix_sizes)

# 添加图例
plt.legend(loc='best')

# 优化布局
plt.tight_layout()

# 保存图表
plt.savefig('matrix_speedup.png', dpi=300)

# 显示图表
plt.show()