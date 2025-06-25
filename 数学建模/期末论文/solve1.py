import pandas as pd
import numpy as np
from datetime import datetime, timedelta
import warnings
from sklearn.preprocessing import MinMaxScaler
from sklearn.metrics import mean_absolute_error, mean_absolute_percentage_error
from statsmodels.tsa.arima.model import ARIMA
from statsmodels.tsa.stattools import adfuller
from statsmodels.tsa.seasonal import seasonal_decompose
import matplotlib.pyplot as plt
import seaborn as sns

# 导入LSTM相关库（如果可用）
try:
    import tensorflow as tf
    from tensorflow.keras.models import Sequential
    from tensorflow.keras.layers import LSTM, Dense, Dropout
    from tensorflow.keras.callbacks import EarlyStopping
    LSTM_AVAILABLE = True
except ImportError:
    print("警告：TensorFlow不可用，将使用多项式回归替代LSTM")
    LSTM_AVAILABLE = False

# 导入指数平滑（备用方案）
try:
    from statsmodels.tsa.holtwinters import ExponentialSmoothing
except ImportError:
    ExponentialSmoothing = None

warnings.filterwarnings('ignore')

# 设置参数
pd.set_option('display.max_columns', None)
pd.set_option('display.max_rows', None)

# ==================== 核心参数配置 ====================
# 业务参数 - 修正发运节点时间
N_DAYS = 7              # 滑动窗口天数：用于传统预测方法的历史数据回望期
ALPHA = 0.5             # 趋势项权重：控制趋势在预测中的影响程度 (0-1)
PREDICT_DATE = '2024/12/16'  # 预测目标日期

# 修正：根据发运节点调整预测时间范围
# 6点发运节点：前一天21:00 - 当天06:00 (9小时) - 在6点前生产完毕
# 14点发运节点：当天11:00 - 当天14:00 (3小时) - 在14点前生产完毕
# 注意：6-11点和14-21点不生产包裹量
START_TIME_0600 = datetime.strptime('2024/12/15 21:00:00', '%Y/%m/%d %H:%M:%S')  # 6点发运开始时间
END_TIME_0600 = datetime.strptime('2024/12/16 06:00:00', '%Y/%m/%d %H:%M:%S')    # 6点发运结束时间
START_TIME_1400 = datetime.strptime('2024/12/16 11:00:00', '%Y/%m/%d %H:%M:%S')  # 14点发运开始时间
END_TIME_1400 = datetime.strptime('2024/12/16 14:00:00', '%Y/%m/%d %H:%M:%S')    # 14点发运结束时间

# ARIMA模型参数
ARIMA_MAX_P = 3         # ARIMA自回归项最大阶数
ARIMA_MAX_D = 2         # ARIMA差分项最大阶数  
ARIMA_MAX_Q = 3         # ARIMA移动平均项最大阶数

# LSTM模型参数
LSTM_LOOKBACK = 10      # LSTM回望窗口：用于预测的历史时间步数
LSTM_EPOCHS = 50        # LSTM训练轮数
LSTM_BATCH_SIZE = 16    # LSTM批处理大小
LSTM_UNITS = 64         # LSTM隐藏单元数
LSTM_DROPOUT = 0.2      # LSTM丢弃率：防止过拟合

# 指数平滑参数（备用方案）
ES_ALPHA = 0.3          # 指数平滑水平参数：控制对新观测值的敏感度
ES_BETA = 0.1           # 指数平滑趋势参数：控制趋势变化的平滑程度
ES_GAMMA = 0.1          # 指数平滑季节参数：控制季节性模式的更新速度

def get_departure_time_info(line_code):
    """
    根据线路编码获取发运节点信息
    
    参数:
    - line_code (str): 线路编码
    
    返回:
    - dict: 发运节点信息
    """
    if '0600' in line_code:
        return {
            'departure_node': '0600',
            'start_time': START_TIME_0600,
            'end_time': END_TIME_0600,
            'active_hours': 9,   # 前一天21:00 - 当天06:00 = 9小时
            'time_slots': 54     # 9小时 * 6个10分钟 = 54个时间段
        }
    elif '1400' in line_code:
        return {
            'departure_node': '1400',
            'start_time': START_TIME_1400,
            'end_time': END_TIME_1400,
            'active_hours': 3,   # 当天11:00 - 当天14:00 = 3小时
            'time_slots': 18     # 3小时 * 6个10分钟 = 18个时间段
        }
    else:
        # 默认使用14点发运
        return {
            'departure_node': '1400',
            'start_time': START_TIME_1400,
            'end_time': END_TIME_1400,
            'active_hours': 3,
            'time_slots': 18
        }

class DataAnalyzer:
    """
    数据分析器：用于分析时间序列数据的基本特征和模式
    """
    
    def __init__(self):
        """初始化数据分析器"""
        self.analysis_results = {}
    
    def analyze_line_data(self, line_code, daily_df, minute_df, show_plots=True):
        """
        分析单条线路的数据特征
        
        参数:
        - line_code (str): 线路编码
        - daily_df (DataFrame): 日货量数据
        - minute_df (DataFrame): 分钟级数据
        - show_plots (bool): 是否显示图表
        
        返回:
        - dict: 分析结果字典
        """
        print(f"\n=== 分析线路: {line_code} ===")
        
        # 提取该线路的数据
        daily_data = daily_df[daily_df['线路编码'] == line_code].sort_values('日期')
        minute_data = minute_df[minute_df['线路编码'] == line_code]
        
        if daily_data.empty:
            print(f"警告：线路 {line_code} 没有日货量数据")
            return None
        
        # 基本统计信息
        analysis = {
            'line_code': line_code,
            'data_points': len(daily_data),
            'date_range': (daily_data['日期'].min(), daily_data['日期'].max()),
            'avg_daily_quantity': daily_data['包裹量'].mean(),
            'std_daily_quantity': daily_data['包裹量'].std(),
            'min_daily_quantity': daily_data['包裹量'].min(),
            'max_daily_quantity': daily_data['包裹量'].max(),
            'trend_coefficient': self._calculate_trend(daily_data),
            'seasonality_strength': self._calculate_seasonality(daily_data),
            'has_minute_data': not minute_data.empty
        }
        
        print(f"数据点数量: {analysis['data_points']}")
        print(f"日期范围: {analysis['date_range'][0]} 到 {analysis['date_range'][1]}")
        print(f"平均日货量: {analysis['avg_daily_quantity']:.2f} ± {analysis['std_daily_quantity']:.2f}")
        print(f"货量范围: [{analysis['min_daily_quantity']}, {analysis['max_daily_quantity']}]")
        print(f"趋势系数: {analysis['trend_coefficient']:.4f}")
        print(f"季节性强度: {analysis['seasonality_strength']:.4f}")
        
        # 如果需要显示图表
        if show_plots and len(daily_data) > 1:
            self._plot_line_analysis(daily_data, minute_data, line_code)
        
        self.analysis_results[line_code] = analysis
        return analysis
    
    def _calculate_trend(self, daily_data):
        """
        计算趋势系数
        
        参数:
        - daily_data (DataFrame): 日货量数据
        
        返回:
        - float: 趋势系数 (正值表示上升趋势，负值表示下降趋势)
        """
        if len(daily_data) < 2:
            return 0
        
        x = np.arange(len(daily_data))
        y = daily_data['包裹量'].values
        
        # 计算线性回归斜率
        slope = np.polyfit(x, y, 1)[0]
        return slope
    
    def _calculate_seasonality(self, daily_data):
        """
        计算季节性强度
        
        参数:
        - daily_data (DataFrame): 日货量数据
        
        返回:
        - float: 季节性强度 (0-1，值越大季节性越强)
        """
        if len(daily_data) < 14:  # 至少需要两周数据
            return 0
        
        try:
            # 使用季节性分解
            ts = daily_data.set_index('日期')['包裹量']
            decomposition = seasonal_decompose(ts, model='additive', period=7, extrapolate_trend='freq')
            
            # 计算季节性变异占总变异的比例
            seasonal_var = np.var(decomposition.seasonal.dropna())
            total_var = np.var(ts)
            
            return seasonal_var / total_var if total_var > 0 else 0
        except:
            return 0
    
    def _plot_line_analysis(self, daily_data, minute_data, line_code):
        """
        绘制线路分析图表
        
        参数:
        - daily_data (DataFrame): 日货量数据
        - minute_data (DataFrame): 分钟级数据
        - line_code (str): 线路编码
        """
        plt.figure(figsize=(15, 10))
        
        # 子图1：日货量时间序列
        plt.subplot(2, 2, 1)
        plt.plot(daily_data['日期'], daily_data['包裹量'], marker='o')
        plt.title(f'{line_code} - 日货量时间序列')
        plt.xlabel('日期')
        plt.ylabel('包裹量')
        plt.xticks(rotation=45)
        plt.grid(True)
        
        # 子图2：日货量分布直方图
        plt.subplot(2, 2, 2)
        plt.hist(daily_data['包裹量'], bins=20, alpha=0.7, edgecolor='black')
        plt.title(f'{line_code} - 日货量分布')
        plt.xlabel('包裹量')
        plt.ylabel('频次')
        plt.grid(True)
        
        # 子图3：如果有分钟数据，显示时间分布
        if not minute_data.empty:
            plt.subplot(2, 2, 3)
            # 计算平均时间分布
            minute_data['hour'] = minute_data['分钟起始'].dt.total_seconds() / 3600
            hourly_avg = minute_data.groupby('hour')['包裹量'].mean()
            plt.plot(hourly_avg.index, hourly_avg.values, marker='o')
            plt.title(f'{line_code} - 平均时间分布')
            plt.xlabel('小时')
            plt.ylabel('平均包裹量')
            plt.grid(True)
        
        # 子图4：周期性分析
        plt.subplot(2, 2, 4)
        if len(daily_data) >= 7:
            daily_data['weekday'] = daily_data['日期'].dt.weekday
            weekday_avg = daily_data.groupby('weekday')['包裹量'].mean()
            weekday_names = ['周一', '周二', '周三', '周四', '周五', '周六', '周日']
            plt.bar(range(7), [weekday_avg.get(i, 0) for i in range(7)])
            plt.title(f'{line_code} - 周内分布')
            plt.xlabel('星期')
            plt.ylabel('平均包裹量')
            plt.xticks(range(7), weekday_names)
            plt.grid(True)
        
        plt.tight_layout()
        plt.show()

class ARIMALSTMPredictor:
    """
    ARIMA+LSTM混合预测器：
    - 使用ARIMA模型捕捉线性趋势和季节性
    - 使用LSTM模型学习ARIMA残差中的非线性模式
    """
    
    def __init__(self, line_code, lookback_window=LSTM_LOOKBACK):
        """
        初始化混合预测器
        
        参数:
        - line_code (str): 线路编码
        - lookback_window (int): LSTM回望窗口大小
        """
        self.line_code = line_code
        self.lookback_window = lookback_window
        
        # 模型组件
        self.arima_model = None      # ARIMA模型
        self.lstm_model = None       # LSTM模型
        self.scaler = MinMaxScaler() # 数据标准化器
        
        # 模型参数
        self.arima_order = (1, 1, 1)  # ARIMA(p,d,q)参数
        self.arima_residuals = None   # ARIMA残差
        
        # 预测结果
        self.arima_forecast = None    # ARIMA预测结果
        self.lstm_correction = None   # LSTM修正值
        
    def find_optimal_arima_order(self, data, max_p=ARIMA_MAX_P, max_d=ARIMA_MAX_D, max_q=ARIMA_MAX_Q):
        """
        自动搜索最优ARIMA参数
        
        参数:
        - data (array): 时间序列数据
        - max_p (int): 最大自回归阶数
        - max_d (int): 最大差分阶数
        - max_q (int): 最大移动平均阶数
        
        返回:
        - tuple: 最优ARIMA(p,d,q)参数
        """
        print(f"正在为线路 {self.line_code} 搜索最优ARIMA参数...")
        
        best_aic = float('inf')
        best_order = (1, 1, 1)
        
        # 网格搜索
        for p in range(max_p + 1):
            for d in range(max_d + 1):
                for q in range(max_q + 1):
                    try:
                        # 拟合ARIMA模型
                        model = ARIMA(data, order=(p, d, q))
                        fitted_model = model.fit()
                        
                        # 选择AIC最小的模型
                        if fitted_model.aic < best_aic:
                            best_aic = fitted_model.aic
                            best_order = (p, d, q)
                    except:
                        continue
        
        print(f"最优ARIMA参数: {best_order}, AIC: {best_aic:.4f}")
        self.arima_order = best_order
        return best_order
    
    def fit_arima(self, data):
        """
        拟合ARIMA模型
        
        参数:
        - data (array): 时间序列数据
        
        返回:
        - array: ARIMA模型残差
        """
        print(f"正在拟合ARIMA模型...")
        
        # 平稳性检验
        adf_result = adfuller(data)
        print(f"ADF检验p值: {adf_result[1]:.4f}")
        
        if adf_result[1] > 0.05:
            print("序列非平稳，正在寻找最优差分阶数...")
            self.find_optimal_arima_order(data)
        
        # 拟合ARIMA模型
        self.arima_model = ARIMA(data, order=self.arima_order).fit()
        
        # 打印模型信息
        print(f"ARIMA{self.arima_order} 模型拟合完成")
        print(f"AIC: {self.arima_model.aic:.4f}")
        print(f"BIC: {self.arima_model.bic:.4f}")
        
        # 计算残差
        self.arima_residuals = data - self.arima_model.fittedvalues
        print(f"ARIMA残差统计: 均值={np.mean(self.arima_residuals):.4f}, 标准差={np.std(self.arima_residuals):.4f}")
        
        return self.arima_residuals
    
    def prepare_lstm_data(self, residuals):
        """
        为LSTM模型准备训练数据
        
        参数:
        - residuals (array): ARIMA残差序列
        
        返回:
        - tuple: (X_train, y_train) 训练数据
        """
        if len(residuals) < self.lookback_window + 1:
            print(f"警告：残差序列长度({len(residuals)})不足以构建LSTM训练数据")
            return None, None
        
        # 数据标准化
        residuals_scaled = self.scaler.fit_transform(residuals.reshape(-1, 1)).flatten()
        
        # 构建滑动窗口数据
        X, y = [], []
        for i in range(self.lookback_window, len(residuals_scaled)):
            X.append(residuals_scaled[i-self.lookback_window:i])
            y.append(residuals_scaled[i])
        
        X = np.array(X)
        y = np.array(y)
        
        print(f"LSTM训练数据形状: X={X.shape}, y={y.shape}")
        return X, y
    
    def build_lstm_model(self, input_shape):
        """
        构建LSTM神经网络模型
        
        参数:
        - input_shape (tuple): 输入数据形状
        
        返回:
        - Sequential: 编译后的LSTM模型
        """
        model = Sequential([
            # 第一层LSTM
            LSTM(LSTM_UNITS, 
                 return_sequences=True, 
                 input_shape=input_shape,
                 name='lstm_layer_1'),
            Dropout(LSTM_DROPOUT, name='dropout_1'),
            
            # 第二层LSTM
            LSTM(LSTM_UNITS // 2, 
                 return_sequences=False,
                 name='lstm_layer_2'),
            Dropout(LSTM_DROPOUT, name='dropout_2'),
            
            # 全连接层
            Dense(32, activation='relu', name='dense_1'),
            Dense(1, name='output_layer')
        ])
        
        # 编译模型
        model.compile(
            optimizer='adam',
            loss='huber',  # 对异常值更鲁棒
            metrics=['mae']
        )
        
        return model
    
    def fit_lstm(self, residuals):
        """
        拟合LSTM模型学习ARIMA残差
        
        参数:
        - residuals (array): ARIMA残差序列
        
        返回:
        - Sequential: 训练好的LSTM模型
        """
        if not LSTM_AVAILABLE:
            print("LSTM不可用，使用多项式回归替代...")
            return self._fit_polynomial_fallback(residuals)
        
        print("正在准备LSTM训练数据...")
        X, y = self.prepare_lstm_data(residuals)
        
        if X is None:
            print("LSTM训练数据准备失败")
            return None
        
        # 重塑数据为LSTM输入格式 [samples, time_steps, features]
        X = X.reshape((X.shape[0], X.shape[1], 1))
        
        # 构建LSTM模型
        print("正在构建LSTM模型...")
        self.lstm_model = self.build_lstm_model((X.shape[1], X.shape[2]))
        
        # 打印模型结构
        print("LSTM模型结构:")
        self.lstm_model.summary()
        
        # 设置早停回调
        early_stopping = EarlyStopping(
            monitor='val_loss',
            patience=10,
            restore_best_weights=True,
            verbose=1
        )
        
        # 训练模型
        print("开始训练LSTM模型...")
        history = self.lstm_model.fit(
            X, y,
            epochs=LSTM_EPOCHS,
            batch_size=LSTM_BATCH_SIZE,
            validation_split=0.2,
            callbacks=[early_stopping],
            verbose=1
        )
        
        # 评估训练结果
        train_loss = history.history['loss'][-1]
        val_loss = history.history['val_loss'][-1]
        print(f"LSTM训练完成 - 训练损失: {train_loss:.6f}, 验证损失: {val_loss:.6f}")
        
        return self.lstm_model
    
    def _fit_polynomial_fallback(self, residuals):
        """
        多项式回归备用方案（当LSTM不可用时）
        
        参数:
        - residuals (array): ARIMA残差序列
        
        返回:
        - Pipeline: 多项式回归模型
        """
        from sklearn.preprocessing import PolynomialFeatures
        from sklearn.linear_model import Ridge
        from sklearn.pipeline import Pipeline
        
        if len(residuals) < 8:
            return None
        
        print("使用多项式回归作为LSTM替代方案...")
        
        # 创建时间特征
        X = np.array(range(len(residuals))).reshape(-1, 1)
        y = residuals.values if hasattr(residuals, 'values') else residuals
        
        # 构建多项式回归管道
        self.polynomial_model = Pipeline([
            ('poly', PolynomialFeatures(degree=2)),
            ('ridge', Ridge(alpha=0.1))
        ])
        
        # 拟合模型
        self.polynomial_model.fit(X, y)
        print("多项式回归模型拟合完成")
        
        return self.polynomial_model
    
    def predict(self, steps=1):
        """
        执行混合预测
        
        参数:
        - steps (int): 预测步数
        
        返回:
        - float: 预测值
        """
        if self.arima_model is None:
            print("错误：ARIMA模型未拟合")
            return None
        
        print(f"开始执行混合预测，预测步数: {steps}")
        
        # 1. ARIMA线性预测
        self.arima_forecast = self.arima_model.forecast(steps=steps)[0]
        print(f"ARIMA预测值: {self.arima_forecast:.4f}")
        
        # 2. LSTM非线性修正
        self.lstm_correction = 0
        
        if LSTM_AVAILABLE and self.lstm_model is not None:
            # 使用LSTM预测残差
            try:
                # 获取最近的残差作为LSTM输入
                recent_residuals = self.arima_residuals[-self.lookback_window:]
                if len(recent_residuals) >= self.lookback_window:
                    # 标准化输入
                    recent_scaled = self.scaler.transform(recent_residuals.reshape(-1, 1))
                    lstm_input = recent_scaled.reshape((1, self.lookback_window, 1))
                    
                    # LSTM预测
                    lstm_pred_scaled = self.lstm_model.predict(lstm_input, verbose=0)
                    self.lstm_correction = self.scaler.inverse_transform(lstm_pred_scaled)[0, 0]
                    
                    print(f"LSTM修正值: {self.lstm_correction:.4f}")
            except Exception as e:
                print(f"LSTM预测失败: {e}")
                self.lstm_correction = 0
        
        elif hasattr(self, 'polynomial_model') and self.polynomial_model is not None:
            # 使用多项式回归预测残差
            try:
                next_time = len(self.arima_residuals)
                X_next = np.array([[next_time]])
                self.lstm_correction = self.polynomial_model.predict(X_next)[0]
                print(f"多项式回归修正值: {self.lstm_correction:.4f}")
            except Exception as e:
                print(f"多项式回归预测失败: {e}")
                self.lstm_correction = 0
        
        # 3. 组合预测结果
        final_prediction = self.arima_forecast + self.lstm_correction
        print(f"最终预测值: {final_prediction:.4f} (ARIMA: {self.arima_forecast:.4f} + 修正: {self.lstm_correction:.4f})")
        
        # 确保预测值非负
        return max(0, final_prediction)

def enhanced_predict_daily_quantity(line_code, df, enable_analysis=False):
    """
    增强的日货量预测函数
    
    参数:
    - line_code (str): 线路编码
    - df (DataFrame): 日货量数据
    - enable_analysis (bool): 是否启用数据分析
    
    返回:
    - float: 预测的日货量
    """
    print(f"\n{'='*60}")
    print(f"开始预测线路: {line_code}")
    print(f"{'='*60}")
    
    # 获取该线路的历史数据
    df_line = df[df['线路编码'] == line_code].sort_values(by='日期')
    
    if len(df_line) < 8:
        print(f"数据不足({len(df_line)}条)，使用简单预测方法")
        return _simple_predict(df_line)
    
    # 可选：数据分析
    if enable_analysis:
        analyzer = DataAnalyzer()
        analyzer.analyze_line_data(line_code, df, pd.DataFrame(), show_plots=False)
    
    # 使用ARIMA+LSTM混合模型
    predictor = ARIMALSTMPredictor(line_code)
    
    try:
        # 提取时间序列数据
        ts_data = df_line['包裹量'].values
        print(f"时间序列长度: {len(ts_data)}")
        
        # 第一步：拟合ARIMA模型
        residuals = predictor.fit_arima(ts_data)
        
        # 第二步：拟合LSTM模型学习残差
        predictor.fit_lstm(residuals)
        
        # 第三步：混合预测
        predicted_qty = predictor.predict(steps=1)
        
        print(f"预测完成！预测货量: {predicted_qty:.2f}")
        return predicted_qty if predicted_qty is not None else 0
        
    except Exception as e:
        print(f"混合模型预测失败: {e}")
        print("使用简单预测方法作为备用")
        return _simple_predict(df_line)

def _simple_predict(df_line):
    """
    简单预测方法（备用方案）
    
    参数:
    - df_line (DataFrame): 单条线路的历史数据
    
    返回:
    - float: 预测货量
    """
    if len(df_line) < 2:
        return None
    
    values = df_line['包裹量'].values
    if len(values) >= N_DAYS:
        # 使用滑动平均 + 趋势预测
        moving_avg = np.mean(values[-N_DAYS:])
        trend = values[-1] - values[0] if len(values) > 1 else 0
        prediction = moving_avg + ALPHA * trend
    else:
        # 使用简单增长预测
        prediction = np.mean(values) * 1.05
    
    return max(0, prediction)

def enhanced_get_time_distribution(line_code, df, use_similar=True):
    """
    修正版时间分布获取函数 - 考虑发运节点的实际生产时间
    
    参数:
    - line_code (str): 线路编码
    - df (DataFrame): 分钟级数据
    - use_similar (bool): 是否使用相似线路数据
    
    返回:
    - array: 144个时间段的分布比例（但只在生产时间段有值）
    """
    print(f"正在分析线路 {line_code} 的时间分布...")
    
    # 获取发运节点信息
    dept_info = get_departure_time_info(line_code)
    print(f"发运节点: {dept_info['departure_node']}, 生产时段: {dept_info['active_hours']}小时")
    
    df_line = df[df['线路编码'] == line_code]
    
    if not df_line.empty:
        # 计算历史时间分布
        grouped = df_line.groupby(['日期'])
        ratio_matrix = []
        
        for date, group in grouped:
            group_sorted = group.sort_values(by='分钟起始')
            total = group_sorted['包裹量'].sum()
            if total > 0:
                ratio = group_sorted['包裹量'].values / total
                if len(ratio) == 144:
                    ratio_matrix.append(ratio)
        
        if ratio_matrix:
            # 计算平均分布
            avg_distribution = np.mean(ratio_matrix, axis=0)
            
            # 修正：只保留生产时间段的分布，其他时间段设为0
            corrected_distribution = apply_production_time_mask(avg_distribution, dept_info)
            
            # 重新归一化
            if corrected_distribution.sum() > 0:
                corrected_distribution = corrected_distribution / corrected_distribution.sum()
                print(f"使用历史数据分布，生产时段货量占比: {corrected_distribution.sum():.3f}")
                return corrected_distribution
    
    # 尝试使用相似线路
    if use_similar:
        origin = line_code.split(' - ')[0] if ' - ' in line_code else line_code.split('-')[0]
        similar_lines = [lc for lc in df['线路编码'].unique() 
                        if lc.startswith(origin.strip()) and dept_info['departure_node'] in lc]
        
        similar_vectors = []
        for similar_line in similar_lines[:3]:  # 减少到3条相似线路
            if similar_line != line_code:
                vector = enhanced_get_time_distribution(similar_line, df, use_similar=False)
                if vector is not None and vector.sum() > 0:
                    similar_vectors.append(vector)
        
        if similar_vectors:
            combined_dist = np.mean(similar_vectors, axis=0)
            print(f"使用相似线路分布 ({len(similar_vectors)}条)")
            return combined_dist
    
    # 生成基于发运节点的默认分布
    default_dist = generate_departure_based_distribution(dept_info)
    print(f"使用默认发运节点分布")
    return default_dist

def apply_production_time_mask(distribution, dept_info):
    """
    应用生产时间掩码，将非生产时间段的分布设为0
    
    参数:
    - distribution (array): 原始144时间段分布
    - dept_info (dict): 发运节点信息
    
    返回:
    - array: 修正后的分布
    """
    corrected = np.zeros(144)
    
    if dept_info['departure_node'] == '0600':
        # 6点发运：前一天21:00 - 当天06:00 (9小时生产时间)
        # 对应时间段：126-144 (前一天21:00-24:00) + 0-36 (当天00:00-06:00)
        
        # 21:00-24:00 对应时间段 126-144 (21*6 = 126)
        corrected[126:144] = distribution[126:144]
        
        # 00:00-06:00 对应时间段 0-36 (6*6 = 36)
        corrected[0:36] = distribution[0:36]
        
        # 注意：6-11点和14-21点(84-126)不生产
        
    elif dept_info['departure_node'] == '1400':
        # 14点发运：当天11:00 - 当天14:00 (3小时生产时间)
        # 对应时间段：66-84 (11*6 = 66, 14*6 = 84)
        corrected[66:84] = distribution[66:84]
        
        # 注意：6-11点(36-66)和14-21点(84-126)不生产
    
    return corrected

def generate_departure_based_distribution(dept_info):
    """
    基于发运节点生成默认时间分布
    
    参数:
    - dept_info (dict): 发运节点信息
    
    返回:
    - array: 144个时间段的默认分布
    """
    distribution = np.zeros(144)
    
    if dept_info['departure_node'] == '0600':
        # 6点发运：前一天21:00 - 当天06:00 生产 (9小时)
        
        # 21:00-24:00 (时间段126-144) - 晚上生产
        evening_slots = slice(126, 144)  # 18个时间段
        for i in range(126, 144):
            # 晚上时段，货量逐渐增加
            hour_progress = (i - 126) / 18  # 0-1之间
            weight = 0.8 + 0.4 * hour_progress  # 0.8-1.2之间
            distribution[i] = weight
        
        # 00:00-06:00 (时间段0-36) - 凌晨生产，临近发运时间货量增加
        morning_slots = slice(0, 36)  # 36个时间段
        for i in range(36):
            # 越接近6点货量越多
            hour = i // 6  # 0-5小时
            weight = 1.0 + (hour / 5.0) * 1.5  # 从1.0递增到2.5
            distribution[i] = weight
        
        # 归一化各部分
        if distribution[evening_slots].sum() > 0:
            distribution[evening_slots] = distribution[evening_slots] / distribution[evening_slots].sum() * 0.3  # 30%在晚上
        if distribution[morning_slots].sum() > 0:
            distribution[morning_slots] = distribution[morning_slots] / distribution[morning_slots].sum() * 0.7  # 70%在凌晨
    
    elif dept_info['departure_node'] == '1400':
        # 14点发运：11:00-14:00 生产 (3小时)
        afternoon_slots = slice(66, 84)  # 18个时间段
        
        # 在14点发运前的3小时内生产，越接近14点货量越多
        for i in range(66, 84):
            hour_in_period = (i - 66) // 6  # 0, 1, 2 对应11-12点, 12-13点, 13-14点
            minute_in_hour = (i - 66) % 6   # 0-5 对应每小时内的10分钟段
            
            # 基础权重随时间递增
            base_weight = 1.0 + (hour_in_period / 2.0) * 2.0  # 1.0 -> 3.0
            # 每小时内也有微调
            minute_weight = 1.0 + (minute_in_hour / 5.0) * 0.2  # 1.0 -> 1.2
            
            distribution[i] = base_weight * minute_weight
        
        # 归一化
        if distribution[afternoon_slots].sum() > 0:
            distribution[afternoon_slots] = distribution[afternoon_slots] / distribution[afternoon_slots].sum()
    
    # 确保分布和为1
    if distribution.sum() > 0:
        distribution = distribution / distribution.sum()
    else:
        # 如果出现问题，使用均匀分布
        print(f"警告：生成分布失败，使用备用均匀分布")
        if dept_info['departure_node'] == '0600':
            # 6点发运：9小时均匀分布
            distribution[126:144] = 1.0 / 54  # 18个时间段
            distribution[0:36] = 1.0 / 54     # 36个时间段
        else:
            # 14点发运：3小时均匀分布
            distribution[66:84] = 1.0 / 18    # 18个时间段
        distribution = distribution / distribution.sum()
    
    return distribution

def generate_minute_predictions(line_code, predicted_qty, minute_df):
    """
    生成分钟级预测结果
    
    参数:
    - line_code (str): 线路编码
    - predicted_qty (float): 预测的日总货量
    - minute_df (DataFrame): 历史分钟级数据
    
    返回:
    - list: 分钟级预测结果列表
    """
    print(f"\n生成线路 {line_code} 的分钟级预测...")
    
    # 获取发运节点信息
    dept_info = get_departure_time_info(line_code)
    
    # 获取时间分布
    r_vector = enhanced_get_time_distribution(line_code, minute_df)
    quantities_10min = predicted_qty * r_vector
    
    # 生成预测结果
    results = []
    
    # 根据发运节点确定时间范围
    if dept_info['departure_node'] == '0600':
        # 6点发运：从前一天21:00开始到当天06:00
        start_time = dept_info['start_time']
        current_time = start_time
        
        # 前一天21:00到24:00
        while current_time.date() < datetime.strptime(PREDICT_DATE, '%Y/%m/%d').date():
            time_slot_idx = (current_time.hour * 6) + (current_time.minute // 10)
            if time_slot_idx < 144:
                predicted_quantity = round(max(0, quantities_10min[time_slot_idx]))
                # 只有预测值大于0才添加到结果中
                if predicted_quantity > 0:
                    results.append({
                        '线路编码': line_code,
                        '日期': current_time.strftime('%Y/%m/%d'),
                        '分钟起始': current_time.strftime('%H:%M:%S'),
                        '包裹量': predicted_quantity
                    })
            current_time += timedelta(minutes=10)
            if current_time >= datetime.strptime(PREDICT_DATE + ' 00:00:00', '%Y/%m/%d %H:%M:%S'):
                break
        
        # 当天00:00到06:00
        current_time = datetime.strptime(PREDICT_DATE + ' 00:00:00', '%Y/%m/%d %H:%M:%S')
        while current_time <= dept_info['end_time']:
            time_slot_idx = (current_time.hour * 6) + (current_time.minute // 10)
            if time_slot_idx < 144:
                predicted_quantity = round(max(0, quantities_10min[time_slot_idx]))
                # 只有预测值大于0才添加到结果中
                if predicted_quantity > 0:
                    results.append({
                        '线路编码': line_code,
                        '日期': current_time.strftime('%Y/%m/%d'),
                        '分钟起始': current_time.strftime('%H:%M:%S'),
                        '包裹量': predicted_quantity
                    })
            current_time += timedelta(minutes=10)
    
    else:
        # 14点发运：当天11:00到14:00
        current_time = dept_info['start_time']
        while current_time <= dept_info['end_time']:
            time_slot_idx = (current_time.hour * 6) + (current_time.minute // 10)
            if time_slot_idx < 144:
                predicted_quantity = round(max(0, quantities_10min[time_slot_idx]))
                # 只有预测值大于0才添加到结果中
                if predicted_quantity > 0:
                    results.append({
                        '线路编码': line_code,
                        '日期': current_time.strftime('%Y/%m/%d'),
                        '分钟起始': current_time.strftime('%H:%M:%S'),
                        '包裹量': predicted_quantity
                    })
            current_time += timedelta(minutes=10)
    
    print(f"生成了 {len(results)} 条分钟级预测记录（已过滤0值）")
    
    # 打印时间分布验证
    if results:
        print(f"时间范围: {results[0]['分钟起始']} - {results[-1]['分钟起始']}")
        total_predicted = sum([r['包裹量'] for r in results])
        print(f"分钟级总和: {total_predicted}, 日预测: {predicted_qty:.0f}")
        
        # 检查是否有货量损失（由于过滤0值导致的)
        all_predicted = sum(quantities_10min)
        if abs(total_predicted - all_predicted) > 1:
            print(f"⚠️ 注意：过滤0值后货量减少了 {all_predicted - total_predicted:.0f}")
    else:
        print("⚠️ 警告：所有预测值都为0，无有效记录生成")
    
    return results

# ==================== 主程序 ====================
if __name__ == "__main__":
    # 读取数据
    print("正在读取数据文件...")
    daily_df = pd.read_excel('D:\\Chino_edu\\数学建模\\期末论文\\rawData\\附件3.xlsx')
    minute_df = pd.read_excel('D:\\Chino_edu\\数学建模\\期末论文\\rawData\\附件2.xlsx')
    result_template_1 = pd.read_excel('D:\\Chino_edu\\数学建模\\期末论文\\rawData\\结果表1.xlsx')
    result_template_2 = pd.read_excel('D:\\Chino_edu\\数学建模\\期末论文\\rawData\\结果表2.xlsx')
    
    # 数据预处理
    print("正在进行数据预处理...")
    daily_df['日期'] = pd.to_datetime(daily_df['日期'])
    minute_df['日期'] = pd.to_datetime(minute_df['日期'])
    minute_df['分钟起始'] = pd.to_timedelta(minute_df['分钟起始'].astype(str))
    
    print(f"数据加载完成:")
    print(f"- 日货量数据: {len(daily_df)} 条记录")
    print(f"- 分钟级数据: {len(minute_df)} 条记录")
    print(f"- 需要预测的线路数: {len(result_template_1['线路编码'].unique())}")
    
    # 可选：数据分析
    ENABLE_DATA_ANALYSIS = True  # 设置为True启用数据分析
    if ENABLE_DATA_ANALYSIS:
        print("\n" + "="*60)
        print("数据分析阶段")
        print("="*60)
        
        analyzer = DataAnalyzer()
        
        # 分析几条典型线路
        sample_lines = result_template_1['线路编码'].unique()[:3]
        for line_code in sample_lines:
            analyzer.analyze_line_data(line_code, daily_df, minute_df, show_plots=False)
    
    # 主预测逻辑
    print("\n" + "="*60)
    print("开始执行ARIMA+LSTM混合预测...")
    print("="*60)
    
    results_1 = []
    results_2 = []
    
    target_lines = result_template_1['线路编码'].unique()
    total_lines = len(target_lines)
    
    for idx, line_code in enumerate(target_lines):
        print(f"\n进度: {idx+1}/{total_lines}")
        #if (idx+1) == 5:  # 增加测试线路数量到5条
        #    break

        # 日货量预测
        predicted_qty = enhanced_predict_daily_quantity(line_code, daily_df, enable_analysis=False)
        
        if predicted_qty is None or predicted_qty <= 0:
            print(f"跳过线路 {line_code}: 预测失败或货量为0")
            continue
        
        print(f"✓ 预测日货量: {predicted_qty:.2f}")
        
        # 保存日预测结果
        results_1.append({
            '线路编码': line_code,
            '日期': PREDICT_DATE,
            '货量': round(predicted_qty)
        })
        
        # 使用新的分钟级预测生成方法
        minute_results = generate_minute_predictions(line_code, predicted_qty, minute_df)
        results_2.extend(minute_results)
    
    # 保存结果
    print(f"\n{'='*60}")
    print("保存预测结果...")
    print(f"{'='*60}")
    
    # 确保输出目录存在
    import os
    output_dir = 'D:\\Chino_edu\\数学建模\\期末论文\\result'
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)
    
    result_df1 = pd.DataFrame(results_1)
    result_df2 = pd.DataFrame(results_2)
    
    try:
        result_df1.to_excel(os.path.join(output_dir, '结果表1_ARIMA_LSTM预测_修正版.xlsx'), index=False)
        result_df2.to_excel(os.path.join(output_dir, '结果表2_ARIMA_LSTM分钟预测_修正版.xlsx'), index=False)
        
        print("✓ 结果保存成功!")
        print(f"  📊 成功预测 {len(results_1)} 条线路")
        print(f"  🕐 生成 {len(results_2)} 条分钟级预测记录（已过滤0值）")
        
        # 验证修正效果
        if len(results_2) > 0:
            total_minute_qty = sum([r['包裹量'] for r in results_2])
            total_daily_qty = sum([r['货量'] for r in results_1])
            print(f"  📈 分钟级总和: {total_minute_qty}")
            print(f"  📊 日级总和: {total_daily_qty}")
            
            # 由于过滤了0值，分钟级总和可能小于日级总和
            difference = total_daily_qty - total_minute_qty
            if difference > 0:
                print(f"  📉 过滤0值导致的差异: {difference}")
                print(f"  ✅ 数据有效性: 分钟级记录均为正值")
            else:
                print(f"  ✅ 数据一致性: 完全匹配")
            
    except Exception as e:
        print(f"✗ 保存失败: {e}")
    
    # 输出修正后的预测结果统计
    print("\n" + "="*60)
    print("修正后预测结果展示")
    print("="*60)
    
    # 显示不同发运节点的统计
    if len(results_1) > 0:
        dept_0600_lines = [r for r in results_1 if '0600' in r['线路编码']]
        dept_1400_lines = [r for r in results_1 if '1400' in r['线路编码']]
        
        print(f"\n📊 发运节点统计:")
        print(f"  🌅 6点发运线路: {len(dept_0600_lines)} 条")
        if dept_0600_lines:
            avg_0600 = np.mean([r['货量'] for r in dept_0600_lines])
            print(f"     平均货量: {avg_0600:.1f} 个包裹")
        
        print(f"  🌇 14点发运线路: {len(dept_1400_lines)} 条")
        if dept_1400_lines:
            avg_1400 = np.mean([r['货量'] for r in dept_1400_lines])
            print(f"     平均货量: {avg_1400:.1f} 个包裹")
    
    print(f"\n{'='*60}")
    print(f"🎉 修正版ARIMA+LSTM预测系统运行完成！")
    print(f"📁 已考虑发运节点的实际生产时间")
    print(f"🔧 修正了时间分布，提高预测准确性")
    print(f"🚫 已过滤预测值为0的时间段")
    print(f"{'='*60}")
    
    # 在预测循环后添加详细验证
    print("\n" + "="*60)
    print("预测结果详细验证")
    print("="*60)
    
    if len(results_2) > 0:
        result_df2_temp = pd.DataFrame(results_2)
        
        # 按发运节点分组验证
        dept_0600_minutes = result_df2_temp[result_df2_temp['线路编码'].str.contains('0600')]
        dept_1400_minutes = result_df2_temp[result_df2_temp['线路编码'].str.contains('1400')]
        
        print(f"\n📊 6点发运节点验证:")
        if not dept_0600_minutes.empty:
            print(f"  记录数: {len(dept_0600_minutes)} （仅包含>0的预测值）")
            time_range_0600 = dept_0600_minutes['分钟起始'].unique()
            print(f"  时间范围: {min(time_range_0600)} - {max(time_range_0600)}")
            print(f"  应该覆盖: 前一天21:00-24:00 和 当天00:00-06:00")
            print(f"  货量范围: {dept_0600_minutes['包裹量'].min()} - {dept_0600_minutes['包裹量'].max()}")
        else:
            print(f"  无有效记录（所有预测值为0）")
        
        print(f"\n📊 14点发运节点验证:")
        if not dept_1400_minutes.empty:
            print(f"  记录数: {len(dept_1400_minutes)} （仅包含>0的预测值）")
            time_range_1400 = dept_1400_minutes['分钟起始'].unique()
            print(f"  时间范围: {min(time_range_1400)} - {max(time_range_1400)}")
            print(f"  应该覆盖: 当天11:00-14:00")
            print(f"  货量范围: {dept_1400_minutes['包裹量'].min()} - {dept_1400_minutes['包裹量'].max()}")
        else:
            print(f"  无有效记录（所有预测值为0）")
    
    print(f"\n{'='*60}")
    print(f"🎉 修正版ARIMA+LSTM预测系统运行完成！")
    print(f"📁 已考虑发运节点的实际生产时间")
    print(f"🔧 修正了时间分布，提高预测准确性")
    print(f"🚫 已过滤预测值为0的时间段")
    print(f"{'='*60}")
