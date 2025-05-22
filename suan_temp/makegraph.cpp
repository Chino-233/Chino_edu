#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <algorithm>
#include <string>
#include <set>
#include <map>
#include <ctime>
#include <queue>
#include <cmath>

using namespace std;

// 二维点结构
struct Point {
    double x, y;
    int id;
    
    Point(double _x = 0, double _y = 0, int _id = 0) : x(_x), y(_y), id(_id) {}
    
    bool operator<(const Point& other) const {
        return x < other.x || (x == other.x && y < other.y);
    }
    
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

// 边结构
struct Edge {
    int u, v;
    double weight;
    
    Edge(int _u, int _v, double _weight = 0.0) {
        // 保证u < v
        if (_u < _v) {
            u = _u;
            v = _v;
        } else {
            u = _v;
            v = _u;
        }
        weight = _weight;
    }
    
    bool operator<(const Edge& other) const {
        if (u != other.u) return u < other.u;
        return v < other.v;
    }
    
    bool operator==(const Edge& other) const {
        return u == other.u && v == other.v;
    }
};

// 三角形结构
struct Triangle {
    int a, b, c; // 顶点索引
    
    Triangle(int _a, int _b, int _c) : a(_a), b(_b), c(_c) {}
    
    bool containsVertex(int v) const {
        return a == v || b == v || c == v;
    }
    
    bool sharesEdgeWith(const Triangle& other) const {
        int sharedVertices = 0;
        if (other.containsVertex(a)) sharedVertices++;
        if (other.containsVertex(b)) sharedVertices++;
        if (other.containsVertex(c)) sharedVertices++;
        return sharedVertices == 2; // 共享一条边意味着有两个共同顶点
    }
    bool operator==(const Triangle& other) const {
        // 检查三角形是否有相同的顶点（不考虑顶点顺序）
        return (a == other.a && b == other.b && c == other.c) ||
               (a == other.a && b == other.c && c == other.b) ||
               (a == other.b && b == other.a && c == other.c) ||
               (a == other.b && b == other.c && c == other.a) ||
               (a == other.c && b == other.a && c == other.b) ||
               (a == other.c && b == other.b && c == other.a);
    }
};

// 计算两点之间的欧氏距离
double distance(const Point& p1, const Point& p2) {
    return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

// 计算三角形面积的两倍 (用于判断点是否在三角形内部)
double triangleArea2(const Point& a, const Point& b, const Point& c) {
    return abs((a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y)));
}

// 判断点p是否在三角形abc内部
bool pointInTriangle(const Point& p, const Point& a, const Point& b, const Point& c) {
    // 使用重心坐标判断
    double area = triangleArea2(a, b, c);
    double s1 = triangleArea2(p, b, c) / area;
    double s2 = triangleArea2(a, p, c) / area;
    double s3 = triangleArea2(a, b, p) / area;
    
    // 对于数值精度问题，使用小的容差
    const double EPSILON = 1e-10;
    return s1 >= -EPSILON && s2 >= -EPSILON && s3 >= -EPSILON && 
           s1 <= 1 + EPSILON && s2 <= 1 + EPSILON && s3 <= 1 + EPSILON &&
           abs(s1 + s2 + s3 - 1.0) < EPSILON;
}

// 判断两条线段是否相交
bool segmentsIntersect(const Point& p1, const Point& p2, const Point& p3, const Point& p4) {
    // 线段p1-p2和p3-p4是否相交
    
    // 计算叉积，判断方向
    auto direction = [](const Point& pi, const Point& pj, const Point& pk) -> double {
        return (pk.x - pi.x) * (pj.y - pi.y) - (pj.x - pi.x) * (pk.y - pi.y);
    };
    
    // 判断点pk是否在线段pi-pj上
    auto onSegment = [](const Point& pi, const Point& pj, const Point& pk) -> bool {
        return (min(pi.x, pj.x) <= pk.x && pk.x <= max(pi.x, pj.x) &&
                min(pi.y, pj.y) <= pk.y && pk.y <= max(pi.y, pj.y));
    };
    
    double d1 = direction(p3, p4, p1);
    double d2 = direction(p3, p4, p2);
    double d3 = direction(p1, p2, p3);
    double d4 = direction(p1, p2, p4);
    
    // 如果两条线段相交，那么p1和p2在p3-p4的两侧，p3和p4在p1-p2的两侧
    if (((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0)) &&
        ((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0)))
        return true;
    
    // 处理线段端点重合或一点在另一线段上的情况
    if (d1 == 0 && onSegment(p3, p4, p1)) return true;
    if (d2 == 0 && onSegment(p3, p4, p2)) return true;
    if (d3 == 0 && onSegment(p1, p2, p3)) return true;
    if (d4 == 0 && onSegment(p1, p2, p4)) return true;
    
    return false;
}

// 检查添加边是否会导致图不再是平面图
bool willCrossExistingEdge(const Point& p1, const Point& p2, 
                           const vector<Point>& points, const set<Edge>& edges) {
    for (const Edge& e : edges) {
        // 跳过共享端点的边
        if (e.u == p1.id || e.u == p2.id || e.v == p1.id || e.v == p2.id) continue;
        
        // 检查两条边是否相交
        if (segmentsIntersect(p1, p2, points[e.u], points[e.v])) {
            return true;
        }
    }
    return false;
}

// 使用Delaunay三角剖分生成平面图
set<Edge> generateDelaunayTriangulation(vector<Point>& points) {
    int n = points.size();
    if (n <= 2) {
        set<Edge> edges;
        if (n == 2) {
            edges.insert(Edge(0, 1));
        }
        return edges;
    }
    
    // 对点进行排序以便处理
    sort(points.begin(), points.end(), [](const Point& a, const Point& b) {
        return a.x < b.x || (a.x == b.x && a.y < b.y);
    });
    
    // 更新点ID
    for (int i = 0; i < n; i++) {
        points[i].id = i;
    }
    
    // 创建初始三角形（足够大以包含所有点）
    double minX = points[0].x, minY = points[0].y;
    double maxX = points[0].x, maxY = points[0].y;
    
    for (const Point& p : points) {
        minX = min(minX, p.x);
        minY = min(minY, p.y);
        maxX = max(maxX, p.x);
        maxY = max(maxY, p.y);
    }
    
    // 边界扩展
    double deltaX = (maxX - minX) * 10;
    double deltaY = (maxY - minY) * 10;
    
    vector<Point> superPoints = {
        Point(minX - deltaX, minY - deltaY, n),
        Point(maxX + deltaX, minY - deltaY, n + 1),
        Point((minX + maxX) / 2, maxY + deltaY, n + 2)
    };
    
    // 超级三角形
    Triangle superTriangle(n, n + 1, n + 2);
    vector<Triangle> triangles = {superTriangle};
    
    // 逐点添加到三角剖分中
    for (int i = 0; i < n; i++) {
        const Point& p = points[i];
        vector<Edge> edges; // 用于临时存储待处理的边
        
        // 找出所有包含点p的三角形
        vector<Triangle> badTriangles;
        for (const Triangle& t : triangles) {
            Point a = (t.a < n) ? points[t.a] : superPoints[t.a - n];
            Point b = (t.b < n) ? points[t.b] : superPoints[t.b - n];
            Point c = (t.c < n) ? points[t.c] : superPoints[t.c - n];
            
            if (pointInTriangle(p, a, b, c)) {
                badTriangles.push_back(t);
            }
        }
        
        // 找出多边形边界
        for (size_t j = 0; j < badTriangles.size(); j++) {
            const Triangle& t = badTriangles[j];
            // 检查每条边是否是多边形边界
            vector<pair<int, int>> edgePairs = {
                {t.a, t.b}, {t.b, t.c}, {t.c, t.a}
            };
            
            for (const auto& [u, v] : edgePairs) {
                bool isShared = false;
                for (size_t k = 0; k < badTriangles.size(); k++) {
                    if (j == k) continue;
                    const Triangle& other = badTriangles[k];
                    if ((other.a == u && other.b == v) || (other.a == v && other.b == u) || 
                        (other.b == u && other.c == v) || (other.b == v && other.c == u) || 
                        (other.c == u && other.a == v) || (other.c == v && other.a == u)) {
                        isShared = true;
                        break;
                    }
                }
                if (!isShared) {
                    edges.push_back(Edge(u, v));
                }
            }
        }
        
        // 从三角形列表中移除包含点p的三角形
        triangles.erase(
            remove_if(triangles.begin(), triangles.end(), 
                  [&badTriangles](const Triangle& t) {
                      return find(badTriangles.begin(), badTriangles.end(), t) != badTriangles.end();
                  }),
            triangles.end()
        );
        
        // 创建新三角形
        for (const Edge& e : edges) {
            triangles.push_back(Triangle(e.u, e.v, i));
        }
    }
    
    // 删除包含超级三角形顶点的所有三角形
    triangles.erase(
        remove_if(triangles.begin(), triangles.end(), 
              [&n](const Triangle& t) {
                  return t.a >= n || t.b >= n || t.c >= n;
              }),
        triangles.end()
    );
    
    // 提取所有唯一边
    set<Edge> finalEdges;
    for (const Triangle& t : triangles) {
        finalEdges.insert(Edge(t.a, t.b));
        finalEdges.insert(Edge(t.b, t.c));
        finalEdges.insert(Edge(t.c, t.a));
    }
    
    return finalEdges;
}

// 检查图是否连通
bool isConnected(int n, const vector<vector<int>>& graph) {
    vector<bool> visited(n, false);
    queue<int> q;
    q.push(0);
    visited[0] = true;
    int count = 1;
    
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        
        for (int v : graph[u]) {
            if (!visited[v]) {
                visited[v] = true;
                q.push(v);
                count++;
            }
        }
    }
    
    return count == n;
}

// 生成包含指定边数的平面图
bool generatePlanarGraph(int nVertices, int nEdges, const string& filename) {
    // 验证参数
    int minEdges = nVertices - 1; // 连通图的最小边数
    int maxEdges = 3 * nVertices - 6; // 平面图的最大边数（欧拉公式）
    
    if (nVertices <= 2) {
        cerr << "错误：顶点数必须大于2!" << endl;
        return false;
    }
    
    if (nEdges < minEdges) {
        cerr << "警告：边数 " << nEdges << " 小于保持连通性的最小边数 " << minEdges 
             << "，将使用最小边数" << endl;
        nEdges = minEdges;
    }
    
    if (nEdges > maxEdges) {
        cerr << "警告：边数 " << nEdges << " 超过了平面图的最大可能边数 " << maxEdges
             << "，将使用最大边数" << endl;
        nEdges = maxEdges;
    }
    
    // 初始化随机数生成器
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 1000.0);
    
    // 使用泊松盘采样生成均匀分布的点（避免点过于聚集）
    vector<Point> points;
    double MIN_DIST = 30.0; // 最小点间距
    
    // 尝试生成点，确保它们足够分散
    for (int i = 0; i < nVertices; i++) {
        Point newPoint(dis(gen), dis(gen), i);
        bool valid = true;
        
        // 检查与现有点的距离
        for (const Point& p : points) {
            if (distance(p, newPoint) < MIN_DIST) {
                valid = false;
                break;
            }
        }
        
        if (valid) {
            points.push_back(newPoint);
        } else {
            // 如果点太近，重新生成
            i--;
            // 避免无限循环
            if (i < -100) {
                // 放宽限制
                i = -1;
                MIN_DIST /= 2;
            }
        }
    }
    
    // 使用Delaunay三角剖分创建初始平面图
    set<Edge> edges = generateDelaunayTriangulation(points);
    
    // 转换为邻接表以便处理
    vector<vector<int>> adjList(nVertices);
    for (const Edge& e : edges) {
        if (e.u < nVertices && e.v < nVertices) { // 确保不包含超级三角形顶点
            adjList[e.u].push_back(e.v);
            adjList[e.v].push_back(e.u);
        }
    }
    
    // 检查生成的图是否连通
    if (!isConnected(nVertices, adjList)) {
        // 如果不连通，使用生成树确保连通性
        vector<bool> visited(nVertices, false);
        vector<Edge> mstEdges;
        priority_queue<pair<double, Edge>, vector<pair<double, Edge>>, greater<>> pq;
        
        // 从顶点0开始
        visited[0] = true;
        
        // 添加与顶点0相连的所有边
        for (int v = 0; v < nVertices; v++) {
            if (v != 0) {
                double w = distance(points[0], points[v]);
                pq.push({w, Edge(0, v, w)});
            }
        }
        
        // Prim算法构建最小生成树
        while (!pq.empty() && mstEdges.size() < nVertices - 1) {
            auto [w, e] = pq.top();
            pq.pop();
            
            int u = e.u;
            int v = e.v;
            
            if (visited[u] && visited[v]) continue;
            
            int next = visited[u] ? v : u;
            visited[next] = true;
            mstEdges.push_back(e);
            
            // 添加与next相连的未访问顶点的边
            for (int neighbor = 0; neighbor < nVertices; neighbor++) {
                if (!visited[neighbor]) {
                    double w = distance(points[next], points[neighbor]);
                    pq.push({w, Edge(next, neighbor, w)});
                }
            }
        }
        
        // 添加MST边确保连通性
        for (const Edge& e : mstEdges) {
            edges.insert(e);
            adjList[e.u].push_back(e.v);
            adjList[e.v].push_back(e.u);
        }
    }
    
    // 如果边不足，尝试添加更多的边
    if (edges.size() < nEdges) {
        // 获取所有可能的边
        vector<Edge> possibleEdges;
        for (int i = 0; i < nVertices; i++) {
            for (int j = i + 1; j < nVertices; j++) {
                Edge e(i, j);
                // 如果边已存在，跳过
                if (edges.find(e) != edges.end()) continue;
                
                // 检查添加这条边是否会使图变得不平面
                if (!willCrossExistingEdge(points[i], points[j], points, edges)) {
                    possibleEdges.push_back(e);
                }
            }
        }
        
        // 随机打乱可能的边
        shuffle(possibleEdges.begin(), possibleEdges.end(), gen);
        
        // 添加边直到达到目标数量或没有更多可添加的边
        for (const Edge& e : possibleEdges) {
            if (edges.size() >= nEdges) break;
            
            edges.insert(e);
            adjList[e.u].push_back(e.v);
            adjList[e.v].push_back(e.u);
        }
    }
    
    // 确保我们没有超出最大边数
    if (edges.size() > nEdges) {
        // 将边转换为向量以便随机选择
        vector<Edge> edgeVector(edges.begin(), edges.end());
        
        // 移除不会破坏连通性的边
        shuffle(edgeVector.begin(), edgeVector.end(), gen);
        
        for (auto it = edgeVector.begin(); it != edgeVector.end() && edges.size() > nEdges; ) {
            Edge e = *it;
            
            // 临时移除边
            edges.erase(e);
            
            // 检查移除后是否仍然连通
            vector<vector<int>> tempAdjList(nVertices);
            for (const Edge& remaining : edges) {
                tempAdjList[remaining.u].push_back(remaining.v);
                tempAdjList[remaining.v].push_back(remaining.u);
            }
            
            if (!isConnected(nVertices, tempAdjList)) {
                // 如果不连通，恢复这条边
                edges.insert(e);
                ++it;
            } else {
                // 如果仍然连通，永久移除这条边
                it = edgeVector.erase(it);
            }
        }
    }
    
    // 写入文件
    ofstream file(filename);
    if (!file) {
        cerr << "无法创建文件: " << filename << endl;
        return false;
    }
    
    // 写入文件头
    file << "c 平面图 - 顶点数: " << nVertices << "\n";
    file << "c 边数: " << edges.size() << "\n";
    file << "c 生成时间: " << time(nullptr) << "\n";
    file << "p edge " << nVertices << " " << edges.size() << "\n";
    
    // 写入边数据（顶点编号从1开始）
    for (const auto& edge : edges) {
        file << "e " << (edge.u + 1) << " " << (edge.v + 1) << "\n";
    }
    
    file.close();
    cout << "成功生成平面图，保存至: " << filename << endl;
    cout << "  顶点数: " << nVertices << ", 边数: " << edges.size() << endl;
    cout << "  边密度: " << (double)edges.size() / (nVertices * (nVertices - 1) / 2) * 100 << "%" << endl;
    
    return true;
}

// 生成一系列不同规模的平面图
void generatePlanarGraphTestCases() {
    string baseDir = "d:\\Chino_edu\\suan_temp\\planar_graphs\\";
    
    // 定义不同的顶点数和对应的边数
    vector<pair<int, int>> cases = {
        {10, 20},
        {20, 40},
        {30, 60},
        {40, 80},
        {50, 100},
        {60, 120},
        {70, 140},
        {80, 160}
    };
    
    cout << "开始生成平面图...\n" << endl;
    
    for (const auto& [vertices, edges] : cases) {
        string filename = baseDir + "planar_" + to_string(vertices) + "_" + to_string(edges) + ".col";
        cout << "生成平面图: 顶点数=" << vertices << ", 边数=" << edges << endl;
        generatePlanarGraph(vertices, edges, filename);
    }
}

int main() {
    cout << "===== 平面图生成器 =====" << endl;
    
    // 创建输出目录
    system("mkdir d:\\Chino_edu\\suan_temp\\planar_graphs 2>nul");
    
    // 生成平面图测试用例
    generatePlanarGraphTestCases();
    
    cout << "\n平面图生成完成！" << endl;
    return 0;
}