#include <iostream> // 包含输入输出流库
#include <string> // 包含字符串库
#include <vector> // 包含向量库
#include <queue> // 包含队列库
#include <cstring> // 包含C字符串处理库
#include <algorithm> // 包含算法库
#include <stdio.h> // 包含标准输入输出库
#include <math.h> // 包含数学库
#include <stdlib.h> // 包含标准库
#include <iomanip> // 包含输入输出操作库
#include <cmath> // 包含数学库
#include <map> // 包含映射库
#include <stack> // 包含栈库
#include <set> // 包含集合库
#include <unordered_map> // 包含无序映射库
#include <unordered_set> // 包含无序集合库
#include <list> // 包含列表库
#include <deque> // 包含双端队列库
#include <forward_list> // 包含单向链表库
#include <array> // 包含数组库
#include <tuple> // 包含元组库
#include <utility> // 包含实用工具库
#include <memory> // 包含内存管理库
#include <functional> // 包含函数对象库
#include <iterator> // 包含迭代器库
#include <numeric> // 包含数值运算库
using namespace std; // 使用标准命名空间
const int N = 1e5 + 10; // 定义常量N
typedef double db; // 定义双精度浮点数类型别名
#define ll long long // 定义长整型别名

// 定义边结构体
struct Edge {
    int u, v, w; // 起点u，终点v，权值w
};

// 比较函数，用于排序
bool cmp(Edge a, Edge b) {
    return a.w < b.w; // 按照边的权值升序排序
}

int main() {
    #ifndef ONLINE_JUDGE
    freopen("in.in", "r", stdin); // 重定向输入
    freopen("out.out", "w", stdout); // 重定向输出
    #endif

    int n; // 定义点的数量
    cin >> n; // 输入点的数量
    string point[100]; // 定义点的名称数组
    map<string, int> mp; // 定义点名称到编号的映射

    // 读取点信息，并存储在map中
    for (int i = 0; i < n; i++) {
        cin >> point[i]; // 输入点的名称
        mp[point[i]] = i; // 将点名称映射到编号
    }

    int m; // 定义边的数量
    cin >> m; // 输入边的数量
    Edge edge[100]; // 定义边数组
    memset(edge, 0x3f3f3f3f, sizeof(edge)); // 初始化边数组

    // 读取边信息，并存储在edge数组中
    for (int i = 0; i < m; i++) {
        string s1, s2; // 定义两个点的名称
        int val = 0; // 定义边的权值
        cin >> s1 >> s2; // 输入两个点的名称
        cin >> val; // 输入边的权值
        edge[i].u = mp[s1]; // 将点名称映射到编号
        edge[i].v = mp[s2]; // 将点名称映射到编号
        edge[i].w = val; // 设置边的权值
    }

    // 对边按照权值进行排序
    sort(edge, edge + m, cmp);

    int visted[100] = {0}; // 定义访问数组

    // 初始化访问数组
    for (int i = 0; i < n; i++) {
        visted[i] = -1; // 设置初始值为-1
    }

    string stone[100][3]; // 定义存储结果的数组
    int cut = 0; // 定义集合编号
    int ans = 0; // 定义总权值
    int cut1 = 0; // 定义结果数组索引

    // 遍历所有边
    for (int i = 0; i < m; i++) {
        // 如果两个点都没有被访问过
        if (visted[edge[i].u] == -1 && visted[edge[i].v] == -1) {
            visted[edge[i].u] = cut; // 设置访问标记
            visted[edge[i].v] = cut; // 设置访问标记
            stone[cut1][0] = point[edge[i].u]; // 存储结果
            stone[cut1][1] = point[edge[i].v]; // 存储结果
            stone[cut1][2] = to_string(edge[i].w); // 存储结果
            cut++; // 增加集合编号
            cut1++; // 增加结果数组索引
            ans += edge[i].w; // 增加总权值
        }
        // 如果一个点被访问过，另一个点没有被访问过
        else if (visted[edge[i].u] == -1 && visted[edge[i].v] != -1) {
            visted[edge[i].u] = visted[edge[i].v]; // 设置访问标记
            stone[cut1][0] = point[edge[i].u]; // 存储结果
            stone[cut1][1] = point[edge[i].v]; // 存储结果
            stone[cut1][2] = to_string(edge[i].w); // 存储结果
            cut1++; // 增加结果数组索引
            ans += edge[i].w; // 增加总权值
        }
        // 如果一个点没有被访问过，另一个点被访问过
        else if (visted[edge[i].u] != -1 && visted[edge[i].v] == -1) {
            visted[edge[i].v] = visted[edge[i].u]; // 设置访问标记
            stone[cut1][0] = point[edge[i].u]; // 存储结果
            stone[cut1][1] = point[edge[i].v]; // 存储结果
            stone[cut1][2] = to_string(edge[i].w); // 存储结果
            cut1++; // 增加结果数组索引
            ans += edge[i].w; // 增加总权值
        }
        // 如果两个点都被访问过，且不在同一个集合中
        else if (visted[edge[i].u] != visted[edge[i].v]) {
            int temp = min(visted[edge[i].u], visted[edge[i].v]); // 获取较小的集合编号
            int temp1 = visted[edge[i].u]; // 获取集合编号
            int temp2 = visted[edge[i].v]; // 获取集合编号
            for (int j = 0; j < n; j++) {
                if (visted[j] == temp1 || visted[j] == temp2) {
                    visted[j] = temp; // 合并集合
                }
            }
            stone[cut1][0] = point[edge[i].u]; // 存储结果
            stone[cut1][1] = point[edge[i].v]; // 存储结果
            stone[cut1][2] = to_string(edge[i].w); // 存储结果
            cut1++; // 增加结果数组索引
            ans += edge[i].w; // 增加总权值
        }
    }

    // 检查是否有未访问的点
    for (int i = 0; i < n; i++) {
        if (visted[i] == -1) {
            cout << "-1" << endl; // 输出-1
        }
    }

    cout << ans << endl; // 输出总权值

    // 输出结果
    for (int j = 0; j < cut1; j++) {
        int pos1 = 0; int pos2 = 0; // 定义两个点的编号
        for (int i = 0; i < n; i++) {
            if (stone[j][0] == point[i]) {
                pos1 = i; // 获取点的编号
            }
            if (stone[j][1] == point[i]) {
                pos2 = i; // 获取点的编号
            }
        }
        if (pos1 > pos2) {
            cout << stone[j][1] << " " << stone[j][0] << " " << stone[j][2] << endl; // 输出结果
        } else {
            cout << stone[j][0] << " " << stone[j][1] << " " << stone[j][2] << endl; // 输出结果
        }
    }

    return 0; // 返回0
}