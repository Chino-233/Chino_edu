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

// 简单 DSU
struct DSU {
    vector<int> p;
    DSU(int n):p(n+1){ iota(p.begin(), p.end(), 0); }
    int find(int x){ return p[x]==x?x:p[x]=find(p[x]); }
    void unite(int a,int b){ a=find(a); b=find(b); if(a!=b) p[a]=b; }
};

// 朴素并查集算法
VB bridge_dsu_naive() {
    VB isBridge(M+1,false);
    for (int eid = 1; eid <= M; eid++) {
        DSU dsu(N);
        for (int i = 1; i <= M; i++) {
            if (i == eid) continue;
            dsu.unite(edges[i].first, edges[i].second);
        }
        auto [u,v] = edges[eid];
        if (dsu.find(u) != dsu.find(v)) isBridge[eid] = true;
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
    VB res = bridge_dsu_naive();
    auto t1 = chrono::steady_clock::now();
    double t = chrono::duration<double>(t1 - t0).count();

    cout << "=== 朴素并查集算法 ===\n";
    cout << "耗时: " << t << " s\n";
    print_bridges(res);
    return 0;
}