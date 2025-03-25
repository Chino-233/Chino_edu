#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
#include <chrono>
#include <filesystem>
#include <algorithm>
#include <iomanip>
#include <atomic>
#include <thread>
#include <random> // 添加这个头文件以解决未定义标识符错误

namespace fs = std::filesystem;
using namespace std;

// 定义全局变量用于超时控制
atomic<bool> timeout_flag(false);
const int TIMEOUT_SECONDS = 600; // 10分钟超时限制

// 使用最小堆（优先队列）找出最大的K个数
vector<int> findTopKWithMinHeap(const vector<int>& nums, int k) {
    // 使用最小堆
    priority_queue<int, vector<int>, greater<int>> minHeap;
    
    for (int num : nums) {
        // 检查超时
        if (timeout_flag.load()) {
            return vector<int>();
        }
        
        if (minHeap.size() < k) {
            // 如果堆的大小小于k，直接加入堆
            minHeap.push(num);
        } else if (num > minHeap.top()) {
            // 如果当前数字大于堆顶（最小的元素），则替换堆顶
            minHeap.pop();
            minHeap.push(num);
        }
    }
    
    // 将堆中的元素放入结果数组
    vector<int> result;
    while (!minHeap.empty()) {
        result.push_back(minHeap.top());
        minHeap.pop();
    }
    
    // 反转结果，使其按降序排列
    reverse(result.begin(), result.end());
    return result;
}

// 从文件加载部分数据进行测试
vector<int> loadTestDataFromFile(const string& filePath, int maxNumbers = -1) {
    ifstream file(filePath, ios::in | ios::binary);
    vector<int> data;
    
    if (!file.is_open()) {
        cerr << "无法打开文件: " << filePath << endl;
        return data;
    }
    
    int value;
    int count = 0;
    
    // 检查文件是否为二进制格式
    if (filePath.find(".txt") != string::npos) {
        // 文本格式
        while (file >> value) {
            data.push_back(value);
            count++;
            if (maxNumbers > 0 && count >= maxNumbers) break;
        }
    } else {
        // 二进制格式
        while (file.read(reinterpret_cast<char*>(&value), sizeof(int))) {
            data.push_back(value);
            count++;
            if (maxNumbers > 0 && count >= maxNumbers) break;
        }
    }
    
    file.close();
    return data;
}

// 超时监控函数
void timeoutMonitor() {
    // 等待指定时间
    this_thread::sleep_for(chrono::seconds(TIMEOUT_SECONDS));
    // 设置超时标志
    timeout_flag.store(true);
    cout << "\n  *** 检测到操作超时(" << TIMEOUT_SECONDS << "秒)，中断当前操作 ***" << endl;
}

// 在小规模数据上测试算法
void testOnSmallData(int k) {
    cout << "\n===== 小规模数据测试（K=" << k << "） =====" << endl;
    
    // 测试数据大小
    const int testSize = 100000;
    cout << "生成" << testSize << "个随机测试数据" << endl;
    
    // 生成随机测试数据
    vector<int> testData(testSize);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(1, 1000000000);
    
    for (int i = 0; i < testSize; i++) {
        testData[i] = dist(gen);
    }
    
    // 重置超时标志
    timeout_flag.store(false);
    
    // 启动超时监控线程
    thread timeout_thread(timeoutMonitor);
    timeout_thread.detach();
    
    // 测试小根堆算法
    cout << "使用小根堆算法查找Top " << k << "..." << endl;
    auto startTime = chrono::high_resolution_clock::now();
    vector<int> result = findTopKWithMinHeap(testData, k);
    auto endTime = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime).count();
    
    // 验证结果（与完全排序比较）
    vector<int> sortedData = testData;
    sort(sortedData.begin(), sortedData.end(), greater<int>());
    vector<int> groundTruth(sortedData.begin(), sortedData.begin() + k);
    
    bool correct = true;
    if (result.size() == groundTruth.size()) {
        for (size_t i = 0; i < result.size(); i++) {
            if (result[i] != groundTruth[i]) {
                correct = false;
                break;
            }
        }
    } else {
        correct = false;
    }
    
    cout << "结果: " << (correct ? "正确" : "错误") << ", 耗时: " << duration << "ms" << endl;
    
    cout << "Top " << k << " 结果: ";
    for (int i = 0; i < k && i < result.size(); i++) {
        cout << result[i] << " ";
    }
    cout << endl;
}

// 分批处理大文件，找出Top K
vector<int> processBigFileTopK(const string& filePath, int k, int batchSize = 10000000) {
    cout << "\n===== 处理大文件数据查找Top " << k << " =====" << endl;
    
    // 检查文件是否存在
    if (!fs::exists(filePath)) {
        cerr << "文件不存在: " << filePath << endl;
        return vector<int>();
    }
    
    ifstream file(filePath, ios::in | ios::binary);
    if (!file.is_open()) {
        cerr << "无法打开文件: " << filePath << endl;
        return vector<int>();
    }
    
    // 获取文件大小
    file.seekg(0, ios::end);
    streamsize fileSize = file.tellg();
    file.seekg(0, ios::beg);
    
    long long totalInts = fileSize / sizeof(int);
    cout << "文件包含 " << totalInts << " 个整数" << endl;
    
    // 重置超时标志
    timeout_flag.store(false);
    
    // 启动超时监控线程
    thread timeout_thread(timeoutMonitor);
    timeout_thread.detach();
    
    auto startTime = chrono::high_resolution_clock::now();
    
    // 使用最小堆保存当前的Top K元素
    priority_queue<int, vector<int>, greater<int>> minHeap;
    
    // 分批读取并处理
    vector<int> buffer(batchSize);
    long long processedCount = 0;
    int batchNumber = 0;
    
    while (!file.eof() && !timeout_flag.load()) {
        // 读取一批数据
        int currentBatchSize = 0;
        file.read(reinterpret_cast<char*>(buffer.data()), batchSize * sizeof(int));
        currentBatchSize = static_cast<int>(file.gcount() / sizeof(int));
        
        if (currentBatchSize <= 0) break;
        
        batchNumber++;
        processedCount += currentBatchSize;
        
        cout << "处理批次 " << batchNumber 
             << " (" << processedCount << "/" << totalInts 
             << ", " << fixed << setprecision(2) 
             << static_cast<double>(processedCount) * 100 / totalInts << "%)" << endl;
        
        // 处理当前批次
        for (int i = 0; i < currentBatchSize; i++) {
            int num = buffer[i];
            
            if (minHeap.size() < k) {
                // 如果堆大小小于k，直接添加
                minHeap.push(num);
            } else if (num > minHeap.top()) {
                // 如果当前数比堆顶大，替换堆顶
                minHeap.pop();
                minHeap.push(num);
            }
            
            // 定期检查超时
            if (i % 1000000 == 0 && timeout_flag.load()) {
                break;
            }
        }
        
        if (timeout_flag.load()) {
            break;
        }
    }
    
    file.close();
    
    // 将堆中元素转换为结果数组
    vector<int> result;
    while (!minHeap.empty()) {
        result.push_back(minHeap.top());
        minHeap.pop();
    }
    
    // 反转结果，使其按降序排列
    reverse(result.begin(), result.end());
    
    auto endTime = chrono::high_resolution_clock::now();
    auto durationMicros = chrono::duration_cast<chrono::microseconds>(endTime - startTime).count();
    
    if (timeout_flag.load()) {
        cout << "处理超时，但已返回部分结果" << endl;
    } else {
        double durationMs = static_cast<double>(durationMicros) / 1000.0;
        cout << "处理完成，耗时: " << fixed << setprecision(6) << durationMs << " ms" << endl;
        cout << "已处理 " << processedCount << "/" << totalInts << " 个数据" << endl;
    }
    
    return result;
}

// 主函数
int main() {
    const int k = 10; // 寻找最大的10个数
    
    // 1. 在小规模数据上测试算法
    testOnSmallData(k);
    
    // 2. 处理10亿数据文件
    string largeFilePath = "d:\\chino_edu\\makenum\\testnum\\n_billion\\billion_numbers.bin";
    
    // 检查大数据文件是否存在
    if (fs::exists(largeFilePath)) {
        cout << "\n===== 尝试在10亿数据上找Top " << k << " =====" << endl;
        vector<int> result = processBigFileTopK(largeFilePath, k);
        
        if (!result.empty()) {
            cout << "找到的Top " << k << " 结果: ";
            for (int num : result) {
                cout << num << " ";
            }
            cout << endl;
        }
    } else {
        cout << "\n大数据文件不存在，请先运行makenum_1b.cpp生成10亿测试数据" << endl;
    }
    
    return 0;
}