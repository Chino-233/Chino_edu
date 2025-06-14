#include <bits/stdc++.h>
using namespace std;

// 图结构
struct Graph {
    int n, m;                         // 节点数和边数
    vector<pair<int, int>> edges;     // 边列表
    set<pair<int, int>> edge_set;     // 用于去重

    Graph(int _n) : n(_n), m(0) {}
    
    // 添加一条边，返回是否成功添加
    bool add_edge(int u, int v) {
        if (u == v) return false;  // 不允许自环
        if (u > v) swap(u, v);     // 确保 u < v
        
        if (edge_set.count({u, v})) return false;  // 不允许重复边
        
        edge_set.insert({u, v});
        edges.push_back({u, v});
        m++;
        return true;
    }
    
    // 保存图到文件
    void save_to_file(const string& filepath) {
        ofstream out(filepath);
        if (!out) {
            cerr << "无法打开文件: " << filepath << endl;
            return;
        }
        
        out << n << " " << m << endl;
        for (auto [u, v] : edges) {
            out << u + 1 << " " << v + 1 << endl;  // 将0-based转为1-based索引
        }
        
        cout << "图已保存到: " << filepath << endl;
        cout << "节点数: " << n << ", 边数: " << m << endl;
    }
};

// 快速生成带桥的图
Graph generate_graph_fast(int n, int m, int num_bridges) {
    Graph g(n);
    
    // 限制桥的数量不超过n-1
    num_bridges = min(num_bridges, n-1);
    
    // 1. 先将所有节点分为num_bridges+1个组
    int num_groups = num_bridges + 1;
    vector<vector<int>> groups(num_groups);
    
    // 均匀分配节点
    for (int i = 0; i < n; i++) {
        groups[i % num_groups].push_back(i);
    }
    
    // 2. 在每个组内部形成一棵树（保证连通）
    for (auto& group : groups) {
        if (group.size() <= 1) continue;
        
        for (size_t i = 1; i < group.size(); i++) {
            g.add_edge(group[0], group[i]);  // 星形连接
        }
    }
    
    // 3. 添加num_bridges条桥，连接各个组
    for (int i = 1; i < num_groups; i++) {
        if (groups[i-1].empty() || groups[i].empty()) continue;
        
        int u = groups[i-1][0];
        int v = groups[i][0];
        g.add_edge(u, v);  // 这些边是桥
    }
    
    // 4. 在每个组内部添加更多的边，直到接近目标边数
    // 计算还需添加多少条边
    int remaining_edges = m - g.m;
    int max_attempts = remaining_edges * 2;  // 最大尝试次数
    int attempts = 0;
    
    while (g.m < m && attempts < max_attempts) {
        // 选择一个随机组
        int group_idx = rand() % num_groups;
        auto& group = groups[group_idx];
        
        if (group.size() <= 1) continue;
        
        // 在组内随机选择两个节点连接
        int idx1 = rand() % group.size();
        int idx2 = rand() % group.size();
        
        if (idx1 != idx2) {
            g.add_edge(group[idx1], group[idx2]);
        }
        
        attempts++;
    }
    
    // 5. 如果还不够，添加随机边（可能跨越组）
    attempts = 0;
    max_attempts = (m - g.m) * 5;  // 更多尝试次数
    
    while (g.m < m && attempts < max_attempts) {
        int u = rand() % n;
        int v = rand() % n;
        g.add_edge(u, v);
        attempts++;
    }
    
    cout << "生成图: n=" << n << ", m=" << g.m << " (目标m=" << m << "), 桥数≈" << num_bridges << endl;
    return g;
}

// 生成测试图集
void generate_test_graphs(const string& output_dir) {
    // 确保输出目录存在
    filesystem::create_directories(output_dir);
    
    // 固定节点数n=10000，变化边数m
    vector<tuple<int, int, int>> fixed_n_varying_m = {
        {10000, 1000, 20},    // n=10000, m=1000, 桥=20
        {10000, 2500, 25},    // n=10000, m=2500, 桥=25
        {10000, 5000, 30},    // n=10000, m=5000, 桥=30
        {10000, 7500, 35},    // n=10000, m=7500, 桥=35
        {10000, 10000, 40},   // n=10000, m=10000, 桥=40
        {10000, 15000, 45},   // n=10000, m=15000, 桥=45
        {10000, 20000, 50}    // n=10000, m=20000, 桥=50
    };
    
    // 固定边数m=10000，变化节点数n
    vector<tuple<int, int, int>> fixed_m_varying_n = {
        {1000, 10000, 15},    // n=1000, m=10000, 桥=15
        {2500, 10000, 20},    // n=2500, m=10000, 桥=20
        {5000, 10000, 25},    // n=5000, m=10000, 桥=25
        {7500, 10000, 30},    // n=7500, m=10000, 桥=30
        {10000, 10000, 35},   // n=10000, m=10000, 桥=35
        {15000, 10000, 40},   // n=15000, m=10000, 桥=40
        {20000, 10000, 45}    // n=20000, m=10000, 桥=45
    };
    
    // 生成固定n变化m的测试图
    for (const auto& [n, m, bridges] : fixed_n_varying_m) {
        string filename = output_dir + "/fixed_n_" + to_string(n) + "_m_" + to_string(m) + ".txt";
        auto g = generate_graph_fast(n, m, bridges);
        g.save_to_file(filename);
    }
    
    // 生成固定m变化n的测试图
    for (const auto& [n, m, bridges] : fixed_m_varying_n) {
        string filename = output_dir + "/fixed_m_" + to_string(m) + "_n_" + to_string(n) + ".txt";
        auto g = generate_graph_fast(n, m, bridges);
        g.save_to_file(filename);
    }
}

int main(int argc, char* argv[]) {
    srand(time(nullptr));  // 初始化随机种子
    
    string output_dir = "test_graphs";  // 默认输出目录
    
    if (argc > 1) {
        output_dir = argv[1];  // 使用命令行参数指定输出目录
    }
    
    cout << "将在目录 '" << output_dir << "' 中生成测试图" << endl;
    generate_test_graphs(output_dir);
    
    cout << "所有测试图文件已生成完成" << endl;
    return 0;
}