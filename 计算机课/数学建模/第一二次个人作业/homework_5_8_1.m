% 导入数据
x = [-2, -1.7, -1.4, -1.1, -0.8, -0.5, -0.2, 0.1, 0.4, 0.7, 1, 1.3, 1.6, 1.9, 2.2, 2.5, 2.8, 3.1, 3.4, 3.7, 4, 4.3, 4.6, 4.9];
y = [0.1029, 0.1174, 0.1316, 0.1448, 0.1566, 0.1662, 0.1733, 0.1775, 0.1785, 0.1764, 0.1711, 0.1630, 0.1526, 0.1402, 0.1266, 0.1122, 0.0977, 0.0835, 0.0702, 0.0588, 0.0479, 0.0373, 0.0291, 0.0224];

% (1) 插值方法的比较
% 线性插值
y_linear = interp1(x, y, x, 'linear');

% 三次样条插值
y_cubic = interp1(x, y, x, 'spline');

% PCHIP插值（保形插值）
y_pchip = interp1(x, y, x, 'pchip');

% Nearest插值
y_nearest = interp1(x, y, x, 'nearest');

% 绘制插值结果
figure('Position', [100, 100, 900, 600]);
subplot(2, 2, 1)
plot(x, y, 'bo', 'MarkerSize', 6, 'DisplayName', '原始数据');
hold on;
plot(x, y_linear, 'r-', 'LineWidth', 1.5, 'DisplayName', '线性插值');
xlabel('x'); ylabel('y');
title('线性插值');
grid on;
legend('Location', 'best');

subplot(2, 2, 2)
plot(x, y, 'bo', 'MarkerSize', 6, 'DisplayName', '原始数据');
hold on;
plot(x, y_cubic, 'g-', 'LineWidth', 1.5, 'DisplayName', '三次样条插值');
xlabel('x'); ylabel('y');
title('三次样条插值');
grid on;
legend('Location', 'best');

subplot(2, 2, 3)
plot(x, y, 'bo', 'MarkerSize', 6, 'DisplayName', '原始数据');
hold on;
plot(x, y_nearest, 'c-', 'LineWidth', 1.5, 'DisplayName', '最近邻插值');
xlabel('x'); ylabel('y');
title('最近邻插值');
grid on;
legend('Location', 'best');

subplot(2, 2, 4)
plot(x, y, 'bo', 'MarkerSize', 6, 'DisplayName', '原始数据');
hold on;
plot(x, y_pchip, 'm-', 'LineWidth', 1.5, 'DisplayName', 'PCHIP插值');
xlabel('x'); ylabel('y');
title('PCHIP插值');
grid on;
legend('Location', 'best');

sgtitle('不同插值方法的比较', 'FontSize', 14);

% 计算每种插值方法在数据点处的误差
test_points = linspace(-1.85, 4.75, 10);
test_points = test_points(~ismember(test_points, x)); % 确保测试点不在原始数据中
true_values = interp1(x, y, test_points, 'spline'); % 使用样条插值作为"真值"参考

methods = {'linear', 'spline', 'pchip', 'nearest'};
method_names = {'线性插值', '三次样条插值', 'PCHIP插值', '最近邻插值'};
rmse_values = zeros(length(methods), 1);

fprintf('不同插值方法的性能比较:\n');
fprintf('方法\t\t均方根误差(RMSE)\n');
fprintf('---------------------------------\n');

for i = 1:length(methods)
    y_interp = interp1(x, y, test_points, methods{i});
    rmse_values(i) = sqrt(mean((y_interp - true_values).^2));
    fprintf('%s\t%.6f\n', method_names{i}, rmse_values(i));
end