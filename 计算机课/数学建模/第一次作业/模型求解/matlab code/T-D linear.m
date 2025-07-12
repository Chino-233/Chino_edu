%% 数据导入与预处理
data = readtable('温度盐度ph.csv', 'VariableNamingRule', 'preserve');

% 调整列索引（假设列顺序为：日期, 温度, 盐度, pH, 蜕壳数, 死亡数）
temp = data{:, 2};       % 温度（第二列）
death = data{:, 6};      % 死亡数（第六列）

% 清洗无效数据（新增负值处理）
valid = ~isnan(temp) & ~isnan(death) & (death >= 0);  % 新增非负条件
temp = temp(valid);
death = death(valid);

%% 线性回归建模（基于非负数据）
model_temp = fitlm(temp, death);

%% 生成可调用预测函数（双重保险）
% 获取回归系数
beta0 = model_temp.Coefficients.Estimate(1);  % 截距
beta1 = model_temp.Coefficients.Estimate(2);  % 斜率

% 创建带双重保护的匿名函数（数据和预测双保险）
predict_death = @(x) max(beta1 * x + beta0, 0);  % 确保预测值非负


%% 可视化验证（红色系配色）
figure('Position', [100 100 800 600]);

% 设置颜色（低饱和度红色系）
scatter_color = [255, 182, 193]/255;   % 浅红色散点
line_color = [178, 34, 34]/255;        % 砖红色回归线

% 绘制散点图
scatter(temp, death, 70,...
    'MarkerEdgeColor', scatter_color,...
    'MarkerFaceColor', scatter_color,...
    'MarkerEdgeAlpha', 0.7);
hold on;

% 生成预测曲线
x_range = linspace(min(temp), max(temp), 100);
y_pred = predict_death(x_range);
plot(x_range, y_pred, 'LineWidth', 2.5, 'Color', line_color);

% 图形标注与坐标轴设置
title('Temperature vs Death Count');
xlabel('Temperature (°C)');
ylabel('Death Count');
ylim([0 1]); % 设置y轴范围
grid on;
box on;

% 添加统计信息（右上角）
text(0.95, 0.90,...
    {sprintf('y = %.4fx + %.4f', beta1, beta0),...
    sprintf('R² = %.3f', model_temp.Rsquared.Adjusted)},...
    'Units', 'normalized',...
    'HorizontalAlignment', 'right',...
    'VerticalAlignment', 'top',...
    'BackgroundColor', [1 1 1 0.8],...
    'EdgeColor', [0.5 0.5 0.5],...
    'Margin', 2,...
    'FontSize', 12);
%% 增强图形美观度
% 设置全局字体
set(gcf, 'DefaultAxesFontName', 'Arial', 'DefaultTextFontName', 'Arial');
% 统一坐标轴风格
set(findall(gcf,'Type','axes'),...
    'Box','on',...
    'LineWidth', 1.2,...
    'FontSize', 11,...
    'GridAlpha', 0.2);

%% 格式化输出回归函数
fprintf('\n=== 最终回归函数 ===\n');
fprintf('数学表达式: y = %.4f·x + %.4f\n', beta1, beta0);
fprintf('LaTeX公式: $y = %.4f x + %.4f$\n', beta1, beta0);
fprintf('MATLAB函数: predict_death = @(x) %.4f*x + %.4f\n', beta1, beta0);

%% 输出完整统计结果
fprintf('\n========== 详细回归结果 ==========\n');
disp(model_temp);