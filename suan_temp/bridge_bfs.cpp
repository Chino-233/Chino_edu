#include <bits/stdc++.h>
using namespace std;
using pii = pair<int,int>;
using VB = vector<bool>;

// 全局变量
int N, M;
vector<pii> edges;

// 显式设置图路径并读取图
bool read_graph(const string &filename) {
    ifstream in(filename);
    if (!in) {
        cerr << "无法打开文件: " << filename << endl;
        return false;
    }

    in >> N >> M;
    edges.resize(M + 1);       // 边编号从1开始

    for (int i = 1; i <= M; i++) {
        in >> edges[i].first >> edges[i].second;
    }
    return true;
}

// 打印桥
void print_bridges(const VB &res) {
    int cnt = count(res.begin()+1, res.end(), true);
    cout << "桥数量 = " << cnt << "\n";
    cout << "桥列表 (编号: u-v)\n";
    for (int i = 1; i <= M; i++) {
        if (res[i]) cout << "  " << i << ": " << edges[i].first << "-" << edges[i].second << "\n";
    }
}

// BFS 基准算法
VB bridge_bfs() {
    vector<vector<pii>> adj(N+1);
    for (int i = 1; i <= M; i++) {
        auto [u,v] = edges[i];
        adj[u].push_back({v,i});
        adj[v].push_back({u,i});
    }
    
    VB isBridge(M+1,false), vis(N+1);
    for (int eid = 1; eid <= M; eid++) {
        fill(vis.begin(), vis.end(), false);
        queue<int> q;
        q.push(edges[eid].first);
        vis[edges[eid].first] = true;
        int cnt = 1;
        
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (auto &pr : adj[u]) {
                int v = pr.first, id = pr.second;
                if (id == eid || vis[v]) continue;
                vis[v] = true;
                q.push(v);
                cnt++;
            }
        }
        
        if (cnt < N) isBridge[eid] = true;
    }
    return isBridge;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string graph_path = "graph2.txt";  // 可以替换为你实际的文件路径

    if (!read_graph(graph_path)) {
        cerr << "读取图失败，请检查路径是否正确！" << endl;
        return 1;
    }

    auto t0 = chrono::steady_clock::now();
    VB res = bridge_bfs();
    auto t1 = chrono::steady_clock::now();
    double t = chrono::duration<double>(t1 - t0).count();

    cout << "=== BFS 基准算法 ===\n";
    cout << "耗时: " << t << " s\n";
    print_bridges(res);
    return 0;
}