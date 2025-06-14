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

using pii = pair<int,int>;
using VB = vector<bool>;

// 全局变量
int N, M;
vector<pair<int,int>> edges;

// 原有线段树算法
vector<vector<int>> seg;   // seg[node] 存一堆要 unite 的边 id
vector<bool> isBridge;     // 最终结果

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

// 对当前 edges/N/M，执行线段树+可回滚并查集算法
VB bridge_rollback_dsu() {
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

// BFS基准算法
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

// 朴素并查集算法
struct DSU {
    vector<int> p;
    DSU(int n):p(n+1){ iota(p.begin(), p.end(), 0); }
    int find(int x){ return p[x]==x?x:p[x]=find(p[x]); }
    void unite(int a,int b){ a=find(a); b=find(b); if(a!=b) p[a]=b; }
};

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

// Tarjan算法
VB bridge_tarjan() {
    vector<vector<pair<int, int>>> adj(N+1);
    for (int i = 1; i <= M; i++) {
        auto [u, v] = edges[i];
        adj[u].emplace_back(v, i);
        adj[v].emplace_back(u, i);
    }
    
    vector<int> dfn(N+1, 0), low(N+1, 0);
    VB isBridge(M+1, false);
    int timer = 0;
    
    function<void(int, int)> dfs = [&](int u, int parentEdge) {
        dfn[u] = low[u] = ++timer;
        for (auto &[v, eid] : adj[u]) {
            if (eid == parentEdge) continue; // 忽略反向边
            if (!dfn[v]) {
                dfs(v, eid);
                low[u] = min(low[u], low[v]);
                if (low[v] > dfn[u]) {
                    isBridge[eid] = true;
                }
            } else {
                low[u] = min(low[u], dfn[v]);
            }
        }
    };
    
    for (int u = 1; u <= N; u++) {
        if (!dfn[u]) {
            dfs(u, -1);
        }
    }
    
    return isBridge;
}

// 显式读取单个图文件
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
    if (cnt <= 100) { // 只在桥数量较少时打印详细信息
        cout << "桥列表 (编号: u-v)\n";
        for (int i = 1; i <= M; i++) {
            if (res[i]) cout << "  " << i << ": " << edges[i].first << "-" << edges[i].second << "\n";
        }
    }
}

// 运行单个文件的所有算法，返回各算法的运行时间
unordered_map<string, double> run_all_algorithms(const string& filename, ofstream& log_file) {
    unordered_map<string, double> times;
    
    log_file << "\n========== 正在处理文件: " << filename << " ==========\n";
    cout << "\n处理文件: " << filename;
    if (!read_graph(filename)) {
        cerr << "无法读取文件: " << filename << endl;
        log_file << "无法读取文件: " << filename << endl;
        return times;
    }
    
    log_file << "图大小: " << N << " 节点, " << M << " 边\n";

    // 运行Tarjan算法（通常最快）
    auto t0 = chrono::steady_clock::now();
    VB tarjan_res = bridge_tarjan();
    auto t1 = chrono::steady_clock::now();
    double tarjan_time = chrono::duration<double>(t1 - t0).count();
    times["Tarjan"] = tarjan_time;
    log_file << "Tarjan 算法耗时: " << tarjan_time << "s, ";
    int bridge_cnt = count(tarjan_res.begin()+1, tarjan_res.end(), true);
    log_file << "桥数量 = " << bridge_cnt << "\n";

    // 运行可回滚并查集算法
    t0 = chrono::steady_clock::now();
    VB rollback_res = bridge_rollback_dsu();
    t1 = chrono::steady_clock::now();
    double rollback_time = chrono::duration<double>(t1 - t0).count();
    times["RollbackDSU"] = rollback_time;
    log_file << "可回滚并查集算法耗时: " << rollback_time << "s, ";
    bridge_cnt = count(rollback_res.begin()+1, rollback_res.end(), true);
    log_file << "桥数量 = " << bridge_cnt << "\n";

    // 对于小图，运行BFS和朴素并查集算法
        // 运行BFS基准算法
        t0 = chrono::steady_clock::now();
        VB bfs_res = bridge_bfs();
        t1 = chrono::steady_clock::now();
        double bfs_time = chrono::duration<double>(t1 - t0).count();
        times["BFS"] = bfs_time;
        log_file << "BFS 基准算法耗时: " << bfs_time << "s, ";
        bridge_cnt = count(bfs_res.begin()+1, bfs_res.end(), true);
        log_file << "桥数量 = " << bridge_cnt << "\n";

        // 运行朴素并查集算法
        t0 = chrono::steady_clock::now();
        VB naive_res = bridge_dsu_naive();
        t1 = chrono::steady_clock::now();
        double naive_time = chrono::duration<double>(t1 - t0).count();
        times["NaiveDSU"] = naive_time;
        log_file << "朴素并查集算法耗时: " << naive_time << "s, ";
        bridge_cnt = count(naive_res.begin()+1, naive_res.end(), true);
        log_file << "桥数量 = " << bridge_cnt << "\n";

        // 验证结果一致性
        bool consistent = true;
        for (int i = 1; i <= M; i++) {
            if (tarjan_res[i] != bfs_res[i] || tarjan_res[i] != rollback_res[i] || tarjan_res[i] != naive_res[i]) {
                consistent = false;
                log_file << "警告: 边 " << i << " 的结果不一致!\n";
                log_file << "  Tarjan: " << tarjan_res[i] 
                         << ", BFS: " << bfs_res[i]
                         << ", RollbackDSU: " << rollback_res[i]
                         << ", NaiveDSU: " << naive_res[i] << "\n";
            }
        }
        if (consistent) {
            log_file << "所有算法的结果一致\n";
        }
    
    return times;
}

// 结果结构体
struct Result {
    string filename;
    int nodes;
    int edges;
    int bridge_count;
    unordered_map<string, double> times;
};

// 批处理目录中的所有图文件
vector<Result> process_directory(const string& dir_path, ofstream& log_file) {
    vector<string> files;
    
    // 读取目录下的所有文件
    for (const auto& entry : filesystem::directory_iterator(dir_path)) {
        if (entry.is_regular_file() && entry.path().extension() == ".txt") {
            files.push_back(entry.path().string());
        }
    }
    
    if (files.empty()) {
        log_file << "目录 " << dir_path << " 中没有找到有效的图文件(.txt)\n";
        cout << "目录 " << dir_path << " 中没有找到有效的图文件(.txt)\n";
        return {};
    }
    
    log_file << "找到 " << files.size() << " 个图文件，开始处理...\n";
    cout << "找到 " << files.size() << " 个图文件，开始处理...\n";
    
    vector<Result> results;
    
    // 处理每个文件
    for (const auto& file : files) {
        auto times = run_all_algorithms(file, log_file);
        
        Result result;
        result.filename = file;
        result.nodes = N;
        result.edges = M;
        
        // 使用Tarjan算法计算桥的数量
        VB bridges = bridge_tarjan();
        result.bridge_count = count(bridges.begin()+1, bridges.end(), true);
        result.times = times;
        
        results.push_back(result);
    }
    
    return results;
}

// 提取文件名的节点数
int extract_nodes(const string& filename) {
    string basename = filesystem::path(filename).filename().string();
    if (basename.find("fixed_n_") != string::npos) {
        // 格式: fixed_n_X_m_Y.txt
        size_t start = basename.find("fixed_n_") + 8;
        size_t end = basename.find("_m_");
        if (end != string::npos) {
            return stoi(basename.substr(start, end - start));
        }
    } else if (basename.find("fixed_m_") != string::npos) {
        // 格式: fixed_m_X_n_Y.txt
        size_t start = basename.find("_n_") + 3;
        size_t end = basename.find(".txt");
        if (end != string::npos) {
            return stoi(basename.substr(start, end - start));
        }
    }
    return 0; // 无法提取
}

// 计算相对速度（以n=1000为基准）
unordered_map<string, vector<pair<int, double>>> calculate_relative_speeds(const vector<Result>& results) {
    unordered_map<string, vector<pair<int, double>>> relative_speeds;
    unordered_map<string, double> base_times; // n=1000的基准时间
    
    // 先找出基准时间 (n=1000)
    for (const auto& result : results) {
        int nodes = extract_nodes(result.filename);
        if (nodes == 1000) {
            for (const auto& [algo, time] : result.times) {
                if (time > 0) {
                    base_times[algo] = time;
                }
            }
            break;
        }
    }
    
    // 如果没有找到n=1000的数据，使用第一个结果作为基准
    if (base_times.empty() && !results.empty()) {
        for (const auto& [algo, time] : results[0].times) {
            if (time > 0) {
                base_times[algo] = time;
            }
        }
    }
    
    // 计算每个结果相对于基准的速度比
    for (const auto& result : results) {
        int nodes = extract_nodes(result.filename);
        if (nodes == 0) continue; // 跳过无法解析的文件名
        
        for (const auto& [algo, time] : result.times) {
            if (time > 0 && base_times.count(algo) && base_times[algo] > 0) {
                // 计算相对性能: 当前时间/基准时间
                double relative = time / base_times[algo];
                relative_speeds[algo].emplace_back(nodes, relative);
            }
        }
    }
    
    // 对每个算法的数据按节点数排序
    for (auto& [algo, data] : relative_speeds) {
        sort(data.begin(), data.end());
    }
    
    return relative_speeds;
}

int main(int argc, char* argv[]) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    // 创建日志文件和结果文件
    ofstream log_file("bridge_log.txt");
    ofstream result_file("bridge_results.csv");
    
    if (!log_file) {
        cerr << "无法创建日志文件 bridge_log.txt" << endl;
        return 1;
    }
    
    if (!result_file) {
        cerr << "无法创建结果文件 bridge_results.csv" << endl;
        return 1;
    }
    
    // 写入CSV表头
    result_file << "文件名,节点数,边数,桥数量,Tarjan(s),回滚DSU(s),BFS(s),朴素DSU(s)" << endl;
    
    vector<Result> all_results;
    
    if (argc == 1) {
        // 没有参数，使用默认文件
        string graph_path = "graph2.txt";
        log_file << "未提供参数，使用默认文件: " << graph_path << endl;
        cout << "未提供参数，使用默认文件: " << graph_path << endl;
        
        auto times = run_all_algorithms(graph_path, log_file);
        
        Result result;
        result.filename = graph_path;
        result.nodes = N;
        result.edges = M;
        VB bridges = bridge_tarjan();
        result.bridge_count = count(bridges.begin()+1, bridges.end(), true);
        result.times = times;
        
        all_results.push_back(result);
    }
    else if (argc == 2) {
        string path = argv[1];
        
        // 检查参数是目录还是文件
        if (filesystem::is_directory(path)) {
            log_file << "处理目录: " << path << endl;
            cout << "处理目录: " << path << endl;
            all_results = process_directory(path, log_file);
        } else {
            log_file << "处理单个文件: " << path << endl;
            cout << "处理单个文件: " << path << endl;
            
            auto times = run_all_algorithms(path, log_file);
            
            Result result;
            result.filename = path;
            result.nodes = N;
            result.edges = M;
            VB bridges = bridge_tarjan();
            result.bridge_count = count(bridges.begin()+1, bridges.end(), true);
            result.times = times;
            
            all_results.push_back(result);
        }
    }
    else if (argc >= 3 && string(argv[1]) == "-f") {
        // 处理单个文件
        string filename = argv[2];
        log_file << "处理单个文件: " << filename << endl;
        cout << "处理单个文件: " << filename << endl;
        
        auto times = run_all_algorithms(filename, log_file);
        
        Result result;
        result.filename = filename;
        result.nodes = N;
        result.edges = M;
        VB bridges = bridge_tarjan();
        result.bridge_count = count(bridges.begin()+1, bridges.end(), true);
        result.times = times;
        
        all_results.push_back(result);
    }
    else if (argc >= 3 && string(argv[1]) == "-d") {
        // 处理目录
        string dir_path = argv[2];
        log_file << "处理目录: " << dir_path << endl;
        cout << "处理目录: " << dir_path << endl;
        all_results = process_directory(dir_path, log_file);
    }
    else {
        log_file << "用法:\n"
             << "  " << argv[0] << " [文件路径]            处理单个文件\n"
             << "  " << argv[0] << " [目录路径]            处理目录中的所有图文件\n"
             << "  " << argv[0] << " -f [文件路径]         处理单个文件\n"
             << "  " << argv[0] << " -d [目录路径]         处理目录中的所有图文件\n";
        cout << "用法:\n"
             << "  " << argv[0] << " [文件路径]            处理单个文件\n"
             << "  " << argv[0] << " [目录路径]            处理目录中的所有图文件\n"
             << "  " << argv[0] << " -f [文件路径]         处理单个文件\n"
             << "  " << argv[0] << " -d [目录路径]         处理目录中的所有图文件\n";
        return 1;
    }
    
    // 打印表格到日志文件
    log_file << "\n\n========== 算法性能比较 ==========\n";
    log_file << setw(30) << "文件名" << " | " 
             << setw(8) << "节点数" << " | " 
             << setw(8) << "边数" << " | " 
             << setw(8) << "桥数量" << " | " 
             << setw(12) << "Tarjan(s)" << " | " 
             << setw(12) << "回滚DSU(s)" << " | " 
             << setw(12) << "BFS(s)" << " | " 
             << setw(12) << "朴素DSU(s)" << "\n";
    
    log_file << string(120, '-') << "\n";
    
    // 写入结果到CSV和日志文件
    for (const auto& result : all_results) {
        string filename = filesystem::path(result.filename).filename().string();
        
        // 写入日志
        log_file << setw(30) << filename << " | " 
                 << setw(8) << result.nodes << " | " 
                 << setw(8) << result.edges << " | " 
                 << setw(8) << result.bridge_count << " | " 
                 << setw(12) << fixed << setprecision(4) << result.times.at("Tarjan") << " | " 
                 << setw(12) << fixed << setprecision(4) << result.times.at("RollbackDSU") << " | ";
        
        if (result.times.at("BFS") < 0)
            log_file << setw(12) << "N/A" << " | ";
        else
            log_file << setw(12) << fixed << setprecision(4) << result.times.at("BFS") << " | ";
        
        if (result.times.at("NaiveDSU") < 0)
            log_file << setw(12) << "N/A";
        else
            log_file << setw(12) << fixed << setprecision(4) << result.times.at("NaiveDSU");
        
        log_file << "\n";
        
        // 写入CSV
        result_file << filename << "," 
                    << result.nodes << "," 
                    << result.edges << "," 
                    << result.bridge_count << "," 
                    << fixed << setprecision(4) << result.times.at("Tarjan") << "," 
                    << fixed << setprecision(4) << result.times.at("RollbackDSU") << ",";
        
        if (result.times.at("BFS") < 0)
            result_file << "N/A,";
        else
            result_file << fixed << setprecision(4) << result.times.at("BFS") << ",";
        
        if (result.times.at("NaiveDSU") < 0)
            result_file << "N/A";
        else
            result_file << fixed << setprecision(4) << result.times.at("NaiveDSU");
        
        result_file << endl;
    }
    
    // 计算相对于n=1000的性能比
    auto relative_speeds = calculate_relative_speeds(all_results);
    
    // 创建相对性能CSV文件
    ofstream relative_file("bridge_relative_performance.csv");
    if (!relative_file) {
        cerr << "无法创建相对性能文件 bridge_relative_performance.csv" << endl;
        return 1;
    }
    
    // 写入相对性能表头
    relative_file << "节点数";
    for (const auto& [algo, _] : relative_speeds) {
        relative_file << "," << algo;
    }
    relative_file << endl;
    
    // 提取所有节点数
    set<int> all_nodes;
    for (const auto& [algo, data] : relative_speeds) {
        for (const auto& [node, _] : data) {
            all_nodes.insert(node);
        }
    }
    
    // 写入每个节点数的相对性能
    for (int node : all_nodes) {
        relative_file << node;
        for (const auto& [algo, data] : relative_speeds) {
            auto it = find_if(data.begin(), data.end(), [node](const pair<int, double>& p) {
                return p.first == node;
            });
            
            if (it != data.end()) {
                relative_file << "," << fixed << setprecision(4) << it->second;
            } else {
                relative_file << ",N/A";
            }
        }
        relative_file << endl;
    }
    
    log_file << "\n========== 相对性能比较 (相对于n=1000) ==========\n";
    log_file << setw(10) << "节点数";
    for (const auto& [algo, _] : relative_speeds) {
        log_file << " | " << setw(12) << algo;
    }
    log_file << "\n" << string(80, '-') << "\n";
    
    for (int node : all_nodes) {
        log_file << setw(10) << node;
        for (const auto& [algo, data] : relative_speeds) {
            auto it = find_if(data.begin(), data.end(), [node](const pair<int, double>& p) {
                return p.first == node;
            });
            
            if (it != data.end()) {
                log_file << " | " << setw(12) << fixed << setprecision(4) << it->second;
            } else {
                log_file << " | " << setw(12) << "N/A";
            }
        }
        log_file << "\n";
    }
    
    cout << "\n所有处理完成！结果已保存到以下文件：" << endl;
    cout << "  - bridge_log.txt (详细日志)" << endl;
    cout << "  - bridge_results.csv (算法性能数据)" << endl;
    cout << "  - bridge_relative_performance.csv (相对于n=1000的性能比)" << endl;
    
    log_file.close();
    result_file.close();
    relative_file.close();
    
    return 0;
}