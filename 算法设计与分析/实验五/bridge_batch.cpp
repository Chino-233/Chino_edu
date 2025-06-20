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

// 对当前 edges/N/M，执行线段树+可回滚并查集算法 - 分离初始化与核心算法
VB bridge_rollback_dsu() {
    // 初始化部分（不计时）
    seg.assign(4*(M+1), {});
    isBridge.assign(M+1, false);

    build(1,1,M);
    // 对每条边 i，除了它之外，其余边都应当"在线段树"上被加入
    for (int i = 1; i <= M; ++i) {
        if (i > 1)       add_interval(1,1,M, 1,   i-1, i);
        if (i < M)       add_interval(1,1,M, i+1, M,   i);
    }

    RollbackDSU dsu(N);
    
    // 从这里开始计时（只计算核心算法部分）
    auto t0 = chrono::steady_clock::now();
    dfs(1,1,M,dsu);
    auto t1 = chrono::steady_clock::now();
    double time_ms = chrono::duration<double, milli>(t1 - t0).count();
    cout << "回滚DSU算法内部计时: " << fixed << setprecision(6) << time_ms << " ms\n";
    
    return isBridge;
}

// BFS基准算法 - 分离初始化与核心算法
VB bridge_bfs() {
    // 初始化部分（不计时）
    vector<vector<pii>> adj(N+1);
    for (int i = 1; i <= M; i++) {
        auto [u,v] = edges[i];
        adj[u].push_back({v,i});
        adj[v].push_back({u,i});
    }
    
    VB isBridge(M+1,false);
    vector<bool> vis(N+1);
    queue<int> q;
    
    // 从这里开始计时（只计算核心算法部分）
    auto t0 = chrono::steady_clock::now();
    
    for (int eid = 1; eid <= M; eid++) {
        fill(vis.begin(), vis.end(), false);
        while (!q.empty()) q.pop();  // 确保队列为空
        
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
    
    auto t1 = chrono::steady_clock::now();
    double time_ms = chrono::duration<double, milli>(t1 - t0).count();
    cout << "BFS算法内部计时: " << fixed << setprecision(6) << time_ms << " ms\n";
    
    return isBridge;
}

// 优化的朴素并查集算法 - 预分配并重用一个并查集数组
VB bridge_dsu_naive() {
    // 在计时外部进行初始化
    vector<int> parent(N+1);  // 预分配一次
    VB isBridge(M+1, false);
    
    // 从这里开始计时（只计算核心算法部分）
    auto t0 = chrono::steady_clock::now();
    
    for (int eid = 1; eid <= M; eid++) {
        // 重置而不是重新分配
        iota(parent.begin(), parent.end(), 0);
        
        // 带路径压缩的find函数(迭代实现，避免递归栈溢出)
        auto find = [&parent](int x) {
            int root = x;
            // 找到根节点
            while (parent[root] != root)
                root = parent[root];
            // 路径压缩
            while (x != root) {
                int next = parent[x];
                parent[x] = root;
                x = next;
            }
            return root;
        };
        
        // 核心算法：添加除当前边外的所有边到并查集
        for (int i = 1; i <= M; i++) {
            if (i == eid) continue;
            int u = find(edges[i].first);
            int v = find(edges[i].second);
            if (u != v) parent[u] = v;
        }
        
        // 检查当前边是否是桥
        int u = find(edges[eid].first);
        int v = find(edges[eid].second);
        if (u != v) isBridge[eid] = true;
    }
    
    auto t1 = chrono::steady_clock::now();
    double time_ms = chrono::duration<double, milli>(t1 - t0).count();
    cout << "朴素DSU算法内部计时: " << fixed << setprecision(6) << time_ms << " ms\n";
    
    return isBridge;
}

// Tarjan算法 - 分离初始化与核心算法
VB bridge_tarjan() {
    // 初始化部分（不计时）
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
    
    // 从这里开始计时（只计算核心算法部分）
    auto t0 = chrono::steady_clock::now();
    
    for (int u = 1; u <= N; u++) {
        if (!dfn[u]) {
            dfs(u, -1);
        }
    }
    
    auto t1 = chrono::steady_clock::now();
    double time_ms = chrono::duration<double, milli>(t1 - t0).count();
    cout << "Tarjan算法内部计时: " << fixed << setprecision(6) << time_ms << " ms\n";
    
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

// 根据文件名和规模判断图的类别
enum class GraphSize {
    SMALL,   // 适合所有算法 (n < 5000)
    MEDIUM,  // 适合回滚DSU和Tarjan (n < 50000)
    LARGE    // 仅适合Tarjan (n >= 50000)
};

GraphSize determine_graph_size(const string& filename, int n, int m) {
    // 从文件名判断
    string basename = filesystem::path(filename).filename().string();
    if (basename.find("small_") != string::npos) {
        return GraphSize::SMALL;
    } else if (basename.find("medium_") != string::npos) {
        return GraphSize::MEDIUM;
    } else if (basename.find("large_") != string::npos) {
        return GraphSize::LARGE;
    }
    
    // 从规模判断
    if (n < 5000 && m < 25000) {
        return GraphSize::SMALL;
    } else if (n < 50000 && m < 100000) {
        return GraphSize::MEDIUM;
    } else {
        return GraphSize::LARGE;
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
    
    // 确定图的规模类别
    GraphSize size = determine_graph_size(filename, N, M);
    log_file << "图类别: " << (size == GraphSize::SMALL ? "小型图" : 
                             size == GraphSize::MEDIUM ? "中型图" : "大型图") << "\n";

    // 所有规模的图都运行Tarjan算法
    cout << "\n运行Tarjan算法...\n";
    auto t0 = chrono::steady_clock::now();
    VB tarjan_res = bridge_tarjan();  // 内部有计时
    auto t1 = chrono::steady_clock::now();
    double tarjan_time = chrono::duration<double, milli>(t1 - t0).count();
    times["Tarjan"] = tarjan_time;
    log_file << "Tarjan 算法总耗时: " << fixed << setprecision(6) << tarjan_time << " ms, ";
    int bridge_cnt = count(tarjan_res.begin()+1, tarjan_res.end(), true);
    log_file << "桥数量 = " << bridge_cnt << "\n";

    // 声明在外部，便于后续引用
    VB rollback_res, bfs_res, naive_res;
    
    // 小型和中型图运行回滚DSU
    if (size != GraphSize::LARGE) {
        cout << "\n运行可回滚DSU算法...\n";
        t0 = chrono::steady_clock::now();
        rollback_res = bridge_rollback_dsu();  // 内部有计时
        t1 = chrono::steady_clock::now();
        double rollback_time = chrono::duration<double, milli>(t1 - t0).count();
        times["RollbackDSU"] = rollback_time;
        log_file << "可回滚并查集算法总耗时: " << fixed << setprecision(6) << rollback_time << " ms, ";
        bridge_cnt = count(rollback_res.begin()+1, rollback_res.end(), true);
        log_file << "桥数量 = " << bridge_cnt << "\n";
        
        // 验证回滚DSU结果与Tarjan一致
        bool consistent = true;
        for (int i = 1; i <= M; i++) {
            if (tarjan_res[i] != rollback_res[i]) {
                consistent = false;
                log_file << "警告: 边 " << i << " 的结果不一致: Tarjan=" << tarjan_res[i] 
                         << ", RollbackDSU=" << rollback_res[i] << "\n";
            }
        }
        if (consistent) {
            log_file << "Tarjan和回滚DSU算法结果一致\n";
        }
    } else {
        // 大型图跳过回滚DSU
        times["RollbackDSU"] = -1;
        log_file << "跳过回滚DSU算法 (图太大)\n";
    }

    // 只有小型图运行BFS和朴素DSU
    if (size == GraphSize::SMALL) {
        // 运行BFS基准算法
        cout << "\n运行BFS基准算法...\n";
        t0 = chrono::steady_clock::now();
        bfs_res = bridge_bfs();  // 内部有计时
        t1 = chrono::steady_clock::now();
        double bfs_time = chrono::duration<double, milli>(t1 - t0).count();
        times["BFS"] = bfs_time;
        log_file << "BFS 基准算法总耗时: " << fixed << setprecision(6) << bfs_time << " ms, ";
        bridge_cnt = count(bfs_res.begin()+1, bfs_res.end(), true);
        log_file << "桥数量 = " << bridge_cnt << "\n";
        
        // 运行朴素并查集算法
        cout << "\n运行朴素DSU算法...\n";
        t0 = chrono::steady_clock::now();
        naive_res = bridge_dsu_naive();  // 内部有计时
        t1 = chrono::steady_clock::now();
        double naive_time = chrono::duration<double, milli>(t1 - t0).count();
        times["NaiveDSU"] = naive_time;
        log_file << "朴素并查集算法总耗时: " << fixed << setprecision(6) << naive_time << " ms, ";
        bridge_cnt = count(naive_res.begin()+1, naive_res.end(), true);
        log_file << "桥数量 = " << bridge_cnt << "\n";
        
        // 验证所有算法结果一致性
        bool consistent = true;
        for (int i = 1; i <= M; i++) {
            if (tarjan_res[i] != bfs_res[i] || tarjan_res[i] != naive_res[i] ||
                (size != GraphSize::LARGE && tarjan_res[i] != rollback_res[i])) {
                consistent = false;
                log_file << "警告: 边 " << i << " 的结果不一致!\n";
            }
        }
        if (consistent) {
            log_file << "所有算法的结果一致\n";
        }
    } else {
        // 中大型图跳过BFS和朴素DSU
        times["BFS"] = -1;
        times["NaiveDSU"] = -1;
        log_file << "跳过BFS和朴素DSU算法 (图太大)\n";
    }
    
    return times;
}

// 结果结构体
struct Result {
    string filename;
    int nodes;
    int edges;
    int bridge_count;
    GraphSize size;  // 新增：图规模类别
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
    
    // 按大小排序文件名，先处理小文件
    sort(files.begin(), files.end(), [](const string& a, const string& b) {
        // 小规模图优先
        if (a.find("small_") != string::npos && b.find("small_") == string::npos) return true;
        if (a.find("small_") == string::npos && b.find("small_") != string::npos) return false;
        
        // 中规模图其次
        if (a.find("medium_") != string::npos && b.find("medium_") == string::npos) return true;
        if (a.find("medium_") == string::npos && b.find("medium_") != string::npos) return false;
        
        // 最后是大规模图，按字典序排列
        return a < b;
    });
    
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
        result.size = determine_graph_size(file, N, M);
        
        // 使用Tarjan算法计算桥的数量
        VB bridges = bridge_tarjan();
        result.bridge_count = count(bridges.begin()+1, bridges.end(), true);
        result.times = times;
        
        results.push_back(result);
    }
    
    return results;
}

// 提取文件名中的信息
void parse_filename_info(const string& filename, int& n, int& m, int& b) {
    string basename = filesystem::path(filename).filename().string();
    
    // 尝试从新格式中提取信息 (n{数字}_m{数字}_b{数字})
    regex pattern("n(\\d+)_m(\\d+)_b(\\d+)");
    smatch matches;
    if (regex_search(basename, matches, pattern) && matches.size() == 4) {
        n = stoi(matches[1].str());
        m = stoi(matches[2].str());
        b = stoi(matches[3].str());
        return;
    }
    
    // 尝试从固定格式中提取信息
    if (basename.find("fixed_n_") != string::npos) {
        // 格式: fixed_n_X_m_Y.txt
        size_t start = basename.find("fixed_n_") + 8;
        size_t end = basename.find("_m_");
        if (end != string::npos) {
            n = stoi(basename.substr(start, end - start));
            start = basename.find("_m_") + 3;
            m = stoi(basename.substr(start));
        }
    } else if (basename.find("fixed_m_") != string::npos) {
        // 格式: fixed_m_X_n_Y.txt
        size_t start = basename.find("fixed_m_") + 8;
        size_t end = basename.find("_n_");
        if (end != string::npos) {
            m = stoi(basename.substr(start, end - start));
            start = basename.find("_n_") + 3;
            n = stoi(basename.substr(start));
        }
    }
    
    // 若文件名中没有指定桥的数量，设为-1表示未知
    b = -1;
}

// 将结果分类并计算每类的平均性能
void generate_category_analysis(const vector<Result>& results, ofstream& log_file) {
    // 按图大小类别和边节点比例分类
    map<pair<GraphSize, string>, vector<Result>> categorized;
    
    for (const auto& result : results) {
        int n = result.nodes;
        int m = result.edges;
        
        // 确定边节点比例类别
        string ratio_category;
        double ratio = (double)m / n;
        
        if (ratio < 1.2) ratio_category = "稀疏图(m≈n)";
        else if (ratio < 3) ratio_category = "中等密度(m≈2n)";
        else ratio_category = "密集图(m>3n)";
        
        categorized[{result.size, ratio_category}].push_back(result);
    }
    
    log_file << "\n\n========== 按类别分析结果 ==========\n\n";
    
    // 对每个类别计算平均性能
    for (const auto& [category, category_results] : categorized) {
        auto [size, ratio] = category;
        string size_str = size == GraphSize::SMALL ? "小型图" : 
                          size == GraphSize::MEDIUM ? "中型图" : "大型图";
        
        log_file << size_str << " + " << ratio << " (" << category_results.size() << "个测试图)\n";
        log_file << "--------------------------\n";
        
        // 计算平均时间
        unordered_map<string, double> avg_times;
        unordered_map<string, int> counts;
        
        for (const auto& result : category_results) {
            for (const auto& [algo, time] : result.times) {
                if (time > 0) {
                    avg_times[algo] += time;
                    counts[algo]++;
                }
            }
        }
        
        log_file << "算法性能平均值(ms):\n";
        for (const auto& [algo, total_time] : avg_times) {
            if (counts[algo] > 0) {
                double avg = total_time / counts[algo];
                log_file << "  " << algo << ": " << fixed << setprecision(6) << avg << " ms\n";
            }
        }
        
        log_file << "\n";
    }
}

// 计算相对速度（按不同基准）
void calculate_relative_speeds(const vector<Result>& results, ofstream& log_file, ofstream& relative_file) {
    // 按类型分组
    vector<Result> small_results, medium_results, large_results;
    
    for (const auto& result : results) {
        if (result.size == GraphSize::SMALL) small_results.push_back(result);
        else if (result.size == GraphSize::MEDIUM) medium_results.push_back(result);
        else large_results.push_back(result);
    }
    
    // 写入相对性能CSV表头
    relative_file << "文件名,节点数,边数,桥数量";
    vector<string> algos = {"Tarjan", "RollbackDSU", "BFS", "NaiveDSU"};
    for (const auto& algo : algos) {
        relative_file << "," << algo << "(ms)";
    }
    relative_file << endl;
    
    // 为每种规模找基准
    unordered_map<string, double> small_base, medium_base, large_base;
    
    // 找到小型图中第一个有所有算法数据的结果作为基准
    for (const auto& result : small_results) {
        bool has_all = true;
        for (const auto& algo : algos) {
            if (!result.times.count(algo) || result.times.at(algo) < 0) {
                has_all = false;
                break;
            }
        }
        if (has_all) {
            for (const auto& algo : algos) {
                small_base[algo] = result.times.at(algo);
            }
            log_file << "小型图基准: " << filesystem::path(result.filename).filename().string() << endl;
            break;
        }
    }
    
    // 中型图基准
    for (const auto& result : medium_results) {
        if (result.times.count("Tarjan") && result.times.at("Tarjan") > 0 &&
            result.times.count("RollbackDSU") && result.times.at("RollbackDSU") > 0) {
            medium_base["Tarjan"] = result.times.at("Tarjan");
            medium_base["RollbackDSU"] = result.times.at("RollbackDSU");
            medium_base["BFS"] = -1;  // 中型图没有BFS数据
            medium_base["NaiveDSU"] = -1;  // 中型图没有朴素DSU数据
            log_file << "中型图基准: " << filesystem::path(result.filename).filename().string() << endl;
            break;
        }
    }
    
    // 大型图基准
    for (const auto& result : large_results) {
        if (result.times.count("Tarjan") && result.times.at("Tarjan") > 0) {
            large_base["Tarjan"] = result.times.at("Tarjan");
            large_base["RollbackDSU"] = -1;  // 大型图没有回滚DSU数据
            large_base["BFS"] = -1;  // 大型图没有BFS数据
            large_base["NaiveDSU"] = -1;  // 大型图没有朴素DSU数据
            log_file << "大型图基准: " << filesystem::path(result.filename).filename().string() << endl;
            break;
        }
    }
    
    // 计算相对性能并写入文件
    for (const auto& result : results) {
        string filename = filesystem::path(result.filename).filename().string();
        unordered_map<string, double> base;
        
        // 选择适当的基准
        if (result.size == GraphSize::SMALL) base = small_base;
        else if (result.size == GraphSize::MEDIUM) base = medium_base;
        else base = large_base;
        
        relative_file << filename << "," << result.nodes << "," << result.edges << "," << result.bridge_count;
        
        for (const auto& algo : algos) {
            if (result.times.count(algo) && result.times.at(algo) > 0 && base.count(algo) && base.at(algo) > 0) {
                double relative = result.times.at(algo) / base.at(algo);
                relative_file << "," << fixed << setprecision(6) << result.times.at(algo);
            } else {
                relative_file << ",N/A";
            }
        }
        relative_file << endl;
    }
    
    // 输出基准信息到日志
    log_file << "\n========== 相对性能基准 ==========\n";
    log_file << "小型图基准时间(ms):\n";
    for (const auto& [algo, time] : small_base) {
        log_file << "  " << algo << ": " << fixed << setprecision(6) << time << " ms\n";
    }
    log_file << "\n中型图基准时间(ms):\n";
    for (const auto& [algo, time] : medium_base) {
        if (time > 0) {
            log_file << "  " << algo << ": " << fixed << setprecision(6) << time << " ms\n";
        }
    }
    log_file << "\n大型图基准时间(ms):\n";
    for (const auto& [algo, time] : large_base) {
        if (time > 0) {
            log_file << "  " << algo << ": " << fixed << setprecision(6) << time << " ms\n";
        }
    }
}

int main(int argc, char* argv[]) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    // 创建日志文件和结果文件
    ofstream log_file("bridge_log.txt");
    ofstream result_file("bridge_results_ms.csv");
    ofstream relative_file("bridge_relative_performance.csv");
    
    if (!log_file || !result_file || !relative_file) {
        cerr << "无法创建日志或结果文件" << endl;
        return 1;
    }
    
    // 记录开始时间
    auto start_time = chrono::system_clock::now();
    time_t start_time_t = chrono::system_clock::to_time_t(start_time);
    log_file << "运行开始时间: " << ctime(&start_time_t) << endl;
    
    // 写入CSV表头
    result_file << "文件名,节点数,边数,桥数量,图类别,Tarjan(ms),回滚DSU(ms),BFS(ms),朴素DSU(ms)" << endl;
    
    vector<Result> all_results;
    
    if (argc == 1) {
        // 没有参数，使用 balanced_graphs 目录作为默认输入
        string dir_path = "balanced_graphs";
        log_file << "未提供参数，使用默认目录: " << dir_path << endl;
        cout << "未提供参数，使用默认目录: " << dir_path << endl;
        
        if (!filesystem::exists(dir_path)) {
            log_file << "错误: 默认目录 " << dir_path << " 不存在" << endl;
            cout << "错误: 默认目录 " << dir_path << " 不存在" << endl;
            return 1;
        }
        
        if (!filesystem::is_directory(dir_path)) {
            log_file << "错误: " << dir_path << " 不是一个目录" << endl;
            cout << "错误: " << dir_path << " 不是一个目录" << endl;
            return 1;
        }
        
        all_results = process_directory(dir_path, log_file);
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
            result.size = determine_graph_size(path, N, M);
            VB bridges = bridge_tarjan();
            result.bridge_count = count(bridges.begin()+1, bridges.end(), true);
            result.times = times;
            
            all_results.push_back(result);
        }
    }
    
    // 打印表格到日志文件
    log_file << "\n\n========== 算法性能比较 ==========\n";
    log_file << setw(30) << "文件名" << " | " 
             << setw(8) << "节点数" << " | " 
             << setw(8) << "边数" << " | " 
             << setw(8) << "桥数量" << " | " 
             << setw(8) << "图类别" << " | " 
             << setw(12) << "Tarjan(ms)" << " | " 
             << setw(12) << "回滚DSU(ms)" << " | " 
             << setw(12) << "BFS(ms)" << " | " 
             << setw(12) << "朴素DSU(ms)" << "\n";
    
    log_file << string(150, '-') << "\n";
    
    // 写入结果到CSV和日志文件
    for (const auto& result : all_results) {
        string filename = filesystem::path(result.filename).filename().string();
        string size_str = result.size == GraphSize::SMALL ? "小型图" : 
                          result.size == GraphSize::MEDIUM ? "中型图" : "大型图";
        
        // 写入日志
        log_file << setw(30) << filename << " | " 
                 << setw(8) << result.nodes << " | " 
                 << setw(8) << result.edges << " | " 
                 << setw(8) << result.bridge_count << " | " 
                 << setw(8) << size_str << " | ";
        
        log_file << setw(12) << fixed << setprecision(6) << result.times.at("Tarjan") << " | ";
        
        if (result.times.count("RollbackDSU") && result.times.at("RollbackDSU") > 0)
            log_file << setw(12) << fixed << setprecision(6) << result.times.at("RollbackDSU") << " | ";
        else
            log_file << setw(12) << "N/A" << " | ";
        
        if (result.times.count("BFS") && result.times.at("BFS") > 0)
            log_file << setw(12) << fixed << setprecision(6) << result.times.at("BFS") << " | ";
        else
            log_file << setw(12) << "N/A" << " | ";
        
        if (result.times.count("NaiveDSU") && result.times.at("NaiveDSU") > 0)
            log_file << setw(12) << fixed << setprecision(6) << result.times.at("NaiveDSU");
        else
            log_file << setw(12) << "N/A";
        
        log_file << "\n";
        
        // 写入CSV
        result_file << filename << "," 
                    << result.nodes << "," 
                    << result.edges << "," 
                    << result.bridge_count << ","
                    << size_str << ",";
        
        result_file << fixed << setprecision(6) << result.times.at("Tarjan") << ",";
        
        if (result.times.count("RollbackDSU") && result.times.at("RollbackDSU") > 0)
            result_file << fixed << setprecision(6) << result.times.at("RollbackDSU") << ",";
        else
            result_file << "N/A,";
        
        if (result.times.count("BFS") && result.times.at("BFS") > 0)
            result_file << fixed << setprecision(6) << result.times.at("BFS") << ",";
        else
            result_file << "N/A,";
        
        if (result.times.count("NaiveDSU") && result.times.at("NaiveDSU") > 0)
            result_file << fixed << setprecision(6) << result.times.at("NaiveDSU");
        else
            result_file << "N/A";
        
        result_file << endl;
    }
    
    // 按类别分析结果
    generate_category_analysis(all_results, log_file);
    
    // 计算相对性能
    calculate_relative_speeds(all_results, log_file, relative_file);
    
    // 记录结束时间
    auto end_time = chrono::system_clock::now();
    time_t end_time_t = chrono::system_clock::to_time_t(end_time);
    log_file << "\n运行结束时间: " << ctime(&end_time_t) << endl;
    
    auto duration = chrono::duration_cast<chrono::seconds>(end_time - start_time);
    log_file << "总运行时间: " << duration.count() << " 秒" << endl;
    
    cout << "\n所有处理完成！结果已保存到以下文件：" << endl;
    cout << "  - bridge_log.txt (详细日志)" << endl;
    cout << "  - bridge_results_ms.csv (算法性能数据，毫秒单位)" << endl;
    cout << "  - bridge_relative_performance.csv (相对性能数据)" << endl;
    
    log_file.close();
    result_file.close();
    relative_file.close();
    
    return 0;
}