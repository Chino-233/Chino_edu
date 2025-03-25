#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <chrono>
#include <iomanip>
#include <cmath>
#include <filesystem>
#include <atomic>
#include <thread>
namespace fs = std::filesystem;

using namespace std;

// 定义全局变量用于超时控制
atomic<bool> timeout_flag(false);
const int TIMEOUT_SECONDS = 300; // 5分钟超时限制

// 快速排序实现
int partition(vector<int>& arr, int low, int high) {
    // 选择最右边的元素作为基准
    int pivot = arr[high];
    
    // 将小于基准的元素放到左侧
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        // 如果当前元素小于基准
        if (arr[j] < pivot) {
            i++; // 增加小于基准元素的计数
            swap(arr[i], arr[j]);
        }
        
        // 检查是否超时
        if (timeout_flag.load()) {
            return -1; // 超时标记
        }
    }
    
    // 将基准元素放到正确的位置
    swap(arr[i + 1], arr[high]);
    return (i + 1); // 返回基准的位置
}

void quickSortImpl(vector<int>& arr, int low, int high) {
    // 检查是否超时
    if (timeout_flag.load()) {
        return;
    }
    
    if (low < high) {
        // 获取分区索引
        int pi = partition(arr, low, high);
        
        // 如果返回-1表示超时中断
        if (pi == -1) {
            return;
        }
        
        // 递归排序基准左侧的元素
        quickSortImpl(arr, low, pi - 1);
        
        // 递归排序基准右侧的元素
        quickSortImpl(arr, pi + 1, high);
    }
}

void quickSort(vector<int>& arr) {
    quickSortImpl(arr, 0, arr.size() - 1);
}

// 从文件加载数据
vector<int> loadDataFromFile(const string& filePath) {
    ifstream file(filePath);
    vector<int> data;
    
    if (!file.is_open()) {
        cerr << "无法打开文件: " << filePath << endl;
        return data;
    }
    
    int value;
    while (file >> value) {
        data.push_back(value);
    }
    
    file.close();
    return data;
}

// 超时监控函数
void timeoutMonitor() {
    // 等待5分钟
    this_thread::sleep_for(chrono::seconds(TIMEOUT_SECONDS));
    // 设置超时标志
    timeout_flag.store(true);
    cout << "\n  *** 检测到排序超时(" << TIMEOUT_SECONDS << "秒)，中断当前排序操作 ***" << endl;
}

// 对指定文件的数据进行排序测试
double testSortingFile(const string& filePath) {
    cout << "  读取文件: " << filePath << endl;
    vector<int> data = loadDataFromFile(filePath);
    
    if (data.empty()) {
        cerr << "  文件为空或读取失败，跳过此测试" << endl;
        return -1.0;
    }
    
    cout << "  开始排序 " << data.size() << " 个元素..." << endl;
    
    // 重置超时标志
    timeout_flag.store(false);
    
    // 启动超时监控线程
    thread timeout_thread(timeoutMonitor);
    timeout_thread.detach(); // 分离线程，让它在后台运行
    
    // 计时开始
    auto startTime = chrono::high_resolution_clock::now();
    
    // 执行排序
    quickSort(data);
    
    // 计时结束
    auto endTime = chrono::high_resolution_clock::now();
    
    // 如果发生超时，返回特殊值
    if (timeout_flag.load()) {
        return -2.0; // -2表示超时
    }
    
    // 计算排序时间（微秒转毫秒）
    auto durationMicros = chrono::duration_cast<chrono::microseconds>(endTime - startTime).count();
    double durationMs = static_cast<double>(durationMicros) / 1000.0;
    
    // 验证排序是否成功
    bool sorted = true;
    for (size_t i = 1; i < data.size(); i++) {
        if (data[i - 1] > data[i]) {
            sorted = false;
            break;
        }
    }
    
    if (!sorted) {
        cerr << "  警告: 排序结果错误!" << endl;
    }
    
    cout << "  完成: " << fixed << setprecision(6) << durationMs << " 毫秒" << endl;
    return durationMs;
}
void exportSortingResults(const string &algorithmName,const vector<int> &dataSizes,const map<int, double> &avgTimes,double standardTime,bool isQuadratic)
{
    // 创建结果文件夹
    string resultDir = "d:\\chino_edu\\makenum\\result\\";
    fs::create_directories(resultDir);

    // 为每个算法创建单独的结果文件
    string resultFile = resultDir + algorithmName + "_results.txt";
    ofstream outFile(resultFile);

    if (!outFile.is_open())
    {
        cerr << "无法创建结果文件: " << resultFile << endl;
        return;
    }

    // 写入表头
    outFile << "========== " << algorithmName << "性能测试结果 ==========\n"
            << endl;
    outFile << "时间复杂度: " << (isQuadratic ? "O(n²)" : "O(n log n)") << endl
            << endl;
    outFile << "数据规模\t平均时间(毫秒)\t理论时间(毫秒)\t时间差异\t差异率(%)" << endl;
    outFile << "-----------------------------------------------------------------------------" << endl;

    // 写入数据
    for (int size : dataSizes)
    {
        if (avgTimes.find(size) != avgTimes.end())
        {
            double actualTime = avgTimes.at(size);

            // 计算理论时间
            double theoreticalTime;
            if (isQuadratic)
            {
                theoreticalTime = standardTime * pow(static_cast<double>(size) / 100000.0, 2);
            }
            else
            {
                double nLogNRatio = (size * log2(size)) / (100000 * log2(100000));
                theoreticalTime = standardTime * nLogNRatio;
            }

            // 计算差异
            double timeDiff = actualTime - theoreticalTime;
            double diffRate = (theoreticalTime > 0) ? (timeDiff / theoreticalTime) * 100.0 : 0.0;

            // 格式化输出
            outFile << fixed << setprecision(2);
            outFile << setw(8) << size << "\t"
                    << setw(12) << actualTime << "\t"
                    << setw(12) << theoreticalTime << "\t"
                    << setw(10) << timeDiff << "\t"
                    << setw(8) << diffRate << "%" << endl;
        }
    }

    outFile.close();
    cout << "结果已保存到: " << resultFile << endl;

    // 同时添加到综合比较文件
    string comparisonFile = resultDir + "sorting_algorithms_comparison.txt";
    bool fileExists = fs::exists(comparisonFile);

    ofstream compOutFile(comparisonFile, ios::app);
    if (!compOutFile.is_open())
    {
        cerr << "无法创建综合比较文件: " << comparisonFile << endl;
        return;
    }

    // 如果文件不存在，添加表头
    if (!fileExists)
    {
        compOutFile << "========== 排序算法性能比较 ==========\n"
                    << endl;
        compOutFile << "算法名称\t时间复杂度\t";

        for (int size : dataSizes)
        {
            compOutFile << size << "元素(ms)\t";
        }
        compOutFile << endl;
        compOutFile << "--------------------------------------------------------------------------------------------" << endl;
    }

    // 添加当前算法数据
    compOutFile << left << setw(10) << algorithmName << "\t";
    compOutFile << left << setw(10) << (isQuadratic ? "O(n²)" : "O(n log n)") << "\t";

    for (int size : dataSizes)
    {
        if (avgTimes.find(size) != avgTimes.end())
        {
            compOutFile << fixed << setprecision(2) << setw(12) << avgTimes.at(size) << "\t";
        }
        else
        {
            compOutFile << setw(12) << "N/A" << "\t";
        }
    }
    compOutFile << endl;

    compOutFile.close();
    cout << "结果已添加到比较文件: " << comparisonFile << endl;
}

int main() {
    // 定义要测试的数据规模，根据要求去掉100规模
    vector<int> dataSizes = {1000, 10000, 100000, 1000000, 10000000};
    
    // 基础路径
    string basePath = "d:\\chino_edu\\makenum\\testnum\\";
    
    // 存储每个规模的测试结果
    map<int, vector<double>> testResults;
    
    // 标准时间（10000规模的平均时间）
    double standardTime = 0.0;
    
    cout << "开始测试不同数据规模的快速排序性能...\n" << endl;
    
    // 对每个数据规模进行测试
    for (int size : dataSizes) {
        cout << "\n测试数据规模: " << size << " 个元素" << endl;
        
        // 确定测试文件数量
        int numFiles = (size >= 1000000) ? 5 : 20;
        
        string sizeFolder = "n_" + to_string(size);
        string folderPath = basePath + sizeFolder;
        
        for (int fileNum = 1; fileNum <= numFiles; fileNum++) {
            string filePath = folderPath + "\\data_" + to_string(fileNum) + ".txt";
            
            // 检查文件是否存在
            if (!fs::exists(filePath)) {
                cerr << "  文件不存在: " << filePath << endl;
                continue;
            }
            
            double duration = testSortingFile(filePath);
            if (duration > 0) {
                testResults[size].push_back(duration);
            } else if (duration == -2.0) {
                cout << "  跳过剩余测试文件，此规模数据排序耗时过长" << endl;
                break; // 如果一个文件超时，跳过该规模的其他测试
            }
        }
    }
    
    // 计算各规模的平均排序时间
    map<int, double> avgTimes;
    for (const auto& [size, times] : testResults) {
        if (!times.empty()) {
            double total = 0.0;
            for (double time : times) {
                total += time;
            }
            avgTimes[size] = total / times.size();
            
            if (size == 100000) {
                standardTime = avgTimes[size];
            }
        }
    }
    
    // 计算理论时间和差距
    cout << "\n\n========== 快速排序性能测试结果 ==========\n" << endl;
    cout << "数据规模\t平均时间(毫秒)\t理论时间(毫秒)\t时间差异\t差异率(%)" << endl;
    cout << "-----------------------------------------------------------------------------" << endl;
    
    for (int size : dataSizes) {
        if (avgTimes.find(size) != avgTimes.end()) {
            double actualTime = avgTimes[size];
            
            // 计算理论时间: O(n log n)
            double nLogNRatio = (size * log2(size)) / (100000 * log2(100000));
            double theoreticalTime = standardTime * nLogNRatio;
            
            // 计算差异
            double timeDiff = actualTime - theoreticalTime;
            double diffRate = (theoreticalTime > 0) ? (timeDiff / theoreticalTime) * 100.0 : 0.0;
            
            // 格式化输出
            cout << fixed << setprecision(2);
            cout << setw(8) << size << "\t" 
                 << setw(12) << actualTime << "\t"
                 << setw(12) << theoreticalTime << "\t"
                 << setw(10) << timeDiff << "\t"
                 << setw(8) << diffRate << "%" << endl;
        }
    }
    bool isQuadratic = true;
    exportSortingResults("quickSort", dataSizes, avgTimes, standardTime, isQuadratic);
    return 0;
}