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


struct Point {
    double x, y;
    int id;
    Point(double _x = 0, double _y = 0, int _id = -1) : x(_x), y(_y), id(_id) {}
    static double distance(const Point& p1, const Point& p2) {
        return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);//优化。距离可以最后再开根号
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

// 蛮力法基准测试
void runBruteForceBenchmark() {
    // 由于蛮力法复杂度较高，使用较小的数据规模
    const int REPEAT_COUNT = 5; // 每个N值重复测试的次数
    const vector<int> TEST_SIZES = {10,100,500,1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000,20000,40000,80000,100000};
    
    // 创建结果文件
    ofstream resultFile("brute_force_benchmark_results.csv");
    if (!resultFile) {
        cerr << "无法创建结果文件！" << endl;
        return;
    }
    
    // 写入CSV头
    resultFile << "N,Test,BruteForceTime(ms),MinDistance" << endl;
    
    for (int N : TEST_SIZES) {
        cout << "测试点数 N = " << N << endl;
        string filename = "points_" + to_string(N) + ".txt";
        
        // 读取点集
        cout << "从文件 " << filename << " 读取点..." << endl;
        vector<Point> points = loadPointsFromFile(filename);
        
        if (points.empty()) {
            cerr << "无法读取点或文件为空！请先生成点文件。" << endl;
            continue;
        }
        
        cout << "成功读取 " << points.size() << " 个点。" << endl;
        
        // 用于计算平均时间
        double totalTime = 0.0;
        double avgMinDist = 0.0;
        
        // 对每个N值进行多次测试
        for (int test = 1; test <= REPEAT_COUNT; test++) {
            cout << "测试 " << test << "/" << REPEAT_COUNT << " 开始..." << endl;
            
            // 蛮力法计算
            double bruteForceMinDist;
            auto start = high_resolution_clock::now();
            pair<Point, Point> bfResult = bruteForce(points, bruteForceMinDist);
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
            double ms = duration.count() / 1000.0; // 将微秒转换为毫秒，保留小数部分
            
            // 累计时间和最短距离
            totalTime += ms;
            avgMinDist += bruteForceMinDist;
            
            cout << "蛮力法完成，耗时: " << fixed << setprecision(4) << ms << " 毫秒" << endl;
            cout << "最短距离: " << bruteForceMinDist << endl;
            
            // 保存结果到CSV
            resultFile << N << "," << test << "," << fixed << setprecision(4) << ms << "," << setprecision(10) << bruteForceMinDist << endl;
        }
        
        // 计算并输出平均值
        double avgTime = totalTime / REPEAT_COUNT;
        avgMinDist /= REPEAT_COUNT;
        
        cout << "\n点数 N = " << N << " 的平均执行时间: " << fixed << setprecision(4) << avgTime << " 毫秒" << endl;
        cout << "点数 N = " << N << " 的平均最短距离: " << setprecision(10) << avgMinDist << endl;
        
        // 将平均值也保存到CSV
        resultFile << N << ",平均," << fixed << setprecision(4) << avgTime << "," << setprecision(10) << avgMinDist << endl;
        
        cout << "完成 N = " << N << " 的测试" << endl << endl;
    }
    
    resultFile.close();
    cout << "所有蛮力法测试完成，结果保存到 brute_force_benchmark_results.csv" << endl;
}

// 分治法基准测试
void runDivideConquerBenchmark() {
    // 测试参数
    const int REPEAT_COUNT = 5; // 每个N值重复测试的次数
    const vector<int> TEST_SIZES = {10,100,1000,10000,100000, 200000, 300000, 400000, 500000, 600000, 700000, 800000, 900000, 1000000};
    
    // 创建结果文件
    ofstream resultFile("divide_conquer_benchmark_results.csv");
    if (!resultFile) {
        cerr << "无法创建结果文件！" << endl;
        return;
    }
    
    // 写入CSV头
    resultFile << "N,Test,DivideConquerTime(ms),MinDistance" << endl;
    
    for (int N : TEST_SIZES) {
        cout << "测试点数 N = " << N << endl;
        string filename = "points_" + to_string(N) + ".txt";
        
        // 读取点集
        cout << "从文件 " << filename << " 读取点..." << endl;
        vector<Point> points = loadPointsFromFile(filename);
        
        if (points.empty()) {
            cerr << "无法读取点或文件为空！请先生成点文件。" << endl;
            continue;
        }
        
        cout << "成功读取 " << points.size() << " 个点。" << endl;
        
        // 用于计算平均时间
        double totalTime = 0.0;
        double avgMinDist = 0.0;
        
        // 对每个N值进行多次测试
        for (int test = 1; test <= REPEAT_COUNT; test++) {
            cout << "测试 " << test << "/" << REPEAT_COUNT << " 开始..." << endl;
            
            // 分治法计算
            double divideConquerMinDist;
            auto start = high_resolution_clock::now();
            pair<Point, Point> dcResult = divideAndConquer(points, divideConquerMinDist);
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
            double ms = duration.count() / 1000.0; // 将微秒转换为毫秒，保留小数部分
            
            // 累计时间和最短距离
            totalTime += ms;
            avgMinDist += divideConquerMinDist;
            
            cout << "分治法完成，耗时: " << fixed << setprecision(4) << ms << " 毫秒" << endl;
            cout << "最短距离: " << divideConquerMinDist << endl;
            
            // 保存结果到CSV
            resultFile << N << "," << test << "," << fixed << setprecision(4) << ms << "," << setprecision(10) << divideConquerMinDist << endl;
        }
        
        // 计算并输出平均值
        double avgTime = totalTime / REPEAT_COUNT;
        avgMinDist /= REPEAT_COUNT;
        
        cout << "\n点数 N = " << N << " 的平均执行时间: " << fixed << setprecision(4) << avgTime << " 毫秒" << endl;
        cout << "点数 N = " << N << " 的平均最短距离: " << setprecision(10) << avgMinDist << endl;
        
        // 将平均值也保存到CSV
        resultFile << N << ",平均," << fixed << setprecision(4) << avgTime << "," << setprecision(10) << avgMinDist << endl;
        
        cout << "完成 N = " << N << " 的测试" << endl << endl;
    }
    
    resultFile.close();
    cout << "所有分治法测试完成，结果保存到 divide_conquer_benchmark_results.csv" << endl;
}

// 运行基准测试的主函数
void runBenchmark() {
    runBruteForceBenchmark();
    runDivideConquerBenchmark();
}

int main() {
    // 运行基准测试
    runBenchmark();
    return 0;
}