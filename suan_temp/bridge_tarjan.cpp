#include <bits/stdc++.h>
using namespace std;

using pii = pair<int, int>;
using VB = vector<bool>;

// 全局变量
int N, M, timer;
vector<vector<pair<int, int>>> adj; // 邻接表
vector<int> dfn, low;
VB isBridge;               // 标记每条边是否为桥
vector<pii> edges;         // 保存所有边（编号从1开始）

// Tarjan DFS 寻找桥
void dfs(int u, int parentEdge) {
    dfn[u] = low[u] = ++timer;
    for (auto &[v, eid] : adj[u]) {
        if (eid == parentEdge) continue; // 忽略反向边
        if (!dfn[v]) {
            dfs(v, eid);
            low[u] = min(low[u], low[v]);
            if (low[v] > dfn[u]) {
                isBridge[eid] = true;
                cout << "  [桥] 边 " << eid << " 是桥 (" << u << "-" << v << ")" << endl;
            }
        } else {
            low[u] = min(low[u], dfn[v]);
        }
    }
}

// 显式设置图路径并读取图
bool read_graph(const string &filename) {
    ifstream in(filename);
    if (!in) {
        cerr << "无法打开文件: " << filename << endl;
        return false;
    }

    in >> N >> M;
    edges.resize(M + 1);       // 边编号从1开始
    adj.assign(N + 1, {});     // 顶点编号从1开始

    for (int i = 1; i <= M; i++) {
        int u, v;
        in >> u >> v;
        edges[i] = {u, v};
        adj[u].emplace_back(v, i);
        adj[v].emplace_back(u, i);
    }
    return true;
}

// 桥检测主函数
void find_bridges_tarjan() {
    dfn.assign(N + 1, 0);
    low.assign(N + 1, 0);
    isBridge.assign(M + 1, false);
    timer = 0;

    cout << "[Tarjan] 开始遍历图的所有连通分量..." << endl;
    for (int u = 1; u <= N; u++) {
        if (!dfn[u]) {
            cout << "  [DFS] 从节点 " << u << " 开始" << endl;
            dfs(u, -1);
        }
    }

    int cnt = 0;
    for (int i = 1; i <= M; i++) {
        if (isBridge[i]) cnt++;
    }

    cout << "\n[结果] 找到 " << cnt << " 条桥：" << endl;
    for (int i = 1; i <= M; i++) {
        if (isBridge[i]) {
            auto [u, v] = edges[i];
            cout << "  边编号 " << i << ": " << u << "-" << v << endl;
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    
    string graph_path = "graph2.txt";  // 可以替换为你实际的文件路径

    if (!read_graph(graph_path)) {
        cerr << "读取图失败，请检查路径是否正确！" << endl;
        return 1;
    }

    find_bridges_tarjan();
    return 0;
}
