
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <bits/stdc++.h>
#include <chrono>

using namespace std;

/*
 * 方法 1：递归 + 记忆化（仅供小规模验证）
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
 * 方法 2：二维 DP 表（O(e * f^2)）
 */
int eggDropDP(int e, int f, long long& elapsedMs, long long timeLimitMs) {
    auto start = chrono::high_resolution_clock::now();

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
            auto now = chrono::high_resolution_clock::now();
            elapsedMs = chrono::duration_cast<chrono::milliseconds>(now - start).count();
            if (elapsedMs > timeLimitMs) {
                return -1;  // 标记为超时
            }
            dp[i][j] = INT_MAX;
            for (int x = 1; x <= j; ++x) {
                int broken    = dp[i - 1][x - 1];
                int notBroken = dp[i][j - x];
                dp[i][j] = min(dp[i][j], 1 + max(broken, notBroken));
            }
        }
    }

    auto end = chrono::high_resolution_clock::now();
    elapsedMs = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    return dp[e][f];
}

/*
 * 方法 3：优化版 DP（逆向思维，二维数组 dp[m][i]）
 */
int eggDropOptimized(int e, int f) {
    int m0 = static_cast<int>(ceil(log2(f + 1.0)));
    vector<vector<long long>> dp(m0 + 10, vector<long long>(e + 1, 0));
    int m = 0;
    
    while (dp[m][e] < f) {
        ++m;
        for (int i = 1; i <= e; ++i) {
            dp[m][i] = dp[m - 1][i - 1] + dp[m - 1][i] + 1;
        }
    }
    return m;
}

/*
 * 方法 4：（一维数组 dp[i]）与鸡蛋数限制
 */
int eggDropComb(int e, int f) {
    int m0 = static_cast<int>(ceil(log2(f + 1.0)));
    if (e >= m0) {
       return m0;
    }
    vector<long long> dp(e + 1, 0LL);
    int m = 0;
    while (dp[e] < f) {
        ++m;
        for (int i = e; i >= 1; --i) {
            dp[i] = dp[i] + dp[i - 1] + 1;
        }
    }
    return m;
}


const long long TIME_LIMIT_MS = 1000000; // 每次测试限时
using Clock = chrono::high_resolution_clock;

// 方法1 测试
void testMethod1() {
    cout << "=== 方法1：递归+记忆化 测试 ===\n";
    // 测试 A：固定 f=10，变化 e={1,2,3,4,5}
    int f_fixed1 = 1000;
    cout << "测试 A (固定 f=10, e变化):\n";
    for (int e = 10; e <= 50; ++e) {
        vector<vector<int>> memo(e + 1, vector<int>(f_fixed1 + 1, -1));
        auto t_start = Clock::now();
        int result = eggDropRecursive(e, f_fixed1, memo);
        auto t_end = Clock::now();
        long long elapsed = chrono::duration_cast<chrono::milliseconds>(t_end - t_start).count();
        cout << "  e=" << e << ", f=" << f_fixed1 
             << "：结果 = " << result 
             << "，耗时 " << elapsed << " ms\n";
    }
    // 测试 B：固定 e=2，变化 f={10,20,30,40,50}
    int e_fixed1 = 20;
    cout << "测试 B (固定 e=20, f变化):\n";
    for (int f = 100; f <= 500; f += 100) {
        vector<vector<int>> memo(e_fixed1 + 1, vector<int>(f + 1, -1));
        auto t_start = Clock::now();
        int result = eggDropRecursive(e_fixed1, f, memo);
        auto t_end = Clock::now();
        long long elapsed = chrono::duration_cast<chrono::milliseconds>(t_end - t_start).count();
        cout << "  e=" << e_fixed1 << ", f=" << f 
             << "：结果 = " << result 
             << "，耗时 " << elapsed << " ms\n";
    }
    cout << endl;
}

// 方法2 测试
void testMethod2() {
    cout << "=== 方法2：二维DP 测试 ===\n";
    // 测试 A：固定 f=100，变化 e={1,2,3,4,5}
    int f_fixed2 = 1000;
    cout << "测试 A (固定 f=100, e变化):\n";
    for (int e = 500; e <= 5000; e+=500) {
        long long elapsed = 0;
        int result = eggDropDP(e, f_fixed2, elapsed, TIME_LIMIT_MS);
        if (result == -1) {
            cout << "  e=" << e << ", f=" << f_fixed2 
                 << "：超时（> " << TIME_LIMIT_MS << " ms）\n";
        } else {
            cout << "  e=" << e << ", f=" << f_fixed2 
                 << "：结果 = " << result 
                 << "，耗时 " << elapsed << " ms\n";
        }
    }
    // 测试 B：固定 e=2，变化 f={100,200,300,400,500}
    int e_fixed2 = 2;
    cout << "测试 B (固定 e=2, f变化):\n";
    for (int f = 1000; f <= 10000; f += 1000) {
        long long elapsed = 0;
        int result = eggDropDP(e_fixed2, f, elapsed, TIME_LIMIT_MS);
        if (result == -1) {
            cout << "  e=" << e_fixed2 << ", f=" << f 
                 << "：超时（> " << TIME_LIMIT_MS << " ms）\n";
        } else {
            cout << "  e=" << e_fixed2 << ", f=" << f 
                 << "：结果 = " << result 
                 << "，耗时 " << elapsed << " ms\n";
        }
    }
    cout << endl;
}

// 方法2特定数据大小测试
void testMethod2Specific() {
    cout << "=== 方法2：二维DP 特定数据测试 ===\n";
    
    // 定义特定的测试用例 (e, f)
    vector<pair<int, int>> testCases = {
        /*
        {20, 100}, 
        {20, 250},
        {20, 500},
        {20, 750},  
        {20, 1000},
        {20, 2500},
        {20, 5000},
        {20, 7500},   
        {20, 10000},
        {20, 25000},
        {20, 50000},
        {20, 75000},
        {20, 100000},
        */
        {100,1000},
        {250,1000},
        {750,1000},

    };
    
    for (const auto& testCase : testCases) {
        int e = testCase.first;
        int f = testCase.second;
        
        long long elapsed = 0;
        int result = eggDropDP(e, f, elapsed, TIME_LIMIT_MS);
        
        cout << "  e=" << e << ", f=" << f << ": ";
        if (result == -1) {
            cout << "超时（> " << TIME_LIMIT_MS << " ms）\n";
        } else {
            cout << "结果 = " << result << "，耗时 " << elapsed << " ms\n";
        }
    }
    cout << endl;
}

// 方法3 测试
void testMethod3() {
    cout << "=== 方法3：优化版DP 测试 ===\n";
    // 测试 A：固定 f=10000，变化 e={5,10,15,20,25}
    int f_fixed3 = 10000;
    cout << "测试 A (固定 f=10000, e变化):\n";
    for (int e = 5; e <= 25; e += 5) {
        auto t_start = Clock::now();
        int result = eggDropOptimized(e, f_fixed3);
        auto t_end = Clock::now();
        long long elapsed = chrono::duration_cast<chrono::milliseconds>(t_end - t_start).count();
        cout << "  e=" << e << ", f=" << f_fixed3 
             << "：结果 = " << result 
             << "，耗时 " << elapsed << " ms\n";
    }
    // 测试 B：固定 e=10，变化 f={1000,2000,3000,4000,5000}
    int e_fixed3 = 10;
    cout << "测试 B (固定 e=10, f变化):\n";
    for (int f = 1000; f <= 5000; f += 1000) {
        auto t_start = Clock::now();
        int result = eggDropOptimized(e_fixed3, f);
        auto t_end = Clock::now();
        long long elapsed = chrono::duration_cast<chrono::milliseconds>(t_end - t_start).count();
        cout << "  e=" << e_fixed3 << ", f=" << f 
             << "：结果 = " << result 
             << "，耗时 " << elapsed << " ms\n";
    }
    cout << endl;
}

// 方法4 测试
void testMethod4() {
    cout << "=== 方法4：组合数累加 测试 ===\n";
    // 测试 A：固定 f=500000，变化 e={50,100,200,400,800}
    int f_fixed4A = 2000000000;
    cout << "测试 A (固定 f=500000, e变化):\n";
    for (long long e = 10; e <= 10000000000; e *= 10) {
        auto t_start = Clock::now();
        int result = eggDropComb(e, f_fixed4A);
        auto t_end = Clock::now();
        long long elapsed = chrono::duration_cast<chrono::milliseconds>(t_end - t_start).count();
        cout << "  e=" << e << ", f=" << f_fixed4A 
             << "：结果 = " << result 
             << "，耗时 " << elapsed << " ms\n";
    }
    // 测试 B：固定 e=200，变化 f={100000, 300000, 500000, 700000, 900000}
    int e_fixed4B = 1000;
    cout << "测试 B (固定 e=200, f变化):\n";
    for (long long f = 10000; f <= 90000; f += 2000) {
        auto t_start = Clock::now();
        int result = eggDropComb(e_fixed4B, f);
        auto t_end = Clock::now();
        long long elapsed = chrono::duration_cast<chrono::milliseconds>(t_end - t_start).count();
        cout << "  e=" << e_fixed4B << ", f=" << f 
             << "：结果 = " << result 
             << "，耗时 " << elapsed << " ms\n";
    }
    cout << endl;
}


// 方法4 特定数据大小测试
void testMethod4Specific() {
    cout << "=== 方法2：二维DP 特定数据测试 ===\n";
    
    // 定义特定的测试用例 (e, f)
    vector<pair<int, int>> testCases = {
        /*
        {20, 100}, 
        {20, 250},
        {20, 500},
        {20, 750},  
        {20, 1000},
        {20, 2500},
        {20, 5000},
        {20, 7500},   
        {20, 10000},
        {20, 25000},
        {20, 50000},
        {20, 75000},
        */
        {10000, 500000000},
        {25000, 500000000},
        {50000, 500000000},
        {75000, 500000000},
        {100000, 500000000},
        {250000, 500000000},
        {500000, 500000000},
        {750000, 500000000},
        {1000000, 500000000},
        {2500000, 500000000},
        {5000000, 500000000},
        {7500000, 500000000},
        {10000000, 500000000},
        {25000000, 500000000},
        {50000000, 500000000},
        {75000000, 500000000},

    };
    for (const auto& testCase : testCases) {
        int e = testCase.first;
        int f = testCase.second;
        
        
        auto t_start = Clock::now();
        int result = eggDropComb(e, f);
        auto t_end = Clock::now();
        long long elapsed = chrono::duration_cast<chrono::milliseconds>(t_end - t_start).count();
        cout << "  e=" << e << ", f=" << f 
             << "：结果 = " << result 
             << "，耗时 " << elapsed << " ms\n";
    }
    cout << endl;
}
int main() {

    testMethod1();
    //testMethod2();
    //testMethod3();
    //testMethod4();
    //testMethod2Specific();
    //testMethod5Specific();
    return 0;
}
