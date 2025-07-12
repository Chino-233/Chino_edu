clear; clc;

% 目标函数为最大化从源点流出的总流量(x1 + x2)
f = [-1, -1, zeros(1, 11)]';  % 最小化 -x1 -x2 等价于最大化 x1+x2


% 每条边的流量必须在下界和上界之间
lb = zeros(13, 1);  % 所有边的流量下限为0
ub = [12, 6, 3, 5, 4, 5, 5, 3, 4, 9, 2, 2, 9]';  % 各边的容量上限


% 对于每个节点(除源点和汇点外)，流入量等于流出量
Aeq = [
    % 节点v1
    1, 0, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0;  
    % 节点v2
    0, 1, 1, 0, 0, -1, -1, 0, 0, 0, 0, 0, 0;  
    % 节点v3:
    0, 0, 1, 0, 1, 1, 0, -1, -1, 0, 0, 0, 0;  
    % 节点v4
    0, 0, 0, 1, 0, 0, 0, 1, 0, -1, 0, 0, 0;  
    % 节点v5
    0, 0, 0, 0, 0, 0, 0, 0, 1, 0, -1, -1, 0;  
    % 节点v6
    0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, -1    
];
beq = zeros(6, 1);  % 每个节点的净流量为0


options = optimoptions('linprog', 'Display', 'iter');
[x, fval, exitflag, output] = linprog(f, [], [], Aeq, beq, lb, ub, options);


if exitflag == 1
    disp('最大流问题求解成功:');
    disp(['最大流量 = ', num2str(-fval)]);  % 目标函数值取负即为最大流量
    
    % 显示每条边的流量
    fprintf('\n各边的流量分配:\n');
    for i = 1:length(x)
        fprintf('x%d = %.2f\n', i, x(i));
    end
else
    disp('求解失败:');
    disp(output.message);
end