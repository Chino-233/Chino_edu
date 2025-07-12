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

// 改进的图生成算法，更精确控制桥的数量
Graph generate_controlled_graph(int n, int m, int target_bridges) {
    Graph g(n);
    
    // 确保目标桥数合理
    target_bridges = min(target_bridges, min(m, n-1));
    
    // 如果m太小，几乎所有边都会是桥
    if (m < n-1) {
        target_bridges = m; // 所有边都会是桥
    }
    
    // 1. 先确定组件数量，等于桥数+1
    int num_components = target_bridges + 1;
    
    // 组件大小应该尽量均衡
    vector<int> component_sizes(num_components, n / num_components);
    for (int i = 0; i < n % num_components; i++) {
        component_sizes[i]++;
    }
    
    // 确保每个组件至少有3个节点(便于形成环)
    for (int& size : component_sizes) {
        size = max(size, 3);
    }
    
    // 重新调整总节点数，确保不超过n
    int total = accumulate(component_sizes.begin(), component_sizes.end(), 0);
    if (total > n) {
        // 从最大的组件开始减少节点
        while (total > n) {
            auto it = max_element(component_sizes.begin(), component_sizes.end());
            (*it)--;
            total--;
        }
    }
    
    // 2. 创建组件并分配节点
    vector<vector<int>> components(num_components);
    int node_id = 0;
    for (int i = 0; i < num_components; i++) {
        for (int j = 0; j < component_sizes[i] && node_id < n; j++) {
            components[i].push_back(node_id++);
        }
    }
    
    // 如果还有剩余节点，分配到最后一个组件
    while (node_id < n) {
        components.back().push_back(node_id++);
    }
    
    // 3. 在每个组件内部创建一个基本环，确保组件强连通
    for (auto& component : components) {
        if (component.size() <= 1) continue;
        
        // 创建一个环
        for (size_t i = 0; i < component.size(); i++) {
            g.add_edge(component[i], component[(i+1) % component.size()]);
        }
    }
    
    // 4. 添加桥边连接组件
    for (int i = 1; i < num_components; i++) {
        if (components[i-1].empty() || components[i].empty()) continue;
        
        int u = components[i-1][rand() % components[i-1].size()];
        int v = components[i][rand() % components[i].size()];
        g.add_edge(u, v);  // 这是桥
    }
    
    // 5. 在各组件内部添加更多边，直到接近目标边数
    // 留出一些边额度用于后续随机连接
    int remaining_edges = m - g.m;
    int edges_for_components = remaining_edges * 0.9; // 90%的边用于组件内
    
    // 先在每个组件内部添加尽可能多的边
    for (auto& component : components) {
        if (component.size() <= 3) continue; // 小于3个节点的组件已经形成了完全图
        
        // 组件内部可能的最大边数
        int max_edges = component.size() * (component.size() - 1) / 2;
        // 已经添加的边数 (环的边数)
        int current_edges = component.size();
        // 还可以添加的边数
        int available_edges = max_edges - current_edges;
        
        // 按组件大小比例分配边
        int target_edges = min(available_edges, 
                              int(edges_for_components * component.size() / double(n)));
        
        // 尝试添加目标数量的随机边
        int attempts = 0;
        int max_attempts = target_edges * 3;
        while (attempts < max_attempts && target_edges > 0) {
            int idx1 = rand() % component.size();
            int idx2 = rand() % component.size();
            
            if (idx1 != idx2) {
                if (g.add_edge(component[idx1], component[idx2])) {
                    target_edges--;
                }
            }
            attempts++;
        }
    }
    
    // 6. 如果还需要更多边，添加跨组件的随机边（不会破坏桥的性质）
    // 注意：添加跨组件边可能会消除某些桥
    remaining_edges = m - g.m;
    if (remaining_edges > 0) {
        cout << "还需添加 " << remaining_edges << " 条跨组件边" << endl;
        
        int attempts = 0;
        int max_attempts = remaining_edges * 5;
        
        while (g.m < m && attempts < max_attempts) {
            // 随机选择两个不同的组件
            int comp1 = rand() % components.size();
            int comp2 = rand() % components.size();
            
            if (comp1 != comp2 && !components[comp1].empty() && !components[comp2].empty()) {
                int u = components[comp1][rand() % components[comp1].size()];
                int v = components[comp2][rand() % components[comp2].size()];
                
                g.add_edge(u, v);
            }
            attempts++;
        }
    }
    
    // 7. 如果还不够边，在整个图中随机添加
    remaining_edges = m - g.m;
    if (remaining_edges > 0) {
        cout << "仍需随机添加 " << remaining_edges << " 条边" << endl;
        
        int attempts = 0;
        int max_attempts = remaining_edges * 10;
        
        while (g.m < m && attempts < max_attempts) {
            int u = rand() % n;
            int v = rand() % n;
            g.add_edge(u, v);
            attempts++;
        }
    }
    
    cout << "生成图: n=" << n << ", m=" << g.m << " (目标m=" << m << "), 目标桥数=" << target_bridges << endl;
    return g;
}

// 生成一系列平衡的测试图
void generate_balanced_test_graphs(const string& output_dir) {
    // 确保输出目录存在
    filesystem::create_directories(output_dir);
    
    // 更合理的测试集设计
    vector<tuple<int, int, int>> test_cases = {
    {500, 50000, 10},
    {1000, 50000, 15},
    {2000, 50000, 25},
    {5000, 50000, 40},
    {10000, 50000, 60},
    {20000, 50000, 100},
    {50000, 50000, 200},
    {100000, 50000, 400},
    {10000, 10000, 100},
    {10000, 20000, 80},
    {10000, 50000, 60},
    {10000, 100000, 40},
    {10000, 200000, 25},
    {10000, 500000, 15},
    {10000, 1000000, 10}
    };
    
    for (const auto& [n, m, bridges] : test_cases) {
        string filename = output_dir + "/n" + to_string(n) + "_m" + to_string(m) + "_b" + to_string(bridges) + ".txt";
        auto g = generate_controlled_graph(n, m, bridges);
        g.save_to_file(filename);
    }
}

int main(int argc, char* argv[]) {
    srand(time(nullptr));  // 初始化随机种子
    
    string output_dir = "balanced_graphs";  // 默认输出目录
    
    if (argc > 1) {
        output_dir = argv[1];  // 使用命令行参数指定输出目录
    }
    
    cout << "将在目录 '" << output_dir << "' 中生成平衡测试图" << endl;
    generate_balanced_test_graphs(output_dir);
    
    cout << "所有测试图文件已生成完成" << endl;
    return 0;
}