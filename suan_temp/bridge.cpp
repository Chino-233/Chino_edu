#include <bits/stdc++.h>
using namespace std;

/*
  RollbackDSU：支持 unite() 和 rollback()，
  所有修改都会记录在栈中，rollback(t) 可恢复到栈大小为 t 时的状态。
*/
struct RollbackDSU {
    vector<int> p, sz;
    // stk 存 (who, old_value)，若 who<0 表示修改的是 sz[~who]
    vector<pair<int,int>> stk;

    RollbackDSU(int n): p(n+1), sz(n+1,1) {
        iota(p.begin(), p.end(), 0);
    }
    int find(int x) {
        while (p[x] != x) x = p[x];
        return x;
    }
    void unite(int a, int b) {
        a = find(a); 
        b = find(b);
        if (a == b) return;
        if (sz[a] < sz[b]) swap(a, b);
        // 记录回滚信息
        stk.emplace_back(b, p[b]);
        stk.emplace_back(~a, sz[a]);
        p[b] = a;
        sz[a] += sz[b];
    }
    void rollback(int t) {
        while ((int)stk.size() > t) {
            auto [who, val] = stk.back(); 
            stk.pop_back();
            if (who < 0) {
                sz[~who] = val;
            } else {
                p[who] = val;
            }
        }
    }
};

// 全局变量，用于构建线段树
int N, M;
vector<pair<int,int>> edges;       // 1-based, edges[1..M]
vector<vector<int>> seg;           // seg[node] 存一堆要 unite 的边 id
vector<bool> isBridge;             // 最终结果

// 构造线段树（1..M），这里只需开空间
void build(int node, int l, int r) {
    seg[node].clear();
    if (l == r) return;
    int mid = (l + r) >> 1;
    build(node<<1,   l, mid);
    build(node<<1|1, mid+1, r);
}

// 在区间 [ql,qr] 上插入边 eid
void add_interval(int node, int l, int r, int ql, int qr, int eid) {
    if (ql > r || qr < l) return;
    if (ql <= l && r <= qr) {
        seg[node].push_back(eid);
        return;
    }
    int mid = (l + r) >> 1;
    add_interval(node<<1,   l, mid, ql, qr, eid);
    add_interval(node<<1|1, mid+1, r, ql, qr, eid);
}

// 分治 DFS：node 对应区间 [l,r]
void dfs(int node, int l, int r, RollbackDSU &dsu) {
    int snap = dsu.stk.size();
    // 先把 seg[node] 的所有边加入 DSU
    for (int eid : seg[node]) {
        auto [u, v] = edges[eid];
        dsu.unite(u, v);
    }
    if (l == r) {
        // 此时除了第 l 条边，其它都已加入
        auto [u, v] = edges[l];
        // 若 u, v 不连通，则第 l 条边是桥
        isBridge[l] = (dsu.find(u) != dsu.find(v));
    } else {
        int mid = (l + r) >> 1;
        dfs(node<<1,   l,   mid, dsu);
        dfs(node<<1|1, mid+1, r,   dsu);
    }
    dsu.rollback(snap);
}

// 对当前 edges/N/M，执行桥检测
vector<bool> compute_bridges(int n, int m, const vector<pair<int,int>> &es) {
    N = n; M = m;
    edges = es;
    seg.assign(4*(M+1), {});
    isBridge.assign(M+1, false);

    build(1,1,M);
    // 对每条边 i，除了它之外，其余边都应当"在线段树"上被加入
    for (int i = 1; i <= M; ++i) {
        if (i > 1)       add_interval(1,1,M, 1,   i-1, i);
        if (i < M)       add_interval(1,1,M, i+1, M,   i);
    }

    RollbackDSU dsu(N);
    dfs(1,1,M,dsu);
    return isBridge;
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

    for (int i = 1; i <= M; i++) {
        in >> edges[i].first >> edges[i].second;
    }
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string graph_path = "graph2.txt";  // 可以替换为你实际的文件路径

    if (!read_graph(graph_path)) {
        cerr << "读取图失败，请检查路径是否正确！" << endl;
        return 1;
    }

    auto res = compute_bridges(N, M, edges);

    int cnt = 0;
    for (int i = 1; i <= M; ++i) if (res[i]) ++cnt;
    cout << "桥的数量 = " << cnt << "\n";
    cout << "桥的编号及其端点 (edge_id: u - v)：\n";
    for (int i = 1; i <= M; ++i) {
        if (res[i]) {
            auto [u, v] = edges[i];
            cout << "  " << i << ": " << u << " - " << v << "\n";
        }
    }
    return 0;
}