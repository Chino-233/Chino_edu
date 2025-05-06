#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <string>
#include <chrono>
#include <limits>
#include <iomanip>

using namespace std;
using namespace std::chrono;

// Point结构体定义
struct Point {
    double x, y;
    int id;
    Point(double _x = 0, double _y = 0, int _id = -1) : x(_x), y(_y), id(_id) {}
    static double distance(const Point& p1, const Point& p2) {
        return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
    }
};

// 从文件读取点
vector<Point> loadPointsFromFile(const string& filename) {
    vector<Point> points;
    ifstream infile(filename);
    
    if (!infile) {
        cerr << "无法打开文件: " << filename << endl;
        return points;
    }
    
    int N;
    infile >> N;
    points.reserve(N);
    
    double x, y;
    
    for (int i = 0; i < N; i++) {
        if (infile >> x >> y) {
            points.emplace_back(x, y, i);
        }
    }
    
    infile.close();
    return points;
}

// 蛮力法求解最近点对
pair<Point, Point> bruteForce(const vector<Point>& points, double& minDist) {
    int n = points.size();
    minDist = numeric_limits<double>::max();
    pair<Point, Point> closestPair;
    
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            double dist = Point::distance(points[i], points[j]);
            if (dist < minDist) {
                minDist = dist;
                closestPair = {points[i], points[j]};
            }
        }
    }
    
    return closestPair;
}

// 分治法相关函数声明
pair<Point, Point> closestPairDC(vector<Point>& pointsByX, vector<Point>& pointsByY, double& minDist);
pair<Point, Point> closestCrossingPair(const vector<Point>& pointsByY, double d, double midX, double& minDist);
pair<Point, Point> divideAndConquer(vector<Point>& points, double& minDist);

// 分治法入口函数
pair<Point, Point> divideAndConquer(vector<Point>& points, double& minDist) {
    // 按x坐标排序
    vector<Point> pointsByX = points;
    sort(pointsByX.begin(), pointsByX.end(), [](const Point& p1, const Point& p2) {
        return p1.x < p2.x;
    });
    
    // 按y坐标排序
    vector<Point> pointsByY = points;
    sort(pointsByY.begin(), pointsByY.end(), [](const Point& p1, const Point& p2) {
        return p1.y < p2.y;
    });
    
    return closestPairDC(pointsByX, pointsByY, minDist);
}

// 分治法核心递归函数
pair<Point, Point> closestPairDC(vector<Point>& pointsByX, vector<Point>& pointsByY, double& minDist) {
    int n = pointsByX.size();
    
    // 基本情况：点数<=3时使用蛮力法
    if (n <= 3) {
        return bruteForce(pointsByX, minDist);
    }
    
    // 分割点集
    int mid = n / 2;
    double midX = pointsByX[mid].x;
    
    vector<Point> leftX(pointsByX.begin(), pointsByX.begin() + mid);
    vector<Point> rightX(pointsByX.begin() + mid, pointsByX.end());
    
    // 按y坐标分割
    vector<Point> leftY, rightY;
    for (const Point& p : pointsByY) {
        if (p.x < midX) {
            leftY.push_back(p);
        } else {
            rightY.push_back(p);
        }
    }
    
    // 递归求解左右子问题
    double leftMin, rightMin;
    leftMin = rightMin = numeric_limits<double>::max();
    
    pair<Point, Point> leftPair = closestPairDC(leftX, leftY, leftMin);
    pair<Point, Point> rightPair = closestPairDC(rightX, rightY, rightMin);
    
    // 取左右子问题中的最小值
    pair<Point, Point> minPair;
    if (leftMin < rightMin) {
        minDist = leftMin;
        minPair = leftPair;
    } else {
        minDist = rightMin;
        minPair = rightPair;
    }
    
    // 处理跨越中线的情况
    double stripMinDist = minDist; // 保存当前最小距离
    pair<Point, Point> stripPair = closestCrossingPair(pointsByY, minDist, midX, stripMinDist);
    
    // 如果在跨越中线的区域找到了更小的距离，则更新结果
    if (stripMinDist < minDist) {
        minDist = stripMinDist;
        return stripPair;
    }
    
    return minPair;
}

// 处理跨越中线的点对
pair<Point, Point> closestCrossingPair(const vector<Point>& pointsByY, double d, double midX, double& minDist) {
    // 筛选出在中线±d范围内的点
    vector<Point> strip;
    for (const Point& p : pointsByY) {
        if (abs(p.x - midX) < d) {
            strip.push_back(p);
        }
    }
    
    // 在带状区域内查找最近点对
    pair<Point, Point> closestPair;
    double minStrip = d;
    int size = strip.size();
    bool foundCloser = false;
    
    for (int i = 0; i < size; i++) {
        // 对每个点，最多只需检查后面7个点
        for (int j = i + 1; j < size && (strip[j].y - strip[i].y) < d; j++) {
            double dist = Point::distance(strip[i], strip[j]);
            if (dist < minStrip) {
                minStrip = dist;
                closestPair = {strip[i], strip[j]};
                foundCloser = true;
            }
        }
    }
    
    if (foundCloser) {
        minDist = minStrip;
        return closestPair;
    }
    
    return pair<Point, Point>(); // 返回空点对
}

int main(int argc, char* argv[]) {
    // 支持从命令行指定点的数量
    int N = 1000000; // 默认值
    string filename;
    
    if (argc > 1) {
        N = atoi(argv[1]);
    }
    
    filename = "points_" + to_string(N) + ".txt";
    
    // 读取点集
    cout << "从文件 " << filename << " 读取点..." << endl;
    vector<Point> points = loadPointsFromFile(filename);
    
    if (points.empty()) {
        cerr << "无法读取点或文件为空！请先生成点文件。" << endl;
        return 1;
    }
    
    cout << "成功读取 " << points.size() << " 个点。" << endl;
    
    // 分治法计算
    double divideConquerMinDist;
    cout << "\n开始分治法计算..." << endl;
    auto start = high_resolution_clock::now();
    pair<Point, Point> dcResult = divideAndConquer(points, divideConquerMinDist);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    
    cout << "分治法完成，耗时: " << duration.count() << " 毫秒" << endl;
    cout << "最近点对: (" << dcResult.first.x << ", " << dcResult.first.y << ") 和 ("
         << dcResult.second.x << ", " << dcResult.second.y << ")" << endl;
    cout << "最短距离: " << divideConquerMinDist << endl;
    
    // 蛮力法计算（仅当点数不太多时执行）
    if (points.size() <= 10000000) {  // 对于大规模数据，蛮力法太慢
        double bruteForceMinDist;
        cout << "\n开始蛮力法计算..." << endl;
        start = high_resolution_clock::now();
        pair<Point, Point> bfResult = bruteForce(points, bruteForceMinDist);
        stop = high_resolution_clock::now();
        duration = duration_cast<milliseconds>(stop - start);
        
        cout << "蛮力法完成，耗时: " << duration.count() << " 毫秒" << endl;
        cout << "最近点对: (" << bfResult.first.x << ", " << bfResult.first.y << ") 和 ("
             << bfResult.second.x << ", " << bfResult.second.y << ")" << endl;
        cout << "最短距离: " << bruteForceMinDist << endl;
        
        // 验证两种方法结果是否一致
        cout << "\n两种方法的结果" << (abs(bruteForceMinDist - divideConquerMinDist) < 1e-9 ? "一致" : "不一致") << endl;
    } else {
        cout << "\n点数过多，跳过蛮力法计算" << endl;
    }
    
    return 0;
}