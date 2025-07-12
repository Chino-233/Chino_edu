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

// 获取连通分量信息
vector<int> get_components(const vector<vector<pii>> &adj) {
    vector<int> comp(N+1, 0);
    int comp_id = 0;
    
    for (int start = 1; start <= N; start++) {
        if (comp[start] > 0) continue;
        comp_id++;
        queue<int> q;
        q.push(start);
        comp[start] = comp_id;
        
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (auto &[v, id] : adj[u]) {
                if (comp[v] == 0) {
                    comp[v] = comp_id;
                    q.push(v);
                }
            }
        }
    }
    return comp;
}

// 改进的BFS桥检测算法
VB bridge_bfs() {
    // 构建邻接表
    vector<vector<pii>> adj(N+1);
    for (int i = 1; i <= M; i++) {
        auto [u,v] = edges[i];
        adj[u].push_back({v,i});
        adj[v].push_back({u,i});
    }
    
    // 获取原图的连通分量
    vector<int> original_comp = get_components(adj);
    int num_components = *max_element(original_comp.begin(), original_comp.end());
    
    // 对每条边检查是否为桥
    VB isBridge(M+1, false);
    for (int eid = 1; eid <= M; eid++) {
        auto [u, v] = edges[eid];
        // 如果两个端点在不同连通分量中，这条边不可能是桥
        if (original_comp[u] != original_comp[v]) continue;
        
        // 临时移除该边
        vector<vector<pii>> temp_adj(N+1);
        for (int i = 1; i <= M; i++) {
            if (i == eid) continue;
            auto [x, y] = edges[i];
            temp_adj[x].push_back({y, i});
            temp_adj[y].push_back({x, i});
        }
        
        // 检查移除边后的连通性
        vector<int> new_comp = get_components(temp_adj);
        
        // 如果连通分量数增加，则该边是桥
        if (*max_element(new_comp.begin(), new_comp.end()) > num_components) {
            isBridge[eid] = true;
        }
        // 或者检查原来同一连通分量的点是否在不同连通分量
        else if (original_comp[u] == original_comp[v] && new_comp[u] != new_comp[v]) {
            isBridge[eid] = true;
        }
    }
    return isBridge;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string graph_path = "mediumDG.txt";  // 可以替换为你实际的文件路径

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