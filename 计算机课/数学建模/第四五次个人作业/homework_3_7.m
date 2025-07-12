expected_returns = [5; 8; 10];
prices = [20; 25; 30];
std_devs = [2; 6; 10];
r = expected_returns ./ prices;
correlation = [1,      5/24,  -0.5;
               5/24,   1,     -0.25;
              -0.5,   -0.25,   1];
cov_matrix = diag(std_devs) * correlation * diag(std_devs);
Sigma = cov_matrix ./ (prices * prices.');
total_funds = 500000;
H = 2 * Sigma;
f = zeros(3, 1);
A = [-r.'; ones(1,3)];  
b = [-0.20; 1];
lb = zeros(3, 1);
opts = optimoptions('quadprog', 'Display', 'off');
w = quadprog(H, f, A, b, [], [], lb, [], [], opts);
alloc = w * total_funds;
expR = r.' * w * 100;
risk = sqrt(w.' * Sigma * w) * 100;
% 画图
fprintf('(1) 满足20%%收益率的最低风险投资方案：\n');
fprintf('股票A：投资%.0f元（%.2f%%）\n', alloc(1), w(1)*100);
fprintf('股票B：投资%.0f元（%.2f%%）\n', alloc(2), w(2)*100);
fprintf('股票C：投资%.0f元（%.2f%%）\n', alloc(3), w(3)*100);
fprintf('总投资：%.0f元\n', sum(alloc));
fprintf('预期年回报率：%.2f%%\n', expR);
fprintf('风险（标准差）：%.2f%%\n\n', risk);

% (2)
lb = zeros(3, 1);
targets = linspace(0.05, 0.30, 50);
risks = zeros(size(targets));
weights = zeros(3, numel(targets));
for i = 1:numel(targets)
    bi = [-targets(i); 1];
    wi = quadprog(H, f, A, bi, [], [], lb, [], [], opts);
    risks(i) = sqrt(wi.' * Sigma * wi);
    weights(:,i) = wi;
end
%画图
figure(1);
plot(risks*100, targets*100, 'b-o', 'LineWidth', 1.2);
xlabel('风险（收益标准差 %）');
ylabel('期望回报率 %');
title('投资组合高效前沿');
grid on;

figure(2);
area(targets*100, weights'*100);
xlabel('目标回报率（%）');
ylabel('投资比例（%）');
legend('股票A', '股票B', '股票C', 'Location', 'best');
title('不同目标回报率下的最优投资比例');
grid on;
