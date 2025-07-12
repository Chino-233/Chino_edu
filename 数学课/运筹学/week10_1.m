% 定义
items = 1:6;
mass = [21, 16, 18, 12, 9, 32];        % 质量
volume = [4, 2, 3, 2, 1, 4];           % 体积
refrigeration = [1, 0, 1, 0, 0, 1];    % 需要冷藏标记为1
flammabilityIndex = [0.2, 0.2, 0.1, 0.2, 0.1, 0.3]; % 可燃指数
value = [12, 11, 13, 10, 9, 15];       % 价值
f = -value;
% 约束1：总质量限制
A1 = mass;
b1 = 400000;
% 约束2：总体积限制
A2 = volume;
b2 = 55000;
% 约束3：冷藏容积限制
A3 = zeros(1, 6);
for i = 1:6
    if refrigeration(i) == 1
        A3(i) = volume(i);
    end
end
b3 = 8500;
% 约束4：可燃指数限制
A4 = flammabilityIndex;
b4 = 700;
A = [A1; A2; A3; A4];
b = [b1; b2; b3; b4];
lb = zeros(6, 1);
intcon = 1:6;

% 求解
options = optimoptions('intlinprog', 'Display', 'off');
[x, fval, exitflag, output] = intlinprog(f, intcon, A, b, [], [], lb, [], options);

% 输出结果
if exitflag > 0
    fprintf('最大总价值 = %d 元\n\n', -fval);  % 注意负号，因为我们定义的是最小化问题
    fprintf('各物品装载数量：\n');
    for i = 1:6
        fprintf('物品%d: %d 件\n', i, round(x(i)));
    end
end