clear; clc;

n = 12;  % 边的数量

% 目标函数系数：最大化从源点流出的流量（x1 + x2 + x3）
f = zeros(n, 1);
f([1, 2, 3]) = -1;  % 最小化负流量等价于最大化流量

% Aeq * x = beq（5个中间节点）
Aeq = zeros(5, n);
beq = zeros(5, 1);

% v1:
Aeq(1, [1, 4, 5]) = [1, -1, -1];

% v2:
Aeq(2, [2, 6, 7]) = [1, 1, -1];

% v3:
Aeq(3, [3, 5, 6, 8, 9]) = [1, 1, -1, -1, -1];

% v4
Aeq(4, [4, 8, 10, 11]) = [1, 1, -1, -1];

% v5
Aeq(5, [7, 9, 10, 12]) = [1, 1, 1, -1];


lb = zeros(n, 1);  % 流量下限为0
ub = [5; 6; 3; 3; 3; 2; 5; 4; 2; 2; 8; 6];  % 流量上限


options = optimoptions('linprog', 'Display', 'none');
% 修正：移除了多余的空数组参数
[x, fval, exitflag] = linprog(f, [], [], Aeq, beq, lb, ub, options);

if exitflag == 1
    disp('最大流结果：');
    disp(['最大流量 = ', num2str(-fval)]);  % 目标函数值为负值
    
    % 显示每条边的流量
    fprintf('\n各边的流量分配:\n');
    for i = 1:n
        fprintf('x%d = %.2f\n', i, x(i));
    end
else
    disp('求解失败，退出标志：');
    disp(exitflag);
end