#include <bits/stdc++.h>
using namespace std;
using namespace chrono;

// 基础边结构
struct Edge {
    int to, cap, rev;
};

// Ford-Fulkerson算法实现（保持原始版本的特性）
struct FordFulkerson {
    vector<vector<Edge>> graph;
    vector<bool> visited;
    int n;
    
    FordFulkerson(int _n) : n(_n) {
        graph.resize(n);
        visited.resize(n);
    }
    
    void add_edge(int from, int to, int cap) {
        graph[from].push_back({to, cap, (int)graph[to].size()});
        graph[to].push_back({from, 0, (int)graph[from].size() - 1});
    }
    
    int dfs(int v, int t, int f) {
        if (v == t) return f;
        visited[v] = true;
        for (auto &e : graph[v]) {
            if (!visited[e.to] && e.cap > 0) {
                int d = dfs(e.to, t, min(f, e.cap));
                if (d > 0) {
                    e.cap -= d;
                    graph[e.to][e.rev].cap += d;
                    return d;
                }
            }
        }
        return 0;
    }
    
    int max_flow(int s, int t) {
        int flow = 0;
        while (true) {
            fill(visited.begin(), visited.end(), false);
            int f = dfs(s, t, INT_MAX);
            if (f == 0) break;
            flow += f;
        }
        return flow;
    }
};

// Edmonds-Karp算法实现
struct EdmondsKarp {
    vector<vector<Edge>> graph;
    int n;
    
    EdmondsKarp(int _n) : n(_n) {
        graph.resize(n);
    }
    
    void add_edge(int from, int to, int cap) {
        graph[from].push_back({to, cap, (int)graph[to].size()});
        graph[to].push_back({from, 0, (int)graph[from].size() - 1});
    }
    
    int bfs(int s, int t, vector<int>& parent) {
        fill(parent.begin(), parent.end(), -1);
        parent[s] = -2;
        queue<pair<int, int>> q;
        q.push({s, INT_MAX});
        
        while (!q.empty()) {
            int cur = q.front().first;
            int flow = q.front().second;
            q.pop();
            
            for (int i = 0; i < graph[cur].size(); i++) {
                Edge &e = graph[cur][i];
                if (parent[e.to] == -1 && e.cap > 0) {
                    parent[e.to] = cur * 10000 + i;
                    int new_flow = min(flow, e.cap);
                    if (e.to == t) return new_flow;
                    q.push({e.to, new_flow});
                }
            }
        }
        return 0;
    }
    
    int max_flow(int s, int t) {
        int flow = 0;
        vector<int> parent(n);
        int new_flow;
        
        while ((new_flow = bfs(s, t, parent)) > 0) {
            flow += new_flow;
            int cur = t;
            while (cur != s) {
                int prev = parent[cur] / 10000;
                int edge_idx = parent[cur] % 10000;
                graph[prev][edge_idx].cap -= new_flow;
                graph[cur][graph[prev][edge_idx].rev].cap += new_flow;
                cur = prev;
            }
        }
        return flow;
    }
};

// Dinic算法实现
struct Dinic {
    vector<vector<Edge>> graph;
    vector<int> level, iter;
    int n;
    
    Dinic(int _n) : n(_n) {
        graph.resize(n);
        level.resize(n);
        iter.resize(n);
    }
    
    void add_edge(int from, int to, int cap) {
        graph[from].push_back({to, cap, (int)graph[to].size()});
        graph[to].push_back({from, 0, (int)graph[from].size() - 1});
    }
    
    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        queue<int> q;
        level[s] = 0;
        q.push(s);
        
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (auto &e : graph[v]) {
                if (e.cap > 0 && level[e.to] < 0) {
                    level[e.to] = level[v] + 1;
                    q.push(e.to);
                }
            }
        }
        return level[t] >= 0;
    }
    
    int dfs(int v, int t, int pushed) {
        if (v == t || pushed == 0) return pushed;
        for (int &cid = iter[v]; cid < graph[v].size(); cid++) {
            Edge &e = graph[v][cid];
            if (level[v] + 1 != level[e.to] || e.cap <= 0) continue;
            int tr = dfs(e.to, t, min(pushed, e.cap));
            if (tr > 0) {
                e.cap -= tr;
                graph[e.to][e.rev].cap += tr;
                return tr;
            }
        }
        return 0;
    }
    
    int max_flow(int s, int t) {
        int flow = 0;
        while (bfs(s, t)) {
            fill(iter.begin(), iter.end(), 0);
            while (int pushed = dfs(s, t, INT_MAX)) {
                flow += pushed;
            }
        }
        return flow;
    }
};

// 改进的图生成器
struct GraphGenerator {
    mt19937 rng;
    
    GraphGenerator() : rng(chrono::steady_clock::now().time_since_epoch().count()) {}
    
    // 生成挑战性的图并保存到文件
    void generate_and_save_graph(int n, int m, const string& filename) {
        ofstream graph_file(filename);
        graph_file << n << " " << m << "\n";
        
        set<pair<int, int>> used;
        vector<tuple<int, int, int>> edges;
        
        // 创建多层结构，源点到汇点需要经过多个层次
        int layers = min(8, n / 20);
        if (layers < 2) layers = 2;
        
        int nodes_per_layer = n / layers;
        if (nodes_per_layer < 2) nodes_per_layer = 2;
        
        // 确保源点和汇点在两端
        int source = 0;
        int sink = n - 1;
        
        // 第一步：创建层次结构的基本连接
        // 从源点连接到第一层
        for (int i = 1; i <= min(nodes_per_layer, n - 2); i++) {
            if (used.find({source, i}) == used.end()) {
                int cap = uniform_int_distribution<int>(100, 1000)(rng);
                edges.push_back({source, i, cap});
                used.insert({source, i});
            }
        }
        
        // 层间连接 - 创建瓶颈
        for (int layer = 1; layer < layers - 1 && edges.size() < m; layer++) {
            int start_node = layer * nodes_per_layer;
            int end_node = min((layer + 1) * nodes_per_layer, n - 1);
            
            if (start_node >= n - 1 || end_node >= n) break;
            
            // 在层之间创建稠密连接，但容量有变化
            for (int i = start_node; i < start_node + nodes_per_layer && i < n - 1; i++) {
                for (int j = end_node - nodes_per_layer; j < end_node && j < n - 1; j++) {
                    if (edges.size() >= m) break;
                    if (i != j && used.find({i, j}) == used.end()) {
                        // 使用不同的容量范围来创建瓶颈效应
                        int capacity;
                        if (layer == layers / 2) {
                            // 中间层使用小容量创建瓶颈
                            capacity = uniform_int_distribution<int>(1, 50)(rng);
                        } else {
                            capacity = uniform_int_distribution<int>(50, 500)(rng);
                        }
                        edges.push_back({i, j, capacity});
                        used.insert({i, j});
                    }
                }
                if (edges.size() >= m) break;
            }
            if (edges.size() >= m) break;
        }
        
        // 最后一层连接到汇点
        int last_layer_start = max(1, (layers - 1) * nodes_per_layer);
        for (int i = last_layer_start; i < n - 1 && edges.size() < m; i++) {
            if (used.find({i, sink}) == used.end()) {
                int cap = uniform_int_distribution<int>(100, 1000)(rng);
                edges.push_back({i, sink, cap});
                used.insert({i, sink});
            }
        }
        
        // 填充剩余边 - 添加一些"捷径"但容量较小
        while (edges.size() < m && edges.size() < (long long)n * (n - 1) / 2) {
            int u = uniform_int_distribution<int>(0, n - 2)(rng);
            int v = uniform_int_distribution<int>(u + 1, n - 1)(rng);
            
            if (used.find({u, v}) == used.end()) {
                // 捷径边使用中等容量
                int capacity = uniform_int_distribution<int>(10, 200)(rng);
                edges.push_back({u, v, capacity});
                used.insert({u, v});
            }
        }
        
        // 如果边数还不够，添加更多随机边
        while (edges.size() < m && edges.size() < (long long)n * (n - 1) / 2) {
            int u = uniform_int_distribution<int>(0, n - 2)(rng);
            int v = uniform_int_distribution<int>(u + 1, n - 1)(rng);
            
            if (used.find({u, v}) == used.end()) {
                int capacity = uniform_int_distribution<int>(1, 1000)(rng);
                edges.push_back({u, v, capacity});
                used.insert({u, v});
            }
        }
        
        // 将边写入文件
        for (const auto& e : edges) {
            graph_file << get<0>(e) << " " << get<1>(e) << " " << get<2>(e) << "\n";
        }
        
        graph_file.close();
    }
    
    // 从文件读取图并测试算法
    double test_algorithm_from_file(const string& algo_name, const string& filename, int runs = 3) {
        double total_time = 0;
        
        for (int run = 0; run < runs; run++) {
            ifstream graph_file(filename);
            int n, m;
            graph_file >> n >> m;
            
            auto start = high_resolution_clock::now();
            
            if (algo_name == "Ford-Fulkerson") {
                FordFulkerson ff(n);
                for (int i = 0; i < m; i++) {
                    int u, v, cap;
                    graph_file >> u >> v >> cap;
                    ff.add_edge(u, v, cap);
                }
                ff.max_flow(0, n-1);
            }
            else if (algo_name == "Edmonds-Karp") {
                EdmondsKarp ek(n);
                for (int i = 0; i < m; i++) {
                    int u, v, cap;
                    graph_file >> u >> v >> cap;
                    ek.add_edge(u, v, cap);
                }
                ek.max_flow(0, n-1);
            }
            else if (algo_name == "Dinic") {
                Dinic dinic(n);
                for (int i = 0; i < m; i++) {
                    int u, v, cap;
                    graph_file >> u >> v >> cap;
                    dinic.add_edge(u, v, cap);
                }
                dinic.max_flow(0, n-1);
            }
            
            auto end = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(end - start);
            total_time += duration.count() / 1000.0; // 转换为毫秒
            
            graph_file.close();
        }
        
        return total_time / runs;
    }
};

int main() {
    GraphGenerator gen;
    
    cout << "开始性能测试..." << endl;
    
    // 创建图文件存储目录
    string graph_dir = "D:\\Chino_edu\\suan_temp\\graphs\\";
    system(("mkdir \"" + graph_dir + "\" 2>nul").c_str());
    
    // 输出文件
    ofstream data_file("D:\\Chino_edu\\suan_temp\\performance_data_enhanced.csv");
    data_file << "测试类型,参数,Ford-Fulkerson,Edmonds-Karp,Dinic\n";
    
    // 第一组测试：固定节点数，变化边数
    cout << "第一组测试：固定节点数2000，变化边数" << endl;
    vector<int> edge_counts = {
        2000, 3000, 4000, 5000, 7500, 10000, 15000, 20000, 
        25000, 30000, 40000, 50000, 60000, 75000, 100000
    };
    int fixed_nodes = 2000;
    
    for (int edges : edge_counts) {
        // 检查边数是否超过最大可能边数
        long long max_edges = (long long)fixed_nodes * (fixed_nodes - 1) / 2;
        if (edges > max_edges) {
            cout << "边数 " << edges << " 超过最大可能边数 " << max_edges << "，跳过" << endl;
            continue;
        }
        
        cout << "测试边数: " << edges << endl;
        
        // 生成图文件
        string graph_filename = graph_dir + "graph_n" + to_string(fixed_nodes) + "_m" + to_string(edges) + ".txt";
        gen.generate_and_save_graph(fixed_nodes, edges, graph_filename);
        cout << "  图已保存到: " << graph_filename << endl;
        
        // 测试三个算法
        double ff_time = gen.test_algorithm_from_file("Ford-Fulkerson", graph_filename);
        double ek_time = gen.test_algorithm_from_file("Edmonds-Karp", graph_filename);
        double dinic_time = gen.test_algorithm_from_file("Dinic", graph_filename);
        
        cout << "  Ford-Fulkerson: " << ff_time << "ms" << endl;
        cout << "  Edmonds-Karp: " << ek_time << "ms" << endl;
        cout << "  Dinic: " << dinic_time << "ms" << endl;
        
        data_file << "固定节点2000,边数" << edges << "," 
                 << ff_time/1000.0 << "," << ek_time/1000.0 << "," << dinic_time/1000.0 << "\n";
        
        // 如果时间超过5秒，跳出循环
        if (max({ff_time, ek_time, dinic_time}) > 5000) {
            cout << "运行时间超过5秒，停止增加边数测试" << endl;
            break;
        }
    }
    /*
    // 第二组测试：固定边数，变化节点数
    cout << "\n第二组测试：固定边数10000，变化节点数" << endl;
    vector<int> node_counts = {
        500, 750, 1000, 1250, 1500, 2000, 
        2500, 3000, 4000, 5000, 6000, 7500, 10000
    };
    int fixed_edges = 10000;
    
    for (int nodes : node_counts) {
        // 检查节点数是否能支持这么多边
        long long max_edges = (long long)nodes * (nodes - 1) / 2;
        if (fixed_edges > max_edges) {
            cout << "节点数 " << nodes << " 无法支持 " << fixed_edges << " 条边（最大 " << max_edges << "），跳过" << endl;
            continue;
        }
        
        cout << "测试节点数: " << nodes << endl;
        
        // 生成图文件
        string graph_filename = graph_dir + "graph_n" + to_string(nodes) + "_m" + to_string(fixed_edges) + ".txt";
        gen.generate_and_save_graph(nodes, fixed_edges, graph_filename);
        cout << "  图已保存到: " << graph_filename << endl;
        
        // 测试三个算法
        double ff_time = gen.test_algorithm_from_file("Ford-Fulkerson", graph_filename);
        double ek_time = gen.test_algorithm_from_file("Edmonds-Karp", graph_filename);
        double dinic_time = gen.test_algorithm_from_file("Dinic", graph_filename);
        
        cout << "  Ford-Fulkerson: " << ff_time << "ms" << endl;
        cout << "  Edmonds-Karp: " << ek_time << "ms" << endl;
        cout << "  Dinic: " << dinic_time << "ms" << endl;
        
        data_file << "固定边数10000,节点" << nodes << "," 
                 << ff_time/1000.0 << "," << ek_time/1000.0 << "," << dinic_time/1000.0 << "\n";
        
        // 如果时间超过5秒，跳出循环
        if (max({ff_time, ek_time, dinic_time}) > 5000) {
            cout << "运行时间超过5秒，停止增加节点数测试" << endl;
            break;
        }
    }
    */
    data_file.close();
    
    cout << "\n性能测试完成！" << endl;
    cout << "数据已保存到: performance_data_enhanced.csv" << endl;
    cout << "图文件已保存到: " << graph_dir << endl;
    
    return 0;
}