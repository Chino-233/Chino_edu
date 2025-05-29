#include <bits/stdc++.h>
using namespace std;
const int MAX_VERTICES = 1000;
const int MAX_COLORS = 30;
class GraphColoring {
public:
    // 使用邻接表表示图
    unordered_map<int, vector<int>> graph;
    std::chrono::steady_clock::time_point startTime;
    
    // 用于剪枝时计算的颜色使用频率
    vector<int> colorUsageCount;
    
    // 存储每个顶点的度数和饱和度
    unordered_map<int, int> vertexDegree;
    unordered_map<int, int> saturationDegree;
    
    // 存储所有找到的解决方案的数量(而不是解决方案本身)
    // vector<unordered_map<int, int>> allSolutions; // 注释掉
    int solutionCount = 0; // 新增：只记录解的数量
    
    long long exploredCombinations = 0; // 已探索的组合数
    double totalCombinations = 0;       // 估算的总组合数
    int maxColors = 0;                  // 最大颜色数量

    // 检查颜色是否可用于顶点
    bool isSafe(int vertex, int color, const unordered_map<int, int>& colors) {
        for (int neighbor : graph[vertex]) {
            if (colors.count(neighbor) > 0 && colors.at(neighbor) == color) {
                return false;
            }
        }
        return true;
    }

    // 前向检查：检查分配此颜色后邻居是否还有可用颜色
    bool forwardCheck(int vertex, int color, vector<bitset<MAX_COLORS>>& available) {
        for (int neighbor : graph[vertex]) {
            if (available[neighbor].count() == 1 && available[neighbor][color-1]) {
                return false;
            }
        }
        return true;
    }

    // 更新邻居的可用颜色
    void updateNeighborsAvailability(int vertex, int color, vector<bitset<MAX_COLORS>>& available, 
                                    vector<unordered_map<int, bool>>& savedAvailability) {
        for (int neighbor : graph[vertex]) {
            if (available[neighbor][color-1]) {
                savedAvailability[color-1][neighbor] = true;
                available[neighbor][color-1] = false;
            }
        }
    }

    // 恢复邻居的可用颜色
    void restoreNeighborsAvailability(int color, vector<bitset<MAX_COLORS>>& available, 
                                      vector<unordered_map<int, bool>>& savedAvailability) {
        for (auto& [neighbor, wasAvailable] : savedAvailability[color-1]) {
            if (wasAvailable) {
                available[neighbor][color-1] = true;
            }
        }
        savedAvailability[color-1].clear();
    }

    // 获取顶点的度数
    int getDegree(int vertex) {
        return graph[vertex].size();
    }
    
    // 计算顶点的饱和度（邻居中不同颜色的数量）
    int calculateSaturation(int vertex, const unordered_map<int, int>& colors) {
        unordered_set<int> neighborColors;
        for (int neighbor : graph[vertex]) {
            if (colors.count(neighbor) > 0) {
                neighborColors.insert(colors.at(neighbor));
            }
        }
        return neighborColors.size();
    }

    // 结合度数和饱和度，按优先级选择下一个顶点
    int selectNextVertex(const vector<int>& uncolored, 
                        vector<bitset<MAX_COLORS>>& available, 
                        const unordered_map<int, int>& colors) {
        int bestIdx = 0;
        int maxSaturation = -1;
        int maxDegree = -1;
        int minAvailable = MAX_COLORS + 1;
        
        for (size_t i = 0; i < uncolored.size(); ++i) {
            int vertex = uncolored[i];
            int saturation = calculateSaturation(vertex, colors);
            int degree = getDegree(vertex);
            int availableCount = available[vertex].count();
            
            // 优先选择饱和度高的顶点
            if (saturation > maxSaturation) {
                maxSaturation = saturation;
                maxDegree = degree;
                minAvailable = availableCount;
                bestIdx = i;
            } 
            // 饱和度相同，选择可用颜色数最少的顶点
            else if (saturation == maxSaturation && availableCount < minAvailable) {
                minAvailable = availableCount;
                maxDegree = degree;
                bestIdx = i;
            }
            // 饱和度和可用颜色数相同，选择度数最大的顶点
            else if (saturation == maxSaturation && availableCount == minAvailable && degree > maxDegree) {
                maxDegree = degree;
                bestIdx = i;
            }
        }
        
        return bestIdx;
    }

    // 显示进度
    void displayProgress() {
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();
        
        cout << "\r找到 " << solutionCount << " 个解，已用时: " << elapsedTime << "秒" << std::flush;
    }
    
    // 修改后的回溯着色算法 - 只记录解的数量
    void backtrackColoringAllSolutions(vector<int>& vertices, unordered_map<int, int>& colors, 
                                  int numColors, size_t index, vector<bitset<MAX_COLORS>>& available,
                                  vector<unordered_map<int, bool>>& savedAvailability,
                                  int& solutionLimit) {
        // 检查是否超时或达到解的数量限制
        auto currentTime = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();
        if (duration > 6000 || (solutionLimit > 0 && solutionCount >= solutionLimit)) { // 60秒超时
            return;
        }
        
        // 所有顶点已着色，找到一个解
        if (index == vertices.size()) {
            // allSolutions.push_back(colors); // 注释掉，不再保存解
            solutionCount++; // 只增加计数
            //displayProgress();
            return;
        }

        // 更新探索进度
        exploredCombinations++;
        if (exploredCombinations % 10000 == 0) {
            //displayProgress();
        }

        // 动态选择下一个顶点
        int nextIdx = selectNextVertex(vector<int>(vertices.begin() + index, vertices.end()), 
                                     available, colors);
        if (nextIdx != 0) {
            std::swap(vertices[index], vertices[index + nextIdx]);
        }

        int vertex = vertices[index];
        
        // 获取已使用的颜色集合，用于等效冗余优化
        unordered_set<int> usedColors;
        for (int i = 0; i < index; ++i) {
            if (colors.count(vertices[i]) > 0) {
                usedColors.insert(colors[vertices[i]]);
            }
        }

        // 为当前顶点选择颜色
        vector<int> colorOrder(numColors);
        for (int i = 0; i < numColors; ++i) {
            colorOrder[i] = i + 1;
        }
        
        // 根据颜色使用频率排序
        std::sort(colorOrder.begin(), colorOrder.end(), 
                 [&](int a, int b) { return colorUsageCount[a-1] < colorUsageCount[b-1]; });

        // 先尝试已使用的颜色
        for (int color : colorOrder) {
            // 排除等效冗余优化：如果此颜色未被使用过，只尝试一种新颜色
            if (usedColors.count(color) == 0 && !usedColors.empty() && color != colorOrder[0]) {
                continue;
            }
            
            if (available[vertex][color-1] && isSafe(vertex, color, colors)) {
                // 前向检查优化
                if (!forwardCheck(vertex, color, available)) {
                    continue;
                }

                // 分配颜色
                colors[vertex] = color;
                colorUsageCount[color-1]++;
                
                // 更新邻居可用颜色
                updateNeighborsAvailability(vertex, color, available, savedAvailability);

                // 递归处理下一个顶点
                backtrackColoringAllSolutions(vertices, colors, numColors, index + 1, 
                                           available, savedAvailability, solutionLimit);

                // 回溯
                colors.erase(vertex);
                colorUsageCount[color-1]--;
                
                // 恢复邻居可用颜色
                restoreNeighborsAvailability(color, available, savedAvailability);
            }
        }

        // 恢复顶点顺序
        if (nextIdx != 0) {
            std::swap(vertices[index], vertices[index + nextIdx]);
        }
    }


    GraphColoring() {}
    
    // 从文件加载图
    bool loadGraphFromFile(const string& filename) {
        std::ifstream file(filename);
        if (!file) {
            cerr << "无法打开文件: " << filename << endl;
            return false;
        }

        string line;
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == 'c') {
                continue;  // 跳过注释行
            }
            if (line[0] == 'p') {
                continue;  // 跳过问题描述行
            }
            if (line[0] == 'e') {
                std::istringstream iss(line);
                char e;
                int v1, v2;
                if (!(iss >> e >> v1 >> v2)) {
                    continue;
                }
                graph[v1].push_back(v2);
                graph[v2].push_back(v1);
            }
        }

        // 预计算每个顶点的度数
        for (const auto& [vertex, neighbors] : graph) {
            vertexDegree[vertex] = neighbors.size();
        }

        cout << "图已加载: " << graph.size() << " 个顶点, " 
             << countEdges() << " 条边" << endl;
        return true;
    }

    // 计算图中的边数
    int countEdges() const {
        int count = 0;
        for (const auto& [_, neighbors] : graph) {
            count += neighbors.size();
        }
        return count / 2;  // 每条边计算了两次
    }

    // 估算搜索空间大小
    void estimateTotalCombinations(int numColors) {
        // 简单估算: numColors^(顶点数)，实际搜索空间可能小很多
        totalCombinations = pow(numColors, min(20, static_cast<int>(graph.size())));
        maxColors = numColors;
    }

    // 修改的图着色函数 - 只返回解的数量
    int colorGraphAllSolutions(int numColors, int solutionLimit = 0) {
        startTime = std::chrono::steady_clock::now();
        // allSolutions.clear(); // 注释掉
        solutionCount = 0; // 重置计数器
        exploredCombinations = 0;
        estimateTotalCombinations(numColors);
        
        cout << "开始对 " << graph.size() << " 个顶点的图进行 " << numColors 
             << " 种颜色的着色，寻找所有解..." << endl;
        
        // 按度数降序排序顶点（初始排序）
        vector<int> vertices;
        for (const auto& [vertex, _] : graph) {
            vertices.push_back(vertex);
        }
        
        std::sort(vertices.begin(), vertices.end(), [&](int a, int b) {
            return vertexDegree[a] > vertexDegree[b];
        });
        
        // 初始化结果
        unordered_map<int, int> colors;
        
        // 初始化颜色可用性位图
        vector<bitset<MAX_COLORS>> available(MAX_VERTICES);
        for (const auto& [vertex, _] : graph) {
            available[vertex].set();  // 所有颜色初始都可用
        }
        
        // 初始化颜色使用频率计数
        colorUsageCount.resize(numColors, 0);
        
        // 用于保存回溯时需要恢复的状态
        vector<unordered_map<int, bool>> savedAvailability(MAX_COLORS);
        
        // 使用回溯法寻找所有解
        backtrackColoringAllSolutions(vertices, colors, numColors, 0, 
                                    available, savedAvailability, solutionLimit);
        
        auto endTime = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
        
        cout << "\n找到 " << solutionCount << " 个解决方案，用时: " << duration << "ms" << endl;
        
        return solutionCount; // 返回解的数量而不是解本身
    }

    // 验证着色结果
    bool verifyColoring(const unordered_map<int, int>& coloring) {
        if (coloring.empty()) {
            return false;
        }

        for (const auto& [vertex, _] : graph) {
            if (coloring.count(vertex) == 0) {
                cerr << "错误：顶点 " << vertex << " 未着色!" << endl;
                return false;
            }
            
            for (int neighbor : graph[vertex]) {
                if (coloring.count(neighbor) > 0 && coloring.at(vertex) == coloring.at(neighbor)) {
                    cerr << "错误：顶点 " << vertex << " 和 " << neighbor 
                         << " 颜色相同 (" << coloring.at(vertex) << ")!" << endl;
                    return false;
                }
            }
        }
        return true;
    }

    // 分析着色结果
    void analyzeColoring(const unordered_map<int, int>& coloring) {
        if (coloring.empty()) {
            return;
        }

        // 计算使用的颜色数量
        unordered_set<int> usedColors;
        for (const auto& [_, color] : coloring) {
            usedColors.insert(color);
        }

        cout << "使用了 " << usedColors.size() << " 种不同的颜色" << endl;

        // 统计每种颜色的使用频率
        unordered_map<int, int> colorCounts;
        for (const auto& [_, color] : coloring) {
            colorCounts[color]++;
        }

        cout << "颜色分布:" << endl;
        vector<pair<int, int>> sortedCounts(colorCounts.begin(), colorCounts.end());
        std::sort(sortedCounts.begin(), sortedCounts.end());
        for (const auto& [color, count] : sortedCounts) {
            cout << "  颜色 " << color << ": " << count << " 个顶点" << endl;
        }
    }
};

// 直接包含原来的GraphColoring类定义
// ...此处应包含GraphColoring类的所有代码...
// 为了简便，我假设你会复制原文件中的GraphColoring类实现

using namespace std;
namespace fs = std::filesystem;

// 分析单个图文件的性能
void analyzeGraphFile(const string& filename, ofstream& outFile) {
    cout << "分析文件: " << filename << endl;
    
    // 提取顶点数信息(从文件名)
    string basename = fs::path(filename).filename().string();
    int vertices = 0;
    int density = 0;
    
    // 从文件名解析信息 (假设文件名格式为 random_vertices_density.col)
    sscanf(basename.c_str(), "planar_%d_%d.col", &vertices, &density);
    
    // 加载图并计时
    GraphColoring coloring;
    if (!coloring.loadGraphFromFile(filename)) {
        cerr << "无法加载图文件: " << filename << endl;
        return;
    }
    
    int edgeCount = coloring.countEdges();
    
    // 设置解决方案数量限制
    const int SOLUTION_LIMIT = 1000000000; // 最多找10000个解
    
    // 寻找所有解并计时
    auto startTime = std::chrono::steady_clock::now();
    int solutionCount = coloring.colorGraphAllSolutions(5, SOLUTION_LIMIT);  // 四色填充，设置解的数量上限
    auto endTime = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    
    // 写入结果到文件
    if (solutionCount > 0) {
        outFile << vertices << "," << edgeCount << "," << density << "," 
                << fixed << setprecision(3) << duration / 1000.0 << "," << solutionCount << endl;
        cout << "  顶点: " << vertices << ", 边: " << edgeCount 
             << ", 密度: " << density << "%"
             << ", 用时: " << duration << "ms"
             << ", 找到: " << solutionCount << " 个解" << endl;
    } else {
        outFile << vertices << "," << edgeCount << "," << density << ",timeout,0" << endl;
        cout << "  顶点: " << vertices << ", 边: " << edgeCount 
             << ", 密度: " << density << "%"
             << ", 结果: 超时或无解" << endl;
    }
}

// 分析所有随机图并生成效率报告
void analyzeAllGraphs() {
    string inputDir = "d:\\Chino_edu\\suan_temp\\random_graphs\\";
    string outputFile = "d:\\Chino_edu\\suan_temp\\efficiency_results.txt";
    
    // 打开输出文件
    ofstream outFile(outputFile);
    if (!outFile) {
        cerr << "无法创建输出文件: " << outputFile << endl;
        return;
    }
    
    // 写入表头
    outFile << "顶点数,边数,密度(%),执行时间(秒),解的数量" << endl;
    
    // 遍历目录中的所有文件
    vector<string> files;
    try {
        for (const auto& entry : fs::directory_iterator(inputDir)) {
            if (entry.is_regular_file() && entry.path().extension() == ".col") {
                files.push_back(entry.path().string());
            }
        }
    } catch (const fs::filesystem_error& e) {
        cerr << "文件系统错误: " << e.what() << endl;
        return;
    }
    
    // 按文件名排序（确保按顶点数递增顺序）
    sort(files.begin(), files.end());
    
    // 分析每个文件
    for (const string& file : files) {
        analyzeGraphFile(file, outFile);
    }
    
    outFile.close();
    cout << "分析完成，结果已保存到: " << outputFile << endl;
}

// 测试单个图文件并找到所有解
void testSingleGraph(const string& filename) {
    cout << "测试文件: " << filename << endl;
    
    // 加载图
    GraphColoring coloring;
    if (!coloring.loadGraphFromFile(filename)) {
        cerr << "无法加载图文件: " << filename << endl;
        return;
    }
    
    // 设置解决方案数量限制（0表示不限制）
    const int SOLUTION_LIMIT = 0;
    
    // 寻找所有解
    int solutionCount = coloring.colorGraphAllSolutions(5, SOLUTION_LIMIT);  // 四色填充
    
    cout << "找到 " << solutionCount << " 个有效解" << endl;
    
    // 由于不再存储解，以下部分需要注释掉或修改
    /*
    // 验证所有解
    int validCount = 0;
    for (const auto& solution : solutions) {
        if (coloring.verifyColoring(solution)) {
            validCount++;
        }
    }
    
    cout << "验证结果: " << validCount << "/" << solutions.size() << " 个有效解" << endl;
    
    // 分析第一个解（如果有）
    if (!solutions.empty()) {
        cout << "\n第一个解的分析:" << endl;
        coloring.analyzeColoring(solutions[0]);
    }
    */
}

int main() {
    analyzeAllGraphs();
    /*
    string filename;
    cout << "请输入要分析的图文件路径(输入'all'分析所有图): ";
    getline(cin, filename);
    
    if (filename == "all") {
        cout << "开始分析所有随机图的算法效率..." << endl;
        analyzeAllGraphs();
    } else {
        testSingleGraph(filename);
    }
    */
    return 0;
}