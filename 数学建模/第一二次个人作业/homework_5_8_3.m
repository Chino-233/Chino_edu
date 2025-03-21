clc;
x = [-2, -1.7, -1.4, -1.1, -0.8, -0.5, -0.2, 0.1, 0.4, 0.7, 1, 1.3, 1.6, 1.9, 2.2, 2.5, 2.8, 3.1, 3.4, 3.7, 4, 4.3, 4.6, 4.9];
y = [0.1029, 0.1174, 0.1316, 0.1448, 0.1566, 0.1662, 0.1733, 0.1775, 0.1785, 0.1764, 0.1711, 0.1630, 0.1526, 0.1402, 0.1266, 0.1122, 0.0977, 0.0835, 0.0702, 0.0588, 0.0479, 0.0373, 0.0291, 0.0224];

% 定义题目给的分布函数
gaussian = @(b, x) b(1) * exp(-(x - b(2)).^2 / (2 * b(3)^2));

[max_y, max_idx] = max(y);
initial_guess = [max_y, x(max_idx), (max(x) - min(x))/4];

% 进行非线性最小二乘拟合
options = optimoptions('lsqcurvefit', 'Display', 'off');
[params, ~, residual, ~, ~, ~, jacobian] = lsqcurvefit(gaussian, initial_guess, x, y, [], [], options);

% 提取拟合参数
amplitude = params(1);
mean_value = params(2);
std_dev = params(3);

% 95%置信区间
residual_var = sum(residual.^2)/(length(x) - length(params));
covariance = residual_var * inv(jacobian' * jacobian);
ci = nlparci(params, residual, 'jacobian', jacobian);

% 拟合优度
y_gaussian = gaussian(params, x);
ss_res = sum((y - y_gaussian).^2);
ss_tot = sum((y - mean(y)).^2);
r_squared = 1 - (ss_res / ss_tot);

% 显示拟合结果
fprintf('高斯分布拟合结果:\n');
fprintf('振幅 A = %.6f (95%% CI: [%.6f, %.6f])\n', amplitude, ci(1,1), ci(1,2));
fprintf('均值 μ = %.6f (95%% CI: [%.6f, %.6f])\n', mean_value, ci(2,1), ci(2,2));
fprintf('标准差 σ = %.6f (95%% CI: [%.6f, %.6f])\n', std_dev, ci(3,1), ci(3,2));
fprintf('归一化因子 1/(σ√(2π)) = %.6f\n', 1/(std_dev*sqrt(2*pi)));
fprintf('拟合优度 R² = %.6f\n', r_squared);

% 计算均方根误差
rmse = sqrt(mean((y - y_gaussian).^2));
fprintf('均方根误差 RMSE = %.6f\n', rmse);

% 绘制拟合结果
figure('Position', [100, 100, 900, 600]);
subplot(2, 1, 1);
plot(x, y, 'bo', 'MarkerSize', 6, 'DisplayName', '原始数据点');
hold on;
plot(x, y_gaussian, 'r-', 'LineWidth', 1.5, 'DisplayName', '高斯分布拟合');
% 将复杂公式简化为更容易解析的形式
text(0.05, 0.82, ['R² = ', num2str(r_squared, '%.6f')], 'Units', 'normalized');
text(0.05, 0.74, ['RMSE = ', num2str(rmse, '%.6f')], 'Units', 'normalized');
xlabel('x');
ylabel('y');
title('高斯分布拟合结果', 'FontSize', 12);
grid on;
legend('Location', 'best');

% 绘制残差图
subplot(2, 1, 2);
residuals = y - y_gaussian;
stem(x, residuals, 'filled', 'MarkerSize', 4);
xlabel('x');
ylabel('残差 (y - y_{fit})');
title('拟合残差', 'FontSize', 12);
grid on;
hold on;
plot([min(x), max(x)], [0, 0], 'k--', 'LineWidth', 1);
residual_std = std(residuals);
confidence_interval = 1.96 * residual_std;
plot([min(x), max(x)], [confidence_interval, confidence_interval], 'r--', 'LineWidth', 1);
plot([min(x), max(x)], [-confidence_interval, -confidence_interval], 'r--', 'LineWidth', 1);
text(0.05, 0.9, ['95% 置信区间: \pm', num2str(confidence_interval, '%.4f')], ...
    'Units', 'normalized', 'Color', 'r');
