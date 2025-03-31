% 求解线性规划问题
% min z=|x1|+2|x2|+3|x3|+4|x4|
% s.t. x1-x2-x3+x4=0
%      x1-x2+x3-3x4=1
%      x1-x2-2x3+3x4=-1/2

% 对于每个变量xi，引入xi_plus和xi_minus，使得xi=xi_plus-xi_minus，|xi|=xi_plus+xi_minus
% 变量顺序：[x1_plus; x1_minus; x2_plus; x2_minus; x3_plus; x3_minus; x4_plus; x4_minus]

% 定义目标函数系数
f = [1; 1; 2; 2; 3; 3; 4; 4];

% 定义等式约束 Aeq*x = beq
Aeq = [1, -1, -1, 1, -1, 1, 1, -1;     % x1-x2-x3+x4=0
       1, -1, -1, 1, 1, -1, -3, 3;     % x1-x2+x3-3x4=1
       1, -1, -1, 1, -2, 2, 3, -3];    % x1-x2-2x3+3x4=-1/2
beq = [0; 1; -1/2];

% 定义变量的下限（所有辅助变量都是非负的）
lb = zeros(8, 1);

% 求解线性规划问题
[x, fval] = linprog(f, [], [], Aeq, beq, lb);

% 计算原始变量值
x1 = x(1) - x(2);
x2 = x(3) - x(4);
x3 = x(5) - x(6);
x4 = x(7) - x(8);

% 输出结果
fprintf('最优解：\n');
fprintf('x1 = %.4f\n', x1);
fprintf('x2 = %.4f\n', x2);
fprintf('x3 = %.4f\n', x3);
fprintf('x4 = %.4f\n', x4);
fprintf('目标函数的最优值 z = %.4f\n', fval);

% 验证约束条件是否满足
fprintf('\n约束条件验证：\n');
fprintf('x1-x2-x3+x4 = %.4f\n', x1-x2-x3+x4);
fprintf('x1-x2+x3-3x4 = %.4f\n', x1-x2+x3-3*x4);
fprintf('x1-x2-2x3+3x4 = %.4f\n', x1-x2-2*x3+3*x4);