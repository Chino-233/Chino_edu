% 5.3 过热蒸汽体积的插值计算
% 已知数据
T = [700, 720, 740, 760, 780];
V = [0.0977, 0.1218, 0.1406, 0.1551, 0.1664];

% 创建更密集的温度点用于绘图
T_dense = linspace(min(T), max(T), 1000);

% 线性插值
V_linear_dense = interp1(T, V, T_dense, 'linear');

% 三次样条插值
V_cubic_dense = interp1(T, V, T_dense, 'spline');

% 计算T=750和T=770时的体积变化
T_interp = [750, 770];
V_linear = interp1(T, V, T_interp, 'linear');
V_cubic = interp1(T, V, T_interp, 'spline');

% 打印结果
fprintf('线性插值结果：\n');
fprintf('T=750时，V=%.4f\n', V_linear(1));
fprintf('T=770时，V=%.4f\n', V_linear(2));
fprintf('三次样条插值结果：\n');
fprintf('T=750时，V=%.4f\n', V_cubic(1));
fprintf('T=770时，V=%.4f\n', V_cubic(2));

% 绘图
figure('Position', [100, 100, 800, 500]);
plot(T, V, 'ko', 'MarkerSize', 8, 'DisplayName', '原始数据点');
hold on;
plot(T_dense, V_linear_dense, 'r-', 'LineWidth', 1.5, 'DisplayName', '线性插值');
plot(T_dense, V_cubic_dense, 'b-', 'LineWidth', 1.5, 'DisplayName', '三次样条插值');
plot(T_interp, V_linear, 'rx', 'MarkerSize', 10, 'DisplayName', '线性插值预测点');
plot(T_interp, V_cubic, 'bx', 'MarkerSize', 10, 'DisplayName', '三次样条插值预测点');
xlabel('温度T (°C)');
ylabel('体积V');
title('过热蒸汽体积随温度变化的插值');
legend('Location', 'northwest');
grid on;