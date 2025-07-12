#include <bits/stdc++.h>
using namespace std;
struct Point {
    double x, y;
    int id;
    Point(double _x = 0, double _y = 0, int _id = -1) : x(_x), y(_y), id(_id) {}
    static double distance(const Point& p1, const Point& p2) {
        return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
    }
};

vector<Point> generateRandomPoints(int N, double minVal = 0.0, double maxVal = 1000.0) {
    vector<Point> points;
    points.reserve(N);
    
    // 使用更好的随机数生成器
    random_device rd;
    mt19937 gen(rd());  // Mersenne Twister随机数生成器
    uniform_real_distribution<> dis(minVal, maxVal);
    
    for (int i = 0; i < N; i++) {
        points.emplace_back(dis(gen), dis(gen), i);
    }
    
    return points;
}

// 将点保存到文件
bool savePointsToFile(const vector<Point>& points, const string& filename) {
    ofstream outfile(filename);
    if (!outfile) {
        cerr << "无法打开文件: " << filename << endl;
        return false;
    }
    
    outfile << points.size() << endl;
    for (const auto& point : points) {
        outfile << point.x << " " << point.y << endl;
    }
    
    outfile.close();
    return true;
}
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

int main(int argc, char* argv[]) {
    // 默认的点数量列表
    vector<int> pointCounts = {20000,40000,80000};
    
    // 如果有命令行参数，则使用命令行参数替换默认列表
    if (argc > 1) {
        pointCounts.clear();
        for (int i = 1; i < argc; i++) {
            int count = atoi(argv[i]);
            if (count <= 0) {
                cerr << "点的数量必须为正整数: " << argv[i] << endl;
                return 1;
            }
            pointCounts.push_back(count);
        }
    }
    
    // 对每个点数量生成并保存点集
    for (int N : pointCounts) {
        // 生成随机点
        cout << "正在生成 " << N << " 个随机点..." << endl;
        vector<Point> points = generateRandomPoints(N);
        
        // 保存到文件
        string filename = "points_" + to_string(N) + ".txt";
        if (savePointsToFile(points, filename)) {
            cout << "成功生成 " << N << " 个随机点并保存到文件 " << filename << endl;
        }
        
        // 输出前10个点作为示例
        cout << "\n生成的前10个点示例：" << endl;
        int show_count = min(10, N);
        for (int i = 0; i < show_count; i++) {
            cout << "点 " << i + 1 << ": (" << points[i].x << ", " << points[i].y << ")" << endl;
        }
        
        cout << "\n------------------------------\n" << endl;
    }
    
    return 0;
}