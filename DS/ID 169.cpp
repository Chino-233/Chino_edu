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
const int N = 1e5 + 10;
typedef double db;
#define ll long long

// 获取字符串的最长真前后缀
string matched_Prefix_Postfix(string str) {
    int n = str.length();
    vector<int> next(n, 0);
    int len = 0; // 当前已匹配的长度
    int i = 1;

    // 计算next数组
    while (i < n) {
        if (str[i] == str[len]) {
            len++;
            next[i] = len;
            i++;
        } else {
            if (len != 0) {
                len = next[len - 1];
            } else {
                next[i] = 0;
                i++;
            }
        }
    }

    // 如果next[n-1]不为0，说明存在最长真前后缀
    if (next[n - 1] != 0) {
        return str.substr(0, next[n - 1]);
    } else {
        return "empty";
    }
}
int main() {
    #ifndef ONLINE_JUDGE
    freopen("in.in", "r", stdin);
    freopen("out.out", "w", stdout);
    #endif

    int t;
    cin >> t;
    while (t--) {
        string str;
        cin >> str;
        cout << matched_Prefix_Postfix(str) << endl;
    }

    return 0;
}
