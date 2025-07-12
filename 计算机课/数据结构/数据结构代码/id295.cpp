#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <cstring>
#include <algorithm>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <iomanip>
#include <cmath>
#include <map>
#include <stack>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <bitset>
#include <list>
#include <deque>
#include <forward_list>
#include <array>
#include <tuple>
#include <utility>
#include <memory>
#include <functional>
#include <iterator>
#include <numeric>
using namespace std;

const int N = 1e5 + 10; // 定义常量N
typedef double db; // 定义类型别名db为double
#define ll long long // 定义宏ll为long long

// 在数组a中查找值为find的元素，直到遇到key为止
int find(vector<int> &a, int find, int key) {
    for (int i = a.size() - 1; a[i] != key; i--) {
        if (a[i] == find) {
            return i; // 找到find，返回索引
        } else if (a[i] == key) {
            // cout<<"error"<<endl;
            return 0; // 遇到key，返回0
        }
    }
    return 0; // 未找到，返回0
}

int main() {
    #ifndef ONLINE_JUDGE
    freopen("in.in", "r", stdin); // 重定向输入
    freopen("out.out", "w", stdout); // 重定向输出
    #endif

    int n;
    cin >> n; // 输入数组长度n
    int arr[100] = {0}; // 初始化数组arr
    for (int i = 0; i < n; i++) {
        cin >> arr[i]; // 输入数组元素
    }

    int k;
    cin >> k; // 输入分段数k
    int findarr[100] = {0}; // 初始化查找数组findarr
    for (int i = 0; i < k; i++) {
        cin >> findarr[i]; // 输入查找数组元素
    }

    int t;
    cin >> t; // 输入查询次数t
    while (t--) {
        int partlen = n / k; // 计算每段长度
        int findnum;
        cin >> findnum; // 输入要查找的数字findnum
        int findtwice = 0; // 初始化查找次数
        int findk = 0; // 初始化查找段索引

        // 确定findnum所在的段
        for (int i = 0; i < k; i++) {
            findtwice++;
            if (i == 0) {
                if (findnum < findarr[i]) {
                    findk = 0;
                    break;
                }
            } else if (findnum > findarr[i - 1] && findnum <= findarr[i]) {
                findk = i;
                break;
            }
        }

        int flag = 0; // 初始化标志位
        // 在确定的段中查找findnum
        for (int j = 0; j < partlen; j++) {
            findtwice++;
            if (arr[findk * partlen + j] == findnum) {
                cout << findk * partlen + j + 1 << "-" << findtwice << endl; // 输出查找结果
                flag = 1;
                break;
            }
        }

        if (flag == 0) {
            cout << "error" << endl; // 未找到，输出error
        }
    }
    return 0;
}