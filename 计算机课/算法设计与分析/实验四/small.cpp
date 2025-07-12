#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <chrono>
#include <random>
#include <iomanip>

using namespace std;

/*
 * 方法 1：递归 + 记忆化（蛮力法）
 */
int eggDropRecursive(int e, int f, vector<vector<int>>& memo) {
    if (f == 0 || f == 1)
        return f;
    if (e == 1)
        return f;
    if (memo[e][f] != -1)
        return memo[e][f];

    int res = INT_MAX;
    for (int k = 1; k <= f; ++k) {
        int broken    = eggDropRecursive(e - 1, k - 1, memo);
        int notBroken = eggDropRecursive(e, f - k, memo);
        int worst     = 1 + max(broken, notBroken);
        res = min(res, worst);
    }
    memo[e][f] = res;
    return res;
}

/*
 * 方法 2：二维 DP 表
 */
int eggDropDP(int e, int f) {
    vector<vector<int>> dp(e + 1, vector<int>(f + 1, 0));
    
    // 边界初始化
    for (int i = 1; i <= e; ++i) {
        dp[i][0] = 0;
        dp[i][1] = 1;
    }
    for (int j = 1; j <= f; ++j) {
        dp[1][j] = j;
    }

    for (int i = 2; i <= e; ++i) {
        for (int j = 2; j <= f; ++j) {
            dp[i][j] = INT_MAX;
            for (int x = 1; x <= j; ++x) {
                int broken    = dp[i - 1][x - 1];
                int notBroken = dp[i][j - x];
                dp[i][j] = min(dp[i][j], 1 + max(broken, notBroken));
            }
        }
    }

    return dp[e][f];
}

// 验证函数
void verifyAlgorithm(int numTests, int maxE, int maxF) {
    // 初始化随机数生成器
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> disE(1, maxE); // 鸡蛋数范围
    uniform_int_distribution<> disF(1, maxF); // 楼层数范围

    int failedTests = 0;
    cout << "开始进行 " << numTests << " 组随机测试...\n";
    cout << "-----------------------------------------\n";
    cout << setw(5) << "测试#" << setw(5) << "鸡蛋e" << setw(5) << "楼层f" 
         << setw(12) << "蛮力法结果" << setw(12) << "算法结果" << setw(10) << "验证状态\n";
    cout << "-----------------------------------------\n";

    for (int i = 1; i <= numTests; ++i) {
        // 生成随机测试数据
        int e = disE(gen);
        int f = disF(gen);
        
        // 方法一：递归+记忆化
        vector<vector<int>> memo(e + 1, vector<int>(f + 1, -1));
        int result1 = eggDropRecursive(e, f, memo);
        
        // 方法二：二维DP
        int result2 = eggDropDP(e, f);
        
        // 验证结果
        bool isValid = (result1 == result2);
        if (!isValid) {
            failedTests++;
        }
        
        // 输出结果
        cout << setw(5) << i << setw(5) << e << setw(5) << f 
             << setw(12) << result1 << setw(12) << result2 
             << setw(10) << (isValid ? "通过" : "失败") << "\n";
    }
    
    cout << "-----------------------------------------\n";
    cout << "测试完成：";
    if (failedTests == 0) {
        cout << "全部通过！算法正确。\n";
    } else {
        cout << "失败 " << failedTests << " 次，共 " << numTests << " 次测试。\n";
        cout << "方法二可能存在错误，请检查算法。\n";
    }
}

int main() {
    cout << "扔鸡蛋算法正确性验证程序\n\n";
    
    // 测试参数
    int numTests = 10;    // 测试次数
    int maxE = 15;        // 最大鸡蛋数
    int maxF = 100;       // 最大楼层数
    
    cout << "将进行 " << numTests << " 组随机测试\n";
    cout << "鸡蛋数范围: 1-" << maxE << "\n";
    cout << "楼层数范围: 1-" << maxF << "\n\n";
    
    // 开始验证
    verifyAlgorithm(numTests, maxE, maxF);
    
    return 0;
}