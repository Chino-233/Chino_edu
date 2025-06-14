// 修复版本：使用ISAP算法解决棒球淘汰问题，解决栈溢出问题

#include <bits/stdc++.h>
using namespace std;

// 改进的ISAP最大流算法
struct ISAP {
    struct Edge {
        int to, cap, rev;  // 目标节点、容量、反向边索引
    };
    
    vector<vector<Edge>> adj;  // 邻接表
    vector<int> dist;          // 距离标签
    vector<int> gap;           // gap优化
    vector<int> cur;           // 当前弧优化
    int n, s, t;               // 节点数、源点、汇点
    const int MAX_DIST = 1000; // 最大距离限制，防止无限增长
    
    ISAP(int _n, int _s, int _t) : n(_n), s(_s), t(_t) {
        adj.resize(n);
        dist.assign(n, 0);
        gap.assign(2*n, 0);    // 增大gap数组，防止越界
        cur.assign(n, 0);
    }
    
    void add_edge(int u, int v, int c) {
        if (u >= n || v >= n) return; // 防止越界
        adj[u].push_back({v, c, (int)adj[v].size()});
        adj[v].push_back({u, 0, (int)adj[u].size()-1});
    }
    
    // BFS计算反向距离标签
    void bfs() {
        dist.assign(n, n);
        gap.assign(2*n, 0);    // 重置gap数组
        dist[t] = 0;
        gap[0] = 1;
        
        queue<int> q;
        q.push(t);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (auto &e : adj[u]) {
                int v = e.to;
                if (v < n && adj[v][e.rev].cap > 0 && dist[v] == n) {
                    dist[v] = dist[u] + 1;
                    if (dist[v] < 2*n) gap[dist[v]]++;
                    q.push(v);
                }
            }
        }
    }
    
    // 增加递归深度参数，防止栈溢出
    int dfs(int u, int flow, int depth = 0) {
        if (depth > n*2) return 0;     // 限制递归深度
        if (u == t) return flow;
        
        int remain = flow;
        for (int &i = cur[u]; i < adj[u].size(); i++) {
            Edge &e = adj[u][i];
            if (e.to >= n) continue;   // 防止越界
            
            if (e.cap > 0 && dist[u] == dist[e.to] + 1) {
                int pushed = dfs(e.to, min(remain, e.cap), depth+1);
                if (pushed > 0) {
                    e.cap -= pushed;
                    adj[e.to][e.rev].cap += pushed;
                    remain -= pushed;
                    if (remain == 0) return flow;
                }
            }
        }
        
        // Gap优化和重新标号
        if (dist[u] < 2*n) {
            if (--gap[dist[u]] == 0 && dist[u] < n) {
                dist[s] = n; // 断开s和t的连接
            }
        }
        
        dist[u] = min(dist[u] + 1, MAX_DIST);
        if (dist[u] < 2*n) gap[dist[u]]++;
        cur[u] = 0; // 重置当前弧
        
        return flow - remain;
    }
    
    int maxflow() {
        bfs(); // 初始化距离标签
        int flow = 0;
        while (dist[s] < n) {
            cur.assign(n, 0);
            flow += dfs(s, INT_MAX);
        }
        return flow;
    }
};

// 棒球淘汰问题判定函数
bool is_eliminated_isap(int x, int N,
                      const vector<int>& w,
                      const vector<int>& r_i,
                      const vector<vector<int>>& rij) {
    if (x < 0 || x >= N) return false; // 防止越界
    
    int R_x = r_i[x];
    int maxWinX = w[x] + R_x;

    // 直接淘汰条件：其他队当前胜场 > maxWinX
    for (int i = 0; i < N; i++) {
        if (i != x && w[i] > maxWinX) return true;
    }

    // 构造网络节点编号
    int gameCount = 0;
    vector<vector<int>> gid(N, vector<int>(N, -1));
    for (int i = 0; i < N; i++) {
        for (int j = i+1; j < N; j++) {
            if (i == x || j == x) continue;
            gid[i][j] = gameCount++;
            gid[j][i] = gid[i][j];    // 添加对称性，方便查询
        }
    }
    
    int teamOffset = gameCount;
    int s = teamOffset + N;
    int t = s + 1;
    int totalNodes = t + 1;

    ISAP isap(totalNodes, s, t);

    // 添加边：源点 -> 比赛节点
    int totalGames = 0;
    for (int i = 0; i < N; i++) {
        for (int j = i+1; j < N; j++) {
            if (i == x || j == x) continue;
            if (gid[i][j] < 0) continue;  // 防止索引错误
            
            int g = gid[i][j];
            int cap = rij[i][j];
            totalGames += cap;
            isap.add_edge(s, g, cap);
            
            // 比赛 -> 两支队伍
            isap.add_edge(g, teamOffset + i, INT_MAX);
            isap.add_edge(g, teamOffset + j, INT_MAX);
        }
    }

    // 添加边：队伍 -> 汇点
    for (int i = 0; i < N; i++) {
        if (i == x) continue;
        int cap = maxWinX - w[i];
        cap = max(cap, 0);
        isap.add_edge(teamOffset + i, t, cap);
    }

    // 计算最大流
    int flow = isap.maxflow();
    
    return flow < totalGames;
}

int main() {
    // 实例数据：四支球队
    int N = 4;
    vector<int> w    = {83, 80, 78, 77};  // 已获胜场次
    vector<int> l    = {71, 79, 78, 82};  // 已失败场次
    vector<int> r_i  = { 8,  3,  6,  3};  // 剩余比赛场次
    vector<vector<int>> r = {              // 相互之间的剩余比赛场次
        {0,1,6,1},
        {1,0,0,2},
        {6,0,0,0},
        {1,2,0,0}
    };

    cout << "球队编号从0开始\n\n";
    cout << "球队\t胜场\t负场\t剩余\t结果\t用时(ms)\n";
    cout << "-----------------------------------------------\n";

    for (int x = 0; x < N; x++) {
        auto start = chrono::high_resolution_clock::now();
        bool elim = is_eliminated_isap(x, N, w, r_i, r);
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> elapsed = end - start;
        
        cout << x << "\t" << w[x] << "\t" << l[x] << "\t" << r_i[x] << "\t" 
             << (elim ? "淘汰" : "可能获胜") << "\t" 
             << fixed << setprecision(4) << elapsed.count() << "\n";
    }
    return 0;
}