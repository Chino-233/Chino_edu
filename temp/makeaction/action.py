import numpy as np
import matplotlib.pyplot as plt
from matplotlib.widgets import Slider, Button, CheckButtons
import time
import math
import matplotlib
from matplotlib.animation import FuncAnimation

# 设置中文字体支持
plt.rcParams['font.sans-serif'] = ['SimHei']  # 使用黑体
plt.rcParams['axes.unicode_minus'] = False    # 正确显示负号

class Point:
    def __init__(self, x, y, id=-1):
        self.x = x
        self.y = y
        self.id = id
    
    @staticmethod
    def distance(p1, p2):
        return np.sqrt((p1.x - p2.x) ** 2 + (p1.y - p2.y) ** 2)
    
    def __repr__(self):
        return f"Point({self.x:.2f}, {self.y:.2f})"

def generate_random_points(n, min_val=0, max_val=1000):
    """生成随机点"""
    np.random.seed(42)  # 使结果可重现
    x_coords = np.random.uniform(min_val, max_val, n)
    y_coords = np.random.uniform(min_val, max_val, n)
    return [Point(x_coords[i], y_coords[i], i) for i in range(n)]

# ------------- 蛮力法相关函数 -------------
def brute_force_with_steps(points, ax=None, update_func=None):
    """带可视化步骤的蛮力法"""
    n = len(points)
    min_dist = float('inf')
    closest_pair = None
    steps = []
    
    # 截至目前全局最小点对
    global_min_dist = float('inf')
    global_min_pair = None
    
    for i in range(n):
        for j in range(i + 1, n):
            current_pair = (points[i], points[j])
            dist = Point.distance(points[i], points[j])
            
            # 更新全局最小点对（如果适用）
            if dist < global_min_dist:
                global_min_dist = dist
                global_min_pair = current_pair
            
            # 记录当前比较步骤和当前最小点对
            steps.append({
                'type': 'compare',
                'pair': current_pair,
                'dist': dist,
                'is_min': dist < min_dist,
                'current_min_pair': closest_pair,
                'current_min_dist': min_dist,
                'global_min_pair': global_min_pair,
                'global_min_dist': global_min_dist
            })
            
            if dist < min_dist:
                min_dist = dist
                closest_pair = current_pair
                # 记录新的最小值
                steps.append({
                    'type': 'new_min',
                    'pair': closest_pair,
                    'dist': min_dist,
                    'current_min_pair': closest_pair,
                    'current_min_dist': min_dist,
                    'global_min_pair': global_min_pair,
                    'global_min_dist': global_min_dist
                })
            
            # 如果提供了可视化功能，更新显示
            if ax and update_func and len(steps) % 10 == 0:  # 每10步更新一次以提高性能
                update_func(ax, steps[-1], closest_pair, min_dist)
    
    return closest_pair, min_dist, steps

# ------------- 分治法相关函数 -------------
def closest_crossing_pair_with_steps(points_by_y, mid_x, min_dist, current_min_pair=None, steps=None, global_min_pair=None, global_min_dist=float('inf')):
    """带可视化步骤的跨越中线处理"""
    if steps is None:
        steps = []
    
    # 筛选出在中线±min_dist范围内的点
    strip = [p for p in points_by_y if abs(p.x - mid_x) < min_dist]
    
    # 记录带状区域
    steps.append({
        'type': 'strip',
        'mid_x': mid_x,
        'width': min_dist,
        'points': strip,
        'current_min_pair': current_min_pair,
        'current_min_dist': min_dist,
        'global_min_pair': global_min_pair,
        'global_min_dist': global_min_dist
    })
    
    strip_size = len(strip)
    if strip_size <= 1:
        return None, float('inf'), steps
    
    min_strip_dist = min_dist
    closest_pair = None
    
    # 在带状区域内查找最近点对
    for i in range(strip_size):
        j = i + 1
        while j < strip_size and (strip[j].y - strip[i].y) < min_dist:
            current_pair = (strip[i], strip[j])
            dist = Point.distance(strip[i], strip[j])
            
            # 更新全局最小点对（如果适用）
            if dist < global_min_dist:
                global_min_dist = dist
                global_min_pair = current_pair
            
            # 记录比较步骤
            steps.append({
                'type': 'strip_compare',
                'pair': current_pair,
                'dist': dist,
                'is_min': dist < min_strip_dist,
                'current_min_pair': closest_pair if closest_pair else current_min_pair,
                'current_min_dist': min(min_strip_dist, min_dist) if closest_pair else min_dist,
                'global_min_pair': global_min_pair,
                'global_min_dist': global_min_dist
            })
            
            if dist < min_strip_dist:
                min_strip_dist = dist
                closest_pair = current_pair
                
                # 记录新的最小值
                steps.append({
                    'type': 'new_min',
                    'pair': closest_pair,
                    'dist': min_strip_dist,
                    'current_min_pair': closest_pair,
                    'current_min_dist': min_strip_dist,
                    'global_min_pair': global_min_pair,
                    'global_min_dist': global_min_dist
                })
            
            j += 1
    
    return closest_pair, min_strip_dist, steps

def closest_pair_dc_with_steps(points_by_x, points_by_y, steps=None, level=0, current_min_pair=None, current_min_dist=float('inf'), global_min_pair=None, global_min_dist=float('inf')):
    """带可视化步骤的分治法核心递归函数"""
    if steps is None:
        steps = []
    
    n = len(points_by_x)
    
    # 记录当前递归层级和处理的点集
    steps.append({
        'type': 'subproblem',
        'level': level,
        'points': points_by_x,
        'current_min_pair': current_min_pair,
        'current_min_dist': current_min_dist,
        'global_min_pair': global_min_pair,
        'global_min_dist': global_min_dist
    })
    
    # 基本情况：点数≤3时使用蛮力法
    if n <= 3:
        min_dist_local = float('inf')
        closest_pair_local = None
        
        # 手动实现蛮力法，以便更新global_min_pair
        for i in range(n):
            for j in range(i + 1, n):
                current_pair = (points_by_x[i], points_by_x[j])
                dist = Point.distance(points_by_x[i], points_by_x[j])
                
                # 更新全局最小距离（如适用）
                if dist < global_min_dist:
                    global_min_dist = dist
                    global_min_pair = current_pair
                
                # 记录比较步骤
                steps.append({
                    'type': 'compare',
                    'pair': current_pair,
                    'dist': dist,
                    'is_min': dist < min_dist_local,
                    'current_min_pair': closest_pair_local,
                    'current_min_dist': min_dist_local,
                    'global_min_pair': global_min_pair,
                    'global_min_dist': global_min_dist
                })
                
                if dist < min_dist_local:
                    min_dist_local = dist
                    closest_pair_local = current_pair
                    
                    # 记录新的局部最小值
                    steps.append({
                        'type': 'new_min',
                        'pair': closest_pair_local,
                        'dist': min_dist_local,
                        'current_min_pair': closest_pair_local,
                        'current_min_dist': min_dist_local,
                        'global_min_pair': global_min_pair,
                        'global_min_dist': global_min_dist
                    })
        
        # 记录基本情况的结果
        steps.append({
            'type': 'base_case_result',
            'level': level,
            'pair': closest_pair_local,
            'dist': min_dist_local,
            'current_min_pair': closest_pair_local,
            'current_min_dist': min_dist_local,
            'global_min_pair': global_min_pair,
            'global_min_dist': global_min_dist
        })
        
        return closest_pair_local, min_dist_local, steps
    
    # 分割点集
    mid = n // 2
    mid_x = points_by_x[mid].x
    
    # 记录分割线
    steps.append({
        'type': 'divide',
        'level': level,
        'mid_x': mid_x,
        'current_min_pair': current_min_pair,
        'current_min_dist': current_min_dist,
        'global_min_pair': global_min_pair,
        'global_min_dist': global_min_dist
    })
    
    left_x = points_by_x[:mid]
    right_x = points_by_x[mid:]
    
    # 按y坐标分割
    left_y = []
    right_y = []
    for p in points_by_y:
        if p.x < mid_x:
            left_y.append(p)
        else:
            right_y.append(p)
    
    # 递归求解左右子问题
    left_pair, left_min, steps = closest_pair_dc_with_steps(left_x, left_y, steps, level+1, 
                                                           current_min_pair, current_min_dist,
                                                           global_min_pair, global_min_dist)
    
    # 更新当前找到的最小点对
    if left_min < current_min_dist:
        current_min_dist = left_min
        current_min_pair = left_pair
    
    # 更新全局最小点对
    if left_min < global_min_dist:
        global_min_dist = left_min
        global_min_pair = left_pair
    
    # 记录左子问题结果
    steps.append({
        'type': 'left_result',
        'level': level,
        'pair': left_pair,
        'dist': left_min,
        'current_min_pair': current_min_pair,
        'current_min_dist': current_min_dist,
        'global_min_pair': global_min_pair,
        'global_min_dist': global_min_dist
    })
    
    right_pair, right_min, steps = closest_pair_dc_with_steps(right_x, right_y, steps, level+1, 
                                                             current_min_pair, current_min_dist,
                                                             global_min_pair, global_min_dist)
    
    # 更新当前找到的最小点对
    if right_min < current_min_dist:
        current_min_dist = right_min
        current_min_pair = right_pair
    
    # 更新全局最小点对
    if right_min < global_min_dist:
        global_min_dist = right_min
        global_min_pair = right_pair
    
    # 记录右子问题结果
    steps.append({
        'type': 'right_result',
        'level': level,
        'pair': right_pair,
        'dist': right_min,
        'current_min_pair': current_min_pair,
        'current_min_dist': current_min_dist,
        'global_min_pair': global_min_pair,
        'global_min_dist': global_min_dist
    })
    
    # 取左右子问题中的最小值
    if left_min < right_min:
        min_pair = left_pair
        min_dist = left_min
    else:
        min_pair = right_pair
        min_dist = right_min
    
    # 记录子问题合并结果
    steps.append({
        'type': 'merge_result',
        'level': level,
        'pair': min_pair,
        'dist': min_dist,
        'current_min_pair': current_min_pair,
        'current_min_dist': current_min_dist,
        'global_min_pair': global_min_pair,
        'global_min_dist': global_min_dist
    })
    
    # 处理跨越中线的情况
    strip_pair, strip_min, steps = closest_crossing_pair_with_steps(points_by_y, mid_x, min_dist, 
                                                                   current_min_pair, steps,
                                                                   global_min_pair, global_min_dist)
    
    if strip_min < min_dist:
        min_dist = strip_min
        min_pair = strip_pair
        
        # 更新当前找到的最小点对
        if min_dist < current_min_dist:
            current_min_dist = min_dist
            current_min_pair = min_pair
        
        # 更新全局最小点对
        if min_dist < global_min_dist:
            global_min_dist = min_dist
            global_min_pair = min_pair
        
        # 记录跨越中线的新结果
        steps.append({
            'type': 'strip_result',
            'level': level,
            'pair': min_pair,
            'dist': min_dist,
            'current_min_pair': current_min_pair,
            'current_min_dist': current_min_dist,
            'global_min_pair': global_min_pair,
            'global_min_dist': global_min_dist
        })
    
    # 记录当前递归层级的最终结果
    steps.append({
        'type': 'level_result',
        'level': level,
        'pair': min_pair,
        'dist': min_dist,
        'current_min_pair': current_min_pair,
        'current_min_dist': current_min_dist,
        'global_min_pair': global_min_pair,
        'global_min_dist': global_min_dist
    })
    
    return min_pair, min_dist, steps

def divide_and_conquer_with_steps(points):
    """带可视化步骤的分治法入口函数"""
    points_by_x = sorted(points, key=lambda p: p.x)
    points_by_y = sorted(points, key=lambda p: p.y)
    
    return closest_pair_dc_with_steps(points_by_x, points_by_y)

# ------------- 可视化相关函数 -------------

def visualize_step(ax, step, points, final_closest=None, final_min_dist=float('inf'), 
                  show_current_result=True, show_final_result=False, show_global_result=True):
    """可视化单个算法步骤"""
    ax.clear()
    
    # 绘制所有点
    xs = [p.x for p in points]
    ys = [p.y for p in points]
    ax.scatter(xs, ys, c='blue', s=15, alpha=0.6)
    
    step_type = step.get('type', '')
    
    # 根据步骤类型进行不同的可视化
    if step_type == 'compare' or step_type == 'strip_compare':
        # 显示当前比较的点对
        pair = step['pair']
        color = 'green' if not step['is_min'] else 'red'
        ax.plot([pair[0].x, pair[1].x], [pair[0].y, pair[1].y], 
                color=color, linewidth=1.5, alpha=0.7)
        ax.scatter([pair[0].x, pair[1].x], [pair[0].y, pair[1].y], 
                  c=color, s=50)
        
        # 显示距离信息
        mid_x = (pair[0].x + pair[1].x) / 2
        mid_y = (pair[0].y + pair[1].y) / 2
        ax.annotate(f"距离: {step['dist']:.2f}", (mid_x, mid_y), 
                   xytext=(10, 10), textcoords='offset points',
                   bbox=dict(boxstyle='round,pad=0.5', fc='yellow', alpha=0.7))
    
    elif step_type == 'divide':
        # 显示分割线
        ax.axvline(x=step['mid_x'], color='purple', linestyle='--')
        ax.text(step['mid_x'], min(ys) - 50, f"分割线", color='purple')
    
    elif step_type == 'strip':
        # 显示带状区域
        mid_x = step['mid_x']
        width = step['width']
        strip_min_x = mid_x - width
        strip_max_x = mid_x + width
        
        # 绘制带状区域
        y_min, y_max = ax.get_ylim()
        rect = plt.Rectangle((strip_min_x, y_min), 
                             width * 2, y_max - y_min, 
                             color='lightgreen', alpha=0.2)
        ax.add_patch(rect)
        
        # 高亮带状区域内的点
        strip_points = step['points']
        ax.scatter([p.x for p in strip_points], [p.y for p in strip_points], 
                  c='green', s=30, alpha=0.8)
        
        ax.axvline(x=mid_x, color='purple', linestyle='--')
        ax.text(mid_x, min(ys) - 50, f"中线", color='purple')
    
    # 显示全局过程中的最小点对（如果有且启用）- 蓝色
    global_min_pair = step.get('global_min_pair')
    global_min_dist = step.get('global_min_dist', float('inf'))
    
    if global_min_pair and show_global_result:
        ax.plot([global_min_pair[0].x, global_min_pair[1].x], 
               [global_min_pair[0].y, global_min_pair[1].y], 
               'b-', linewidth=2)
        ax.scatter([global_min_pair[0].x, global_min_pair[1].x], 
                  [global_min_pair[0].y, global_min_pair[1].y], 
                  c='blue', s=80, marker='o')
        
        # 显示截至目前最小距离
        mid_x = (global_min_pair[0].x + global_min_pair[1].x) / 2
        mid_y = (global_min_pair[0].y + global_min_pair[1].y) / 2
        ax.annotate(f"截至目前最小距离: {global_min_dist:.2f}", (mid_x, mid_y + 40), 
                   ha='center',
                   bbox=dict(boxstyle='round,pad=0.5', fc='lightblue', alpha=0.7))
    
    # 显示当前步骤的最小点对（如果有且启用）- 红色
    current_min_pair = step.get('current_min_pair')
    current_min_dist = step.get('current_min_dist', float('inf'))
    
    if current_min_pair and show_current_result:
        ax.plot([current_min_pair[0].x, current_min_pair[1].x], 
               [current_min_pair[0].y, current_min_pair[1].y], 
               'r-', linewidth=2)
        ax.scatter([current_min_pair[0].x, current_min_pair[1].x], 
                  [current_min_pair[0].y, current_min_pair[1].y], 
                  c='red', s=80)
        
        # 显示当前最小距离
        mid_x = (current_min_pair[0].x + current_min_pair[1].x) / 2
        mid_y = (current_min_pair[0].y + current_min_pair[1].y) / 2
        ax.annotate(f"当前步骤最小距离: {current_min_dist:.2f}", (mid_x, mid_y + 20), 
                   ha='center',
                   bbox=dict(boxstyle='round,pad=0.5', fc='yellow', alpha=0.7))
    
    # 显示最终的全局最小点对（如果有且启用）- 绿色
    if final_closest and show_final_result:
        ax.plot([final_closest[0].x, final_closest[1].x], 
               [final_closest[0].y, final_closest[1].y], 
               'g--', linewidth=2)
        ax.scatter([final_closest[0].x, final_closest[1].x], 
                  [final_closest[0].y, final_closest[1].y], 
                  c='green', s=80, marker='*')
        
        # 显示全局最小距离
        mid_x = (final_closest[0].x + final_closest[1].x) / 2
        mid_y = (final_closest[0].y + final_closest[1].y) / 2
        ax.annotate(f"全局最小距离: {final_min_dist:.2f}", (mid_x, mid_y - 20), 
                   ha='center',
                   bbox=dict(boxstyle='round,pad=0.5', fc='lightgreen', alpha=0.7))
    
    ax.set_title(f"算法执行过程 - 步骤类型: {step_type}")
    ax.set_xlabel("X 坐标")
    ax.set_ylabel("Y 坐标")
    ax.grid(True, alpha=0.3)
    
    # 调整显示范围，确保所有点都可见
    ax.set_xlim(min(xs) - 50, max(xs) + 50)
    ax.set_ylim(min(ys) - 50, max(ys) + 50)

# 主程序 - 交互式版本
def main():
    # 创建图形和轴
    fig = plt.figure(figsize=(15, 10))
    
    # 创建主绘图区、信息区和步骤可视化区
    ax_main = plt.subplot2grid((2, 2), (0, 0), rowspan=1, colspan=1)
    ax_steps = plt.subplot2grid((2, 2), (0, 1), rowspan=1, colspan=1)
    ax_info = plt.subplot2grid((2, 2), (1, 0), rowspan=1, colspan=1)
    
    # 控件区域
    ax_controls = plt.subplot2grid((2, 2), (1, 1), rowspan=1, colspan=1)
    ax_controls.axis('off')
    
    plt.subplots_adjust(hspace=0.3, wspace=0.3, bottom=0.15)
    
    # 初始点数和算法类型
    initial_n = 100
    max_n = 1000
    current_algo = "分治法"
    current_step_idx = 0
    algorithm_steps = []
    show_current_result = True   # 显示当前步骤的最小点对
    show_global_result = True    # 显示截至当前步骤的全局最小点对
    show_final_result = False    # 默认不显示最终的全局最小点对
    auto_play = False
    play_speed = 0.1  # 默认播放速度（秒/步）
    
    # 保存动画对象的引用，防止被垃圾回收
    animation_ref = None
    
    # 生成初始随机点
    all_points = generate_random_points(max_n)
    points = all_points[:initial_n]
    
    # 计算最近点对
    closest_pair, min_dist = None, float('inf')
    
    # 显示信息的文本对象
    ax_info.axis('off')
    info_text = ax_info.text(0.05, 0.95, "", transform=ax_info.transAxes, 
                            fontsize=9, verticalalignment='top')  # 字体大小减小为9
    
    # 添加滑动条区域 - 点数范围修改为10-1000
    plt.figtext(0.1, 0.02, '点数:', fontsize=12)
    ax_slider = plt.axes([0.18, 0.02, 0.65, 0.03])
    n_slider = Slider(ax_slider, '', 10, max_n, valinit=initial_n, valfmt='%d')
    
    # 添加速度滑动条 - 使用手动对数刻度转换
    plt.figtext(0.1, 0.06, '播放速度:', fontsize=12)
    ax_speed_slider = plt.axes([0.18, 0.06, 0.65, 0.02])
    
    # 对数刻度变换函数
    def log_transform(x):
        return np.log10(x)
    
    def inverse_log_transform(x):
        return 10**x
    
    # 从0.002秒/步(500步/秒)到2秒/步
    min_speed = 0.002
    max_speed = 2.0
    
    # 转换成对数刻度
    log_min_speed = log_transform(min_speed)
    log_max_speed = log_transform(max_speed)
    log_init_speed = log_transform(play_speed)
    
    # 使用简单字符串格式而不是函数
    speed_slider = Slider(
        ax_speed_slider, '', 
        log_min_speed, log_max_speed, 
        valinit=log_init_speed, 
        valfmt='%1.3f'  # 使用简单的字符串格式，后续会手动更新显示
    )
    
    # 更新播放速度 - 修改为使用对数转换并手动更新文本
    def update_speed(val):
        nonlocal play_speed
        play_speed = inverse_log_transform(speed_slider.val)  # 转换回实际速度值
        
        # 手动更新显示的文本
        steps_per_second = 1.0/play_speed if play_speed > 0 else 0
        speed_slider.valtext.set_text(f"{play_speed:.3f}秒/步 ({steps_per_second:.1f}步/秒)")
        
        update_main_view()
        fig.canvas.draw_idle()
    
    # 添加算法选择按钮
    ax_dc_button = plt.axes([0.18, 0.10, 0.3, 0.03])
    ax_bf_button = plt.axes([0.53, 0.10, 0.3, 0.03])
    dc_button = Button(ax_dc_button, '分治法')
    bf_button = Button(ax_bf_button, '蛮力法')
    
    # 添加步骤导航按钮
    ax_prev_button = plt.axes([0.18, 0.14, 0.2, 0.03])
    ax_next_button = plt.axes([0.43, 0.14, 0.2, 0.03])
    ax_auto_button = plt.axes([0.68, 0.14, 0.15, 0.03])
    prev_button = Button(ax_prev_button, '上一步')
    next_button = Button(ax_next_button, '下一步')
    auto_button = Button(ax_auto_button, '自动播放')
    
    # 添加显示/隐藏当前步骤最小点对的复选框
    ax_show_current = plt.axes([0.18, 0.18, 0.3, 0.03])
    show_current_check = CheckButtons(ax_show_current, ['显示当前步骤最小点对'], [show_current_result])
    
    # 添加显示/隐藏全局历史最小点对的复选框
    ax_show_global = plt.axes([0.18, 0.22, 0.3, 0.03])
    show_global_check = CheckButtons(ax_show_global, ['显示截至目前最小点对'], [show_global_result])
    
    # 添加显示/隐藏全局最终结果的复选框
    ax_show_final = plt.axes([0.53, 0.18, 0.3, 0.03])
    show_final_check = CheckButtons(ax_show_final, ['显示全局最小点对'], [show_final_result])
    
    # 更新主显示函数
    def update_main_view():
        ax_main.clear()
        
        # 绘制所有点
        xs = [p.x for p in points]
        ys = [p.y for p in points]
        ax_main.scatter(xs, ys, c='blue', s=15, alpha=0.6)
        
        # 获取当前步骤信息（如果有）
        current_step = algorithm_steps[current_step_idx] if algorithm_steps else None
        current_min_pair = None
        current_min_dist = float('inf')
        global_min_pair = None
        global_min_dist = float('inf')
        
        if current_step:
            current_min_pair = current_step.get('current_min_pair')
            current_min_dist = current_step.get('current_min_dist', float('inf'))
            global_min_pair = current_step.get('global_min_pair')
            global_min_dist = current_step.get('global_min_dist', float('inf'))
        
        # 显示截至目前的全局最小点对（如果有且启用）
        if global_min_pair and show_global_result:
            ax_main.plot([global_min_pair[0].x, global_min_pair[1].x], 
                        [global_min_pair[0].y, global_min_pair[1].y], 
                        'b-', linewidth=2)
            ax_main.scatter([global_min_pair[0].x, global_min_pair[1].x], 
                           [global_min_pair[0].y, global_min_pair[1].y], 
                           c='blue', s=80, marker='o')
            
            # 显示截至目前最小距离
            mid_x = (global_min_pair[0].x + global_min_pair[1].x) / 2
            mid_y = (global_min_pair[0].y + global_min_pair[1].y) / 2
            ax_main.annotate(f"截至目前最小距离: {global_min_dist:.2f}", (mid_x, mid_y + 40), 
                           ha='center',
                           bbox=dict(boxstyle='round,pad=0.5', fc='lightblue', alpha=0.7))
        
        # 如果有当前步骤的最小点对且选择显示，绘制它们
        if current_min_pair and show_current_result:
            ax_main.plot([current_min_pair[0].x, current_min_pair[1].x], 
                        [current_min_pair[0].y, current_min_pair[1].y], 
                        'r-', linewidth=2)
            ax_main.scatter([current_min_pair[0].x, current_min_pair[1].x], 
                           [current_min_pair[0].y, current_min_pair[1].y], 
                           c='red', s=80)
            
            # 显示当前步骤最小距离
            mid_x = (current_min_pair[0].x + current_min_pair[1].x) / 2
            mid_y = (current_min_pair[0].y + current_min_pair[1].y) / 2
            ax_main.annotate(f"当前步骤最小距离: {current_min_dist:.2f}", (mid_x, mid_y + 20), 
                           ha='center',
                           bbox=dict(boxstyle='round,pad=0.5', fc='yellow', alpha=0.7))
        
        # 如果有全局最小点对且选择显示，绘制它们
        if closest_pair and show_final_result:
            ax_main.plot([closest_pair[0].x, closest_pair[1].x], 
                        [closest_pair[0].y, closest_pair[1].y], 
                        'g--', linewidth=2)
            ax_main.scatter([closest_pair[0].x, closest_pair[1].x], 
                           [closest_pair[0].y, closest_pair[1].y], 
                           c='green', s=80, marker='*')
            
            # 显示全局最小距离
            mid_x = (closest_pair[0].x + closest_pair[1].x) / 2
            mid_y = (closest_pair[0].y + closest_pair[1].y) / 2
            ax_main.annotate(f"全局最小距离: {min_dist:.2f}", (mid_x, mid_y - 20), 
                           ha='center',
                           bbox=dict(boxstyle='round,pad=0.5', fc='lightgreen', alpha=0.7))
        
        ax_main.set_title(f"最近点对可视化 - {current_algo} (点数: {len(points)})")
        ax_main.set_xlabel("X 坐标")
        ax_main.set_ylabel("Y 坐标")
        ax_main.grid(True, alpha=0.3)
        
        # 更新信息文本 - 采用更紧凑的布局，使用固定位置展示
        current_result_status = "显示" if show_current_result else "隐藏"
        global_result_status = "显示" if show_global_result else "隐藏"
        final_result_status = "显示" if show_final_result else "隐藏"
        
        # 获取当前步骤信息
        current_step_info = ""
        if current_step:
            current_step_info = f"步骤类型: {current_step.get('type', 'N/A')}"
        
        # 计算步数每秒
        steps_per_second = 1.0/play_speed if play_speed > 0 else 0
        
        # 格式化距离字符串
        if min_dist != float('inf'):
            final_dist_str = f"{min_dist:.4f}"
        else:
            final_dist_str = "N/A"
            
        if current_min_dist != float('inf'):
            current_dist_str = f"{current_min_dist:.4f}"
        else:
            current_dist_str = "N/A"
            
        if global_min_dist != float('inf'):
            global_dist_str = f"{global_min_dist:.4f}"
        else:
            global_dist_str = "N/A"
            
        # 格式化点对字符串
        current_pair_str = "None"
        if current_min_pair:
            p1 = current_min_pair[0]
            p2 = current_min_pair[1]
            current_pair_str = f"({p1.x:.1f},{p1.y:.1f})-({p2.x:.1f},{p2.y:.1f})"
            
        global_pair_str = "None"
        if global_min_pair:
            p1 = global_min_pair[0]
            p2 = global_min_pair[1]
            global_pair_str = f"({p1.x:.1f},{p1.y:.1f})-({p2.x:.1f},{p2.y:.1f})"
            
        final_pair_str = "None"
        if closest_pair:
            p1 = closest_pair[0]
            p2 = closest_pair[1]
            final_pair_str = f"({p1.x:.1f},{p1.y:.1f})-({p2.x:.1f},{p2.y:.1f})"
        
        # 主要算法信息
        algo_info = (f"算法: {current_algo} | 点数: {len(points)} | "
                   f"步骤: {current_step_idx + 1}/{len(algorithm_steps) if algorithm_steps else 0} | "
                   f"{current_step_info}\n"
                   f"速度: {play_speed:.3f}秒/步 ({steps_per_second:.1f}步/秒) | "
                   f"显示: 当前({current_result_status}) 目前({global_result_status}) 最终({final_result_status})\n")
        
        # 点对信息 - 固定宽度展示，去除多余空白
        points_info = "当前步骤(红色)                      截至目前(蓝色)                   全局最终(绿色)\n"
        points_info += f"点对: {current_pair_str:<24} {global_pair_str:<24} {final_pair_str}\n"
        points_info += f"距离: {current_dist_str:<24} {global_dist_str:<24} {final_dist_str}"
        
        # 组合信息文本
        info_str = algo_info + points_info
        info_text.set_text(info_str)
        
        fig.canvas.draw_idle()
    
    # 计算并准备步骤
    def prepare_algorithm():
        nonlocal closest_pair, min_dist, algorithm_steps, current_step_idx, auto_play
        
        # 如果正在自动播放，先停止
        if auto_play:
            auto_play = False
            auto_button.label.set_text('自动播放')
        
        # 计时开始
        start_time = time.time()
        
        # 根据当前选择的算法计算最近点对
        if current_algo == "分治法":
            closest_pair, min_dist, algorithm_steps = divide_and_conquer_with_steps(points)
        else:
            closest_pair, min_dist, algorithm_steps = brute_force_with_steps(points)
        
        # 计时结束
        calc_time = time.time() - start_time
        print(f"计算完成，耗时: {calc_time:.6f}秒，共{len(algorithm_steps)}个步骤")
        
        # 重置步骤索引
        current_step_idx = 0
        
        # 更新主视图
        update_main_view()
        
        # 如果有步骤，显示第一步
        if algorithm_steps:
            visualize_step(ax_steps, algorithm_steps[0], points, 
                          closest_pair, min_dist, show_current_result, 
                          show_final_result, show_global_result)
        
        fig.canvas.draw_idle()
    
    # 更新点数
    def update_n(val):
        nonlocal points, auto_play
        # 如果正在自动播放，先停止
        if auto_play:
            auto_play = False
            auto_button.label.set_text('自动播放')
        
        n = int(n_slider.val)
        points = all_points[:n]
        prepare_algorithm()
    
    # 算法选择回调
    def use_divide_conquer(event):
        nonlocal current_algo
        current_algo = "分治法"
        prepare_algorithm()
    
    def use_brute_force(event):
        nonlocal current_algo
        current_algo = "蛮力法"
        prepare_algorithm()
    
    # 步骤导航回调
    def prev_step(event):
        nonlocal current_step_idx, auto_play
        # 如果正在自动播放，先停止
        if auto_play:
            auto_play = False
            auto_button.label.set_text('自动播放')
        
        if current_step_idx > 0:
            current_step_idx -= 1
            if algorithm_steps:
                visualize_step(ax_steps, algorithm_steps[current_step_idx], points, 
                              closest_pair, min_dist, show_current_result, 
                              show_final_result, show_global_result)
                update_main_view()
                fig.canvas.draw_idle()
    
    def next_step(event):
        nonlocal current_step_idx, auto_play
        # 如果正在自动播放，先停止
        if auto_play:
            auto_play = False
            auto_button.label.set_text('自动播放')
        
        if algorithm_steps and current_step_idx < len(algorithm_steps) - 1:
            current_step_idx += 1
            visualize_step(ax_steps, algorithm_steps[current_step_idx], points, 
                          closest_pair, min_dist, show_current_result, 
                          show_final_result, show_global_result)
            update_main_view()
            fig.canvas.draw_idle()
    
    # 自动播放控制
    def toggle_auto_play(event):
        nonlocal auto_play, animation_ref
        auto_play = not auto_play
        auto_button.label.set_text('停止播放' if auto_play else '自动播放')
        fig.canvas.draw_idle()
        
        if auto_play:
            # 使用FuncAnimation来处理自动播放
            animation_ref = start_auto_play()
    
    def start_auto_play():
        def animation_func(frame):
            nonlocal current_step_idx, auto_play
            if auto_play and algorithm_steps and current_step_idx < len(algorithm_steps) - 1:
                current_step_idx += 1
                visualize_step(ax_steps, algorithm_steps[current_step_idx], points, 
                              closest_pair, min_dist, show_current_result, 
                              show_final_result, show_global_result)
                update_main_view()
            elif current_step_idx >= len(algorithm_steps) - 1 and auto_play:
                # 到达末尾，停止自动播放
                auto_play = False
                auto_button.label.set_text('自动播放')
                anim.event_source.stop()
                
            return ax_steps,
        
        # 创建动画对象
        interval = int(play_speed * 1000)  # 转换为毫秒
        anim = FuncAnimation(fig, animation_func, interval=interval, blit=False, cache_frame_data=False)
        plt.draw()
        return anim
        
    # 显示/隐藏当前步骤最小点对回调
    def toggle_show_current(label):
        nonlocal show_current_result
        show_current_result = not show_current_result
        
        # 更新主视图和步骤视图
        update_main_view()
        if algorithm_steps:
            visualize_step(ax_steps, algorithm_steps[current_step_idx], points, 
                          closest_pair, min_dist, show_current_result, 
                          show_final_result, show_global_result)
        
        fig.canvas.draw_idle()
        
    # 显示/隐藏截至目前全局最小点对回调
    def toggle_show_global(label):
        nonlocal show_global_result
        show_global_result = not show_global_result
        
        # 更新主视图和步骤视图
        update_main_view()
        if algorithm_steps:
            visualize_step(ax_steps, algorithm_steps[current_step_idx], points, 
                          closest_pair, min_dist, show_current_result, 
                          show_final_result, show_global_result)
        
        fig.canvas.draw_idle()
    
    # 显示/隐藏全局最终结果回调
    def toggle_show_final(label):
        nonlocal show_final_result
        show_final_result = not show_final_result
        
        # 更新主视图和步骤视图
        update_main_view()
        if algorithm_steps:
            visualize_step(ax_steps, algorithm_steps[current_step_idx], points, 
                          closest_pair, min_dist, show_current_result, 
                          show_final_result, show_global_result)
        
        fig.canvas.draw_idle()
    
    # 连接事件处理函数
    n_slider.on_changed(update_n)
    speed_slider.on_changed(update_speed)
    dc_button.on_clicked(use_divide_conquer)
    bf_button.on_clicked(use_brute_force)
    prev_button.on_clicked(prev_step)
    next_button.on_clicked(next_step)
    auto_button.on_clicked(toggle_auto_play)
    show_current_check.on_clicked(toggle_show_current)
    show_global_check.on_clicked(toggle_show_global)
    show_final_check.on_clicked(toggle_show_final)
    
    # 初始化
    prepare_algorithm()
    
    plt.show()

if __name__ == "__main__":
    main()