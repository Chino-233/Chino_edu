#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <string>
#include <sstream>
#include <chrono>
#include <random>
#include <bitset>
#include <ctime>
#include <queue>
#include <functional>
#include <tuple>

using namespace std;

// 定义最大顶点数和最大颜色数
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

// 生成随机图
unordered_map<int, vector<int>> generateRandomGraph(int nVertices, double edgeProbability) {
    unordered_map<int, vector<int>> graph;
    
    // 初始化顶点
    for (int i = 1; i <= nVertices; ++i) {
        graph[i] = vector<int>();
    }
    
    // 确保图连通
    for (int i = 2; i <= nVertices; ++i) {
        int j = rand() % (i - 1) + 1;  // 连接到前面的随机一个顶点
        graph[i].push_back(j);
        graph[j].push_back(i);
    }
    
    // 随机数生成器
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    
    // 随机添加更多边
    for (int i = 1; i <= nVertices; ++i) {
        for (int j = i + 1; j <= nVertices; ++j) {
            // 避免添加已有的边
            bool edgeExists = false;
            for (int neighbor : graph[i]) {
                if (neighbor == j) {
                    edgeExists = true;
                    break;
                }
            }
            
            if (!edgeExists && dis(gen) < edgeProbability) {
                graph[i].push_back(j);
                graph[j].push_back(i);
            }
        }
    }
    
    return graph;
}

// 创建并保存小规模测试用例
void createSmallExample(const string& filename) {
    std::ofstream file(filename);
    if (!file) {
        cerr << "无法创建文件: " << filename << endl;
        return;
    }
    
    file << "c 小规模测试用例\n";
    file << "p edge 5 9\n";  // 5个顶点，9条边
    file << "e 1 2\n";
    file << "e 1 4\n";
    file << "e 1 5\n";
    file << "e 2 3\n";
    file << "e 2 5\n";
    file << "e 3 4\n";
    file << "e 3 5\n";
    file << "e 4 5\n";
    file << "e 4 1\n";
    
    file.close();
    cout << "创建小规模测试文件: " << filename << endl;
}

// 测试小规模示例
void testSmallExample(const string& filename) {
    cout << "\n===== 测试小规模数据 =====\n";
    
    createSmallExample(filename);
    
    GraphColoring coloring;
    coloring.loadGraphFromFile(filename);
    
    auto startTime = std::chrono::steady_clock::now();
    auto result = coloring.colorGraph(4);  // 尝试用4种颜色
    auto endTime = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    
    cout << "四色填充用时: " << duration << "ms" << endl;
    
    if (!result.empty()) {
        cout << "着色方案:" << endl;
        vector<pair<int, int>> sortedResult(result.begin(), result.end());
        std::sort(sortedResult.begin(), sortedResult.end());
        for (const auto& [vertex, color] : sortedResult) {
            cout << "  顶点 " << vertex << ": 颜色 " << color << endl;
        }
        
        if (coloring.verifyColoring(result)) {
            cout << "验证成功：所有相邻顶点颜色不同" << endl;
        }
        coloring.analyzeColoring(result);
    } else {
        cout << "无法找到四色方案" << endl;
    }
}

// 测试大规模数据
void testLargeExamples() {
    cout << "\n===== 测试大规模数据 =====\n";
    
    vector<pair<string, int>> testFiles = {
        {"d:\\Chino_edu\\suan_temp\\le450_5a.col", 5},
        {"d:\\Chino_edu\\suan_temp\\le450_15b.col", 15},
        {"d:\\Chino_edu\\suan_temp\\le450_25a.col", 25}
    };
    
    for (const auto& [filename, numColors] : testFiles) {
        cout << "\n处理文件: " << filename << ", 尝试用 " << numColors << " 色着色" << endl;
        
        GraphColoring coloring;
        try {
            if (coloring.loadGraphFromFile(filename)) {
                auto startTime = std::chrono::steady_clock::now();
                auto result = coloring.colorGraph(numColors);
                auto endTime = std::chrono::steady_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
                
                cout << "着色用时: " << duration << "ms" << endl;
                
                if (!result.empty()) {
                    if (coloring.verifyColoring(result)) {
                        cout << "验证成功：所有相邻顶点颜色不同" << endl;
                        coloring.analyzeColoring(result);
                    } else {
                        cout << "验证失败：存在相邻顶点颜色相同" << endl;
                    }
                } else {
                    cout << "无法用 " << numColors << " 种颜色着色" << endl;
                }
            }
        } catch (const std::exception& e) {
            cerr << "处理错误: " << e.what() << endl;
        }
    }
}

// 分析算法效率与图规模的关系
void analyzeEfficiency() {
    cout << "\n===== 分析算法效率与图规模的关系 =====\n";
    
    // 测试不同规模的图
    vector<int> sizes = {10, 15, 20, 25, 30, 35};
    vector<double> times;
    
    for (int size : sizes) {
        cout << "测试大小为 " << size << " 的随机图" << endl;
        double edgeProb = 0.3;
        
        // 运行多次取平均值
        const int NUM_RUNS = 3;
        double totalTime = 0;
        int successCount = 0;
        
        for (int run = 0; run < NUM_RUNS; run++) {
            auto graph = generateRandomGraph(size, edgeProb);
            
            int edgeCount = 0;
            for (const auto& [_, neighbors] : graph) {
                edgeCount += neighbors.size();
            }
            edgeCount /= 2;  // 每条边计算了两次
            
            if (run == 0) {
                cout << "生成图有 " << edgeCount << " 条边" << endl;
            }
            
            GraphColoring coloring;
            // 将生成的图结构加载到GraphColoring对象
            for (const auto& [vertex, neighbors] : graph) {
                for (int neighbor : neighbors) {
                    coloring.graph[vertex].push_back(neighbor);
                }
            }
            
            auto startTime = std::chrono::steady_clock::now();
            auto result = coloring.colorGraph(4);  // 尝试用4种颜色
            auto endTime = std::chrono::steady_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
            
            if (!result.empty()) {
                totalTime += duration;
                successCount++;
            }
        }
        
        if (successCount > 0) {
            double avgTime = totalTime / successCount;
            times.push_back(avgTime / 1000.0);  // 转换为秒
            cout << "四色填充平均用时: " << avgTime << "ms, 成功率: " 
                << (double)successCount / NUM_RUNS * 100 << "%" << endl;
        } else {
            times.push_back(0);
            cout << "所有尝试均失败" << endl;
        }
    }
    
    // 输出时间数据，可以用于绘图
    cout << "\n图规模与执行时间关系:" << endl;
    cout << "顶点数,执行时间(秒)" << endl;
    for (size_t i = 0; i < sizes.size(); ++i) {
        cout << sizes[i] << "," << times[i] << endl;
    }
}

int main() {
    // 测试小规模示例
    //testSmallExample("small_example.col");
    
    // 测试大规模数据
    testLargeExamples();
    
    // 分析算法效率
    //analyzeEfficiency();
    
    return 0;
}