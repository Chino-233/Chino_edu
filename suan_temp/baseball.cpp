// baseball_elimination.cpp
// 使用 Dinic 算法判断棒球赛淘汰问题的实现

#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int v;       // 终点
    int cap;     // 容量
    int rev;     // 反向边在 adj[v] 中的下标
};

// Dinic 算法实现
struct Dinic {
    int N;
    vector<vector<Edge>> adj;
    vector<int> level, ptr;
    int source, sink;

    Dinic(int n, int s, int t) : N(n), source(s), sink(t) {
        adj.assign(n, {});
        level.assign(n, -1);
        ptr.assign(n, 0);
    }

    void add_edge(int u, int v, int cap) {
        Edge a = {v, cap, (int)adj[v].size()};
        Edge b = {u, 0,   (int)adj[u].size()};
        adj[u].push_back(a);
        adj[v].push_back(b);
    }

    bool bfs() {
        fill(level.begin(), level.end(), -1);
        queue<int> q;
        level[source] = 0;
        q.push(source);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (auto &e : adj[u]) {
                if (level[e.v] < 0 && e.cap > 0) {
                    level[e.v] = level[u] + 1;
                    q.push(e.v);
                }
            }
        }
        return level[sink] >= 0;
    }

    int dfs(int u, int pushed) {
        if (u == sink || pushed == 0) return pushed;
        for (int &cid = ptr[u]; cid < adj[u].size(); cid++) {
            Edge &e = adj[u][cid];
            if (level[e.v] == level[u] + 1 && e.cap > 0) {
                int tr = dfs(e.v, min(pushed, e.cap));
                if (tr > 0) {
                    e.cap -= tr;
                    adj[e.v][e.rev].cap += tr;
                    return tr;
                }
            }
        }
        return 0;
    }

    int maxflow() {
        int flow = 0;
        while (bfs()) {
            fill(ptr.begin(), ptr.end(), 0);
            while (int pushed = dfs(source, INT_MAX)) {
                flow += pushed;
            }
        }
        return flow;
    }
};

// 棒球淘汰问题判定函数
bool is_eliminated(int x, int N,
                   const vector<int>& w,
                   const vector<int>& r_i,
                   const vector<vector<int>>& rij) {
    int R_x = r_i[x];
    int maxWinX = w[x] + R_x;

    // 直接淘汰条件：其他队当前胜场 > maxWinX
    for (int i = 0; i < N; i++) {
        if (i != x && w[i] > maxWinX) return true;
    }

    // 构造网络节点编号
    // source = 0, game nodes 1..M, team nodes M+1..M+N
    int gameCount = 0;
    vector<vector<int>> gameId(N, vector<int>(N, -1));
    for (int i = 0; i < N; i++) {
        for (int j = i+1; j < N; j++) {
            if (i == x || j == x) continue;
            gameId[i][j] = ++gameCount;
        }
    }
    int source = 0;
    int sink = 1 + gameCount + N;
    int totalNodes = sink + 1;

    Dinic dinic(totalNodes, source, sink);

    // 源 -> 游戏节点
    int totalGames = 0;
    for (int i = 0; i < N; i++) {
        for (int j = i+1; j < N; j++) {
            if (gameId[i][j] < 0) continue;
            int gid = gameId[i][j];
            int cap = rij[i][j];
            totalGames += cap;
            dinic.add_edge(source, gid, cap);
            // 游戏 -> 两队节点
            int tid_i = gameCount + 1 + i;
            int tid_j = gameCount + 1 + j;
            dinic.add_edge(gid, tid_i, cap);
            dinic.add_edge(gid, tid_j, cap);
        }
    }

    // 队伍节点 -> 汇点
    for (int i = 0; i < N; i++) {
        if (i == x) continue;
        int tid = gameCount + 1 + i;
        int cap = maxWinX - w[i];
        cap = max(cap, 0);
        dinic.add_edge(tid, sink, cap);
    }

    // 计算最大流并比较
    int flow = dinic.maxflow();
    return flow < totalGames;
}

int main() {
    // 示例输入：四队数据，可按需修改或改为文件/标准输入读取
    int N = 4;
    vector<int> w = {83, 80, 78, 77};
    vector<int> l = {71, 79, 78, 82};
    vector<int> r_i = {8, 3, 6, 3};
    vector<vector<int>> r = {
        {0,1,6,1},
        {1,0,0,2},
        {6,0,0,0},
        {1,2,0,0}
    };

    for (int x = 0; x < N; x++) {
        auto start = chrono::high_resolution_clock::now();
        bool elim = is_eliminated(x, N, w, r_i, r);
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> elapsed = end - start;
        cout << "Team " << x << (elim ? " is eliminated" : " can still win") 
             << " (Time: " << fixed << setprecision(4) << elapsed.count() << " ms)\n";
    }
    return 0;
}
