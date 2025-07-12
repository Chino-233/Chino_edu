#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <filesystem>
#include <vector>
#include <chrono>
using namespace std;
namespace fs = std::filesystem;
void generateAndSaveData(int size, int fileCount, const string& baseDir) {
    //创建目录
    string dirName = baseDir + "\\n_" + to_string(size);
    fs::create_directories(dirName);
    //随机数生成器
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(1, 1000000000); // 1到10亿之间的随机整数
    vector<int> data(size);
    for (int file = 1; file <= fileCount; ++file) {
        //创建文件
        string fileName = dirName + "\\data_" + to_string(file) + ".txt";
        ofstream outFile(fileName);
        
        if (!outFile) {
            cerr << "无法创建文件: " << fileName << endl;
            continue;
        }
        //生成随机数据
        for (int i = 0; i < size; ++i) {
            data[i] = dist(gen);
        }
        //写入文件
        for (int i = 0; i < size; ++i) {
            outFile << data[i];
            if (i < size - 1) {
                outFile << " ";
            }
        }
        outFile.close();
        cout << "生成文件: " << fileName << endl;
    }
}

int main() {
    const string baseDir = "D:\\chino_edu\\makenum\\testnum";
    const int fileCount = 20;
    fs::create_directories(baseDir);
    //定义数据规模
    vector<int> sizes = {250000, 500000, 750000};
    cout << "开始生成测试数据..." << endl;
    auto startTime = chrono::high_resolution_clock::now();
    //生成各规模数据
    for (int size : sizes) {
        cout << "生成规模为 " << size << " 的数据集..." << endl;
        generateAndSaveData(size, fileCount, baseDir);
    }
    auto endTime = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::seconds>(endTime - startTime).count();
    cout << "数据生成完成，总耗时: " << duration << " 秒" << endl;
    return 0;
}