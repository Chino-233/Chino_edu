#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <filesystem>
#include <vector>
#include <chrono>
using namespace std;
namespace fs = std::filesystem;
void generateBillionNumbers(const string& baseDir) {
    const long long totalNumbers = 1000000000LL;
    const int bufferSize = 10000000; 
    string dirName = baseDir + "\\n_billion";
    fs::create_directories(dirName);
    //随机数生成器
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(1, 1000000000); // 1到10亿之间的随机整数
    //创建文件
    string fileName = dirName + "\\billion_numbers.bin";
    ofstream outFile(fileName, ios::out | ios::binary);
    if (!outFile) {
        cerr << "无法创建文件: " << fileName << endl;
        return;
    }
    cout << "开始生成10亿数据到单个文件..." << endl;
    auto startTime = chrono::high_resolution_clock::now();
    vector<int> buffer(bufferSize);
    long long numbersGenerated = 0;
    while (numbersGenerated < totalNumbers) {
        int currentBatchSize = min(bufferSize, static_cast<int>(totalNumbers - numbersGenerated));
        for (int i = 0; i < currentBatchSize; ++i) {
            buffer[i] = dist(gen);
        }
        outFile.write(reinterpret_cast<char*>(buffer.data()), 
                     currentBatchSize * sizeof(int));
        
        numbersGenerated += currentBatchSize;
        double percentage = static_cast<double>(numbersGenerated) * 100.0 / totalNumbers;
        cout << "已完成: " << fixed << setprecision(2) << percentage 
             << "% (" << numbersGenerated << "/" << totalNumbers << ")" << endl;
    }
    outFile.close();
    auto endTime = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::minutes>(endTime - startTime).count();
    cout << "10亿数据生成完成，保存到: " << fileName << endl;
    cout << "总耗时: " << duration << " 分钟" << endl;
    cout << "文件大小: " << static_cast<double>(totalNumbers * sizeof(int)) / (1024 * 1024 * 1024) << " GB" << endl;
}

int main() {
    const string baseDir = "D:\\chino_edu\\makenum\\testnum";
    fs::create_directories(baseDir);
    cout << "开始生成10亿测试数据..." << endl;
    generateBillionNumbers(baseDir);
    cout << "数据生成完成" << endl;
    return 0;
}