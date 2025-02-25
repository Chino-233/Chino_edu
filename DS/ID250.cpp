#include <iostream> // 输入输出流
#include <string> // 字符串
#include <vector> // 向量容器
#include <queue> // 队列容器
#include <cstring> // C风格字符串处理
#include <algorithm> // 算法库
#include <stdio.h> // 标准输入输出
#include <math.h> // 数学库
#include <stdlib.h> // 标准库
#include <iomanip> // 输入输出流格式控制
#include <cmath> // 数学库
#include <map> // 映射容器
#include <stack> // 栈容器
#include <set> // 集合容器
#include <unordered_map> // 无序映射容器
#include <unordered_set> // 无序集合容器
#include <bitset> // 位集
#include <list> // 双向链表容器
#include <deque> // 双端队列容器
#include <forward_list> // 单向链表容器
#include <array> // 数组容器
#include <tuple> // 元组
#include <utility> // 实用工具库
#include <memory> // 智能指针
#include <functional> // 函数对象
#include <iterator> // 迭代器
#include <numeric> // 数值算法
using namespace std; // 使用标准命名空间
const int N = 1e5 + 10; // 常量N，表示数组大小
typedef double db; // 定义别名db为double
#define ll long long // 定义别名ll为long long

int main(){
    #ifndef ONLINE_JUDGE
    freopen("in.in", "r", stdin); // 重定向输入到文件in.in
    freopen("out.out", "w", stdout); // 重定向输出到文件out.out
    #endif
    int n; // 定义整数n
    cin >> n; // 输入n
    string point[100]; // 定义字符串数组point，大小为100
    map<string, int> mp; // 定义映射mp，键为字符串，值为整数
    for(int i = 0; i < n; i++){
        cin >> point[i]; // 输入点名称
        mp[point[i]] = i; // 将点名称映射到索引i
    }

    int m; // 定义整数m
    cin >> m; // 输入m
    int edge[100][100]; // 定义二维数组edge，大小为100x100
    memset(edge, 0x3f3f3f3f, sizeof(edge)); // 将edge数组初始化为一个很大的值（表示无穷大）
    for(int i = 0; i < m; i++){
        string s1, s2; // 定义两个字符串s1和s2
        int val = 0; // 定义整数val
        cin >> s1 >> s2; // 输入两个点名称
        cin >> val; // 输入边的权值
        edge[mp[s1]][mp[s2]] = val; // 设置边的权值
        edge[mp[s2]][mp[s1]] = val; // 设置边的权值（无向图）
    }
    int visted[100] = {0}; // 定义并初始化访问数组visted
    int dis[8]; // 定义距离数组dis
    memset(dis, 0x3f3f3f3f, sizeof(dis)); // 将dis数组初始化为一个很大的值（表示无穷大）
    int mindis = 0; // 定义并初始化最小距离mindis
    string start; // 定义字符串start
    cin >> start; // 输入起始点名称
    int flag = mp[start]; // 获取起始点的索引
    string stone[100][2]; // 定义二维字符串数组stone，大小为100x2
    int stone1[100] = {0}; // 定义并初始化整数数组stone1
    int adj[100] = {0}; // 定义并初始化整数数组adj
    for(int i = 0; i < n - 1; i++){
        visted[flag] = 1; // 标记当前点为已访问
        for(int j = 0; j < n; j++){
            if(visted[j] == 0 && j != flag){
                if(dis[j] > edge[flag][j]){
                    dis[j] = edge[flag][j]; // 更新距离
                    adj[j] = flag; // 更新邻接点
                }
            }
        }
        int mi = 0x3f3f3f3f; // 定义并初始化最小值mi
        int flag1 = 0; // 定义并初始化标记flag1
        for(int j = 0; j < n; j++){
            if(!visted[j] && dis[j] < mi){
                flag1 = adj[j]; // 更新标记flag1
                mi = dis[j]; // 更新最小值mi
                flag = j; // 更新标记flag
            }
        }
        stone[i][0] = point[flag1]; // 记录边的起点
        stone[i][1] = point[flag]; // 记录边的终点
        stone1[i] = edge[flag1][flag]; // 记录边的权值
        mindis += mi; // 累加最小距离
    }
    cout << mindis << endl; // 输出最小生成树的总权值
    for(int i = 0; i < n - 1; i++){
        cout << stone[i][0] << " " << stone[i][1] << " " << stone1[i] << endl; // 输出每条边的信息
    }
    return 0; // 返回0，表示程序正常结束
}
