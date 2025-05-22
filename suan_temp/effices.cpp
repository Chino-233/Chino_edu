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

    // 优化版的回溯着色算法
    bool backtrackColoringOptimized(vector<int>& vertices, unordered_map<int, int>& colors, 
                                  int numColors, size_t index, vector<bitset<MAX_COLORS>>& available,
                                  vector<unordered_map<int, bool>>& savedAvailability) {
        // 检查是否超时
        auto currentTime = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();
        if (duration > 30) { // 30秒超时，减少等待时间
            return false;
        }

        // 所有顶点已着色
        if (index == vertices.size()) {
            return true;
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
                if (backtrackColoringOptimized(vertices, colors, numColors, index + 1, 
                                            available, savedAvailability)) {
                    return true;
                }

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

        return false;
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

    // 主要的图着色函数
    unordered_map<int, int> colorGraph(int numColors) {
        startTime = std::chrono::steady_clock::now();
        cout << "开始对 " << graph.size() << " 个顶点的图进行 " << numColors << " 种颜色的着色..." << endl;
        
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
        
        // 尝试贪心着色法预先填充部分顶点
        for (int vertex : vertices) {
            for (int color = 1; color <= numColors; ++color) {
                if (isSafe(vertex, color, colors)) {
                    colors[vertex] = color;
                    colorUsageCount[color-1]++;
                    break;
                }
            }
        }
        
        // 获取未着色的顶点
        vector<int> uncoloredVertices;
        for (int vertex : vertices) {
            if (colors.count(vertex) == 0) {
                uncoloredVertices.push_back(vertex);
            } else {
                // 更新已着色顶点的邻居可用颜色
                int color = colors[vertex];
                for (int neighbor : graph[vertex]) {
                    available[neighbor][color-1] = false;
                }
            }
        }
        
        // 如果有未着色的顶点，使用回溯法
        bool success = true;
        if (!uncoloredVertices.empty()) {
            colors.clear(); // 清除贪心结果，从头开始回溯
            colorUsageCount.assign(numColors, 0);
            
            // 重置可用颜色
            for (const auto& [vertex, _] : graph) {
                available[vertex].set();
            }
            
            success = backtrackColoringOptimized(vertices, colors, numColors, 0, 
                                               available, savedAvailability);
        }
        
        auto endTime = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
        
        if (success) {
            cout << "成功找到着色方案，用时: " << duration << "ms" << endl;
            return colors;
        } else {
            cout << "无法找到使用 " << numColors << " 种颜色的着色方案，用时: " << duration << "ms" << endl;
            return {};
        }
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
    
    // 在固定时间内运行多次，取最佳结果
    const int NUM_RUNS = 3;
    double bestSuccessTime = numeric_limits<double>::max();
    bool anySuccess = false;
    
    for (int run = 0; run < NUM_RUNS; run++) {
        auto startTime = std::chrono::steady_clock::now();
        auto result = coloring.colorGraph(4);  // 四色填充
        auto endTime = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
        
        if (!result.empty() && coloring.verifyColoring(result)) {
            bestSuccessTime = min(bestSuccessTime, static_cast<double>(duration));
            anySuccess = true;
        }
    }
    
    // 写入结果到文件
    if (anySuccess) {
        outFile << vertices << "," << edgeCount << "," << density << "," 
                << fixed << setprecision(3) << bestSuccessTime / 1000.0 << endl;
        cout << "  顶点: " << vertices << ", 边: " << edgeCount 
             << ", 密度: " << density << "%"
             << ", 用时: " << bestSuccessTime << "ms" << endl;
    } else {
        outFile << vertices << "," << edgeCount << "," << density << ",timeout" << endl;
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
    outFile << "顶点数,边数,密度(%),执行时间(秒)" << endl;
    
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

int main() {
    cout << "开始分析随机图的算法效率..." << endl;
    
    // 分析所有生成的随机图
    analyzeAllGraphs();
    
    return 0;
}