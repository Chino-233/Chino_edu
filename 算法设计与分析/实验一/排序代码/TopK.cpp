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
#include <random>
namespace fs = std::filesystem;
using namespace std;
atomic<bool> timeout_flag(false);
const int TIMEOUT_SECONDS = 600;
vector<int> findTopKWithMinHeap(const vector<int>& nums, int k) {
    priority_queue<int, vector<int>, greater<int>> minHeap;
    for (int num : nums) {
        if (timeout_flag.load()) {
            return vector<int>();
        }
        if (minHeap.size() < k) {
            minHeap.push(num);
        } else if (num > minHeap.top()) {
            minHeap.pop();
            minHeap.push(num);
        }
    }
    vector<int> result;
    while (!minHeap.empty()) {
        result.push_back(minHeap.top());
        minHeap.pop();
    }
    reverse(result.begin(), result.end());
    return result;
}
//文件加载测试
vector<int> loadTestDataFromFile(const string& filePath, int maxNumbers = -1) {
    ifstream file(filePath, ios::in | ios::binary);
    vector<int> data;
    if (!file.is_open()) {
        cerr << "无法打开文件: " << filePath << endl;
        return data;
    }
    int value;
    int count = 0;
    if (filePath.find(".txt") != string::npos) {
        while (file >> value) {
            data.push_back(value);
            count++;
            if (maxNumbers > 0 && count >= maxNumbers) break;
        }
    }
    else {
        while (file.read(reinterpret_cast<char*>(&value), sizeof(int))) {
            data.push_back(value);
            count++;
            if (maxNumbers > 0 && count >= maxNumbers) break;
        }
    }
    file.close();
    return data;
}
//时间函数
void timeoutMonitor() {
    this_thread::sleep_for(chrono::seconds(TIMEOUT_SECONDS));
    timeout_flag.store(true);
    cout << "\n  *** 检测到操作超时(" << TIMEOUT_SECONDS << "秒)，中断当前操作 ***" << endl;
}
void testOnSmallData(int k) {
    cout << "\n===== 小规模数据测试（K=" << k << "） =====" << endl;
    const int testSize = 100000;
    cout << "生成" << testSize << "个随机测试数据" << endl;
    //生成随机数据
    vector<int> testData(testSize);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(1, 1000000000);
    for (int i = 0; i < testSize; i++) {
        testData[i] = dist(gen);
    }
    timeout_flag.store(false);
    thread timeout_thread(timeoutMonitor);
    timeout_thread.detach();
    //测试算法
    cout << "使用小根堆算法查找Top " << k << "..." << endl;
    auto startTime = chrono::high_resolution_clock::now();
    vector<int> result = findTopKWithMinHeap(testData, k);
    auto endTime = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime).count();
    // 验证结果
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

vector<int> processBigFileTopK(const string& filePath, int k, int batchSize = 10000000) {
    cout << "\n===== 处理大文件数据查找Top " << k << " =====" << endl;
    //检查文件
    if (!fs::exists(filePath)) {
        cerr << "文件不存在: " << filePath << endl;
        return vector<int>();
    }
    ifstream file(filePath, ios::in | ios::binary);
    if (!file.is_open()) {
        cerr << "无法打开文件: " << filePath << endl;
        return vector<int>();
    }

    file.seekg(0, ios::end);
    streamsize fileSize = file.tellg();
    file.seekg(0, ios::beg);
    long long totalInts = fileSize / sizeof(int);
    cout << "文件包含 " << totalInts << " 个整数" << endl;
    timeout_flag.store(false);
    thread timeout_thread(timeoutMonitor);
    timeout_thread.detach();
    auto startTime = chrono::high_resolution_clock::now();
    priority_queue<int, vector<int>, greater<int>> minHeap;
    //分批读取
    vector<int> buffer(batchSize);
    long long processedCount = 0;
    int batchNumber = 0;
    while (!file.eof() && !timeout_flag.load()) {
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
                minHeap.push(num);
            } 
            else if (num > minHeap.top()) {

                minHeap.pop();
                minHeap.push(num);
            }
            if (i % 1000000 == 0 && timeout_flag.load()) {
                break;
            }
        }
        
        if (timeout_flag.load()) {
            break;
        }
    }
    file.close();
    vector<int> result;
    while (!minHeap.empty()) {
        result.push_back(minHeap.top());
        minHeap.pop();
    }
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

int main() {
    const int k = 10; // 寻找最大的10个数
    
    //在小规模数据上测试
    testOnSmallData(k);
    //处理文件
    string largeFilePath = "d:\\chino_edu\\makenum\\testnum\\n_billion\\billion_numbers.bin";
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
    }
    else {
        cout << "\n大数据文件不存在，请先运行makenum_1b.cpp生成10亿测试数据" << endl;
    }
    
    return 0;
}