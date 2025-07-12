c = [6; 8; 4; 2];  % 线性项系数
Q = diag([-1, -1, -1, -1]);  % 主对角线全为-1
for i = 1:3
    Q(i, i+1) = 2;
    Q(i+1, i) = 2;
end
x_values = [-1, 1];
[X1, X2, X3, X4] = ndgrid(x_values, x_values, x_values, x_values);
all_x = [X1(:), X2(:), X3(:), X4(:)];

max_val = -inf;
optimal_x = [];
feasible_solutions = [];
feasible_obj_values = [];
for i = 1:size(all_x, 1)
    x = all_x(i, :)';
    nonlinear_constraint = x(1)*x(2) + x(3)*x(4);
    linear_constraint = sum(x);
    if -1 <= nonlinear_constraint && nonlinear_constraint <= 1 && ...
       -3 <= linear_constraint && linear_constraint <= 2
        obj_val = c'*x + 0.5*x'*Q*x;
        feasible_solutions = [feasible_solutions; x'];
        feasible_obj_values = [feasible_obj_values; obj_val];
        if obj_val > max_val
            max_val = obj_val;
            optimal_x = x;
        end
    end
end

% 显示结果
fprintf('\n最优解：\n');
fprintf('x = [%d, %d, %d, %d]\n', optimal_x(1), optimal_x(2), optimal_x(3), optimal_x(4));
fprintf('最大目标函数值：%.2f\n', max_val);