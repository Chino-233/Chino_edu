% 导入数据
x = [-2, -1.7, -1.4, -1.1, -0.8, -0.5, -0.2, 0.1, 0.4, 0.7, 1, 1.3, 1.6, 1.9, 2.2, 2.5, 2.8, 3.1, 3.4, 3.7, 4, 4.3, 4.6, 4.9];
y = [0.1029, 0.1174, 0.1316, 0.1448, 0.1566, 0.1662, 0.1733, 0.1775, 0.1785, 0.1764, 0.1711, 0.1630, 0.1526, 0.1402, 0.1266, 0.1122, 0.0977, 0.0835, 0.0702, 0.0588, 0.0479, 0.0373, 0.0291, 0.0224];

% 创建更密集的x点用于绘图
x_new = linspace(-2, 4.9, 1000);

% (1) 插值方法的比较
% 线性插值
y_linear = interp1(x, y, x_new, 'linear');

% 三次样条插值
y_cubic = interp1(x, y, x_new, 'spline');

% PCHIP插值（保形插值）
y_pchip = interp1(x, y, x_new, 'pchip');

% 绘制插值结果
figure('Position', [100, 100, 900, 600]);
subplot(2, 2, 1)
plot(x, y, 'bo', 'DisplayName', '原始数据');
hold on;
plot(x_new, y_linear, 'r-', 'DisplayName', '线性插值');
title('线性插值');
grid on;
legend('Location', 'best');

subplot(2, 2, 2)
plot(x, y, 'bo', 'DisplayName', '原始数据');
hold on;
plot(x_new, y_cubic, 'g-', 'DisplayName', '三次样条插值');
title('三次样条插值');
grid on;
legend('Location', 'best');

subplot(2, 2, 3)
plot(x, y, 'bo', 'DisplayName', '原始数据');
hold on;
% 注意：标准MATLAB中不提供Akima插值，这里使用样条插值代替
plot(x_new, y_cubic, 'c-', 'DisplayName', '三次插值');
title('三次插值');
grid on;
legend('Location', 'best');

subplot(2, 2, 4)
plot(x, y, 'bo', 'DisplayName', '原始数据');
hold on;
plot(x_new, y_pchip, 'm-', 'DisplayName', 'PCHIP插值');
title('PCHIP插值');
grid on;
legend('Location', 'best');

sgtitle('不同插值方法的比较');

% (2) 最小二乘多项式拟合
% 寻找最佳多项式阶数
max_degree = 10;
results = zeros(max_degree, 3);

for degree = 1:max_degree
    p = polyfit(x, y, degree);
    y_fit = polyval(p, x);
    rmse = sqrt(mean((y - y_fit).^2));
    r2 = 1 - sum((y - y_fit).^2) / sum((y - mean(y)).^2);
    results(degree, :) = [degree, rmse, r2];
end

disp('多项式拟合结果比较：');
disp('阶数    均方根误差    R²');
disp(results);

% 选择合适的多项式阶数
best_degree = 4;  % 假设根据结果选择4阶是合适的

% 进行多项式拟合
poly_coef = polyfit(x, y, best_degree);
fprintf('\n最佳多项式阶数：%d\n', best_degree);
disp('多项式系数（从高阶到低阶）：');
for i = 1:length(poly_coef)
    fprintf('x^%d 系数：%.6f\n', best_degree-(i-1), poly_coef(i));
end

% 计算拟合值和残差标准差
y_poly = polyval(poly_coef, x);
residual_std = sqrt(mean((y - y_poly).^2));
fprintf('残差标准差：%.6f\n', residual_std);

% 绘制多项式拟合结果
figure('Position', [100, 100, 800, 500]);
plot(x, y, 'o', 'DisplayName', '原始数据点');
hold on;
plot(x_new, polyval(poly_coef, x_new), 'r-', 'DisplayName', [num2str(best_degree), '阶多项式拟合']);
xlabel('x');
ylabel('y');
title('多项式拟合结果');
legend('Location', 'best');
grid on;

% (3) 高斯分布非线性拟合
% 定义高斯函数
gaussian = @(b, x) b(1) * exp(-(x - b(2)).^2 / (2 * b(3)^2));

% 初始猜测值：振幅、均值、标准差
initial_guess = [0.18, 0.1, 1.5];

% 进行曲线拟合
options = optimoptions('lsqcurvefit', 'Display', 'off');
[popt, ~, residual] = lsqcurvefit(gaussian, initial_guess, x, y, [], [], options);

% 输出拟合参数
a_fit = popt(1);
mu_fit = popt(2);
sigma_fit = popt(3);
fprintf('\n高斯分布拟合结果：\n');
fprintf('振幅 A = %.6f\n', a_fit);
fprintf('均值 μ = %.6f\n', mu_fit);
fprintf('标准差 σ = %.6f\n', sigma_fit);
fprintf('归一化因子 1/(sqrt(2π)σ) = %.6f\n', 1/(sqrt(2*pi)*sigma_fit));

% 计算拟合优度 R²
y_normal = gaussian(popt, x);
ss_res = sum((y - y_normal).^2);
ss_tot = sum((y - mean(y)).^2);
r_squared = 1 - (ss_res / ss_tot);
fprintf('R² = %.6f\n', r_squared);

% 绘制高斯拟合结果
figure('Position', [100, 100, 800, 500]);
plot(x, y, 'o', 'DisplayName', '原始数据点');
hold on;
plot(x_new, gaussian(popt, x_new), 'g-', 'DisplayName', ['高斯拟合：μ=', num2str(mu_fit, '%.4f'), ', σ=', num2str(sigma_fit, '%.4f')]);
xlabel('x');
ylabel('y');
title('高斯分布拟合结果');
legend('Location', 'best');
grid on;
