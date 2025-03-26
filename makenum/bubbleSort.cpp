#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <chrono>
#include <iomanip>
#include <cmath>
#include <filesystem>
namespace fs = std::filesystem;

using namespace std;

// 冒泡排序实现（添加超时检测）
bool bubbleSort(vector<int> &arr, const chrono::steady_clock::time_point &startTime, const double timeoutSeconds)
{
    int n = arr.size();
    for (int i = 0; i < n - 1; i++)
    {
        // 每轮外循环检查一次是否超时
        auto currentTime = chrono::steady_clock::now();
        double elapsedSeconds = chrono::duration<double>(currentTime - startTime).count();
        if (elapsedSeconds > timeoutSeconds)
        {
            return false; // 超时返回false
        }

        bool swapped = false;
        for (int j = 0; j < n - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }

            // 每1000次比较检查一次是否超时（避免频繁检查时间影响性能）
            if (j % 1000 == 0)
            {
                currentTime = chrono::steady_clock::now();
                elapsedSeconds = chrono::duration<double>(currentTime - startTime).count();
                if (elapsedSeconds > timeoutSeconds)
                {
                    return false; // 超时返回false
                }
            }
        }
        
        // 如果内层循环未发生交换，则数组已排序
        if (!swapped)
        {
            break;
        }
    }
    return true; // 正常完成返回true
}

// 从文件加载数据
vector<int> loadDataFromFile(const string &filePath)
{
    ifstream file(filePath);
    vector<int> data;

    if (!file.is_open())
    {
        cerr << "无法打开文件: " << filePath << endl;
        return data;
    }

    int value;
    while (file >> value)
    {
        data.push_back(value);
    }

    file.close();
    return data;
}

// 对指定文件的数据进行排序测试
double testSortingFile(const string &filePath)
{
    cout << "  读取文件: " << filePath << endl;
    vector<int> data = loadDataFromFile(filePath);

    if (data.empty())
    {
        cerr << "  文件为空或读取失败，跳过此测试" << endl;
        return -1.0;
    }

    cout << "  开始排序 " << data.size() << " 个元素..." << endl;

    // 计时开始 - 修改为使用steady_clock保持一致
    auto startTime = chrono::steady_clock::now();
    
    // 设置超时限制：200分钟 = 12000秒
    const double timeoutSeconds = 12000.0;

    // 执行排序（带超时检测）
    bool completed = bubbleSort(data, startTime, timeoutSeconds);

    if (!completed)
    {
        cout << "  排序已超时（超过" << timeoutSeconds << "秒）" << endl;
        return timeoutSeconds * 1000; // 返回超时上限（转换为毫秒）
    }

    // 计时结束
    auto endTime = chrono::steady_clock::now();

    // 计算排序时间（微秒转毫秒）
    auto durationMicros = chrono::duration_cast<chrono::microseconds>(endTime - startTime).count();
    double durationMs = static_cast<double>(durationMicros) / 1000.0;

    // 验证排序是否成功
    bool sorted = true;
    for (size_t i = 1; i < data.size(); i++)
    {
        if (data[i - 1] > data[i])
        {
            sorted = false;
            break;
        }
    }

    if (!sorted)
    {
        cerr << "  警告: 排序结果错误!" << endl;
    }

    cout << "  完成: " << fixed << setprecision(6) << durationMs << " 毫秒" << endl;
    return durationMs;
}

// 导出排序结果到文件
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
            
            // 检查是否超时（12000秒 = 12000000毫秒）
            if (actualTime >= 12000000.0)
            {
                outFile << setw(8) << size << "\t"
                        << "超时(>12000秒)" << "\t"
                        << "N/A\t"
                        << "N/A\t"
                        << "N/A" << endl;
                continue;
            }

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
            double time = avgTimes.at(size);
            if (time >= 12000000.0)
            {
                compOutFile << setw(12) << "超时" << "\t";
            }
            else
            {
                compOutFile << fixed << setprecision(2) << setw(12) << time << "\t";
            }
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

int main()
{
    // 定义要测试的数据规模
    vector<int> dataSizes = {1000, 10000, 100000, 1000000};

    // 基础路径
    string basePath = "d:\\chino_edu\\makenum\\testnum\\";

    // 存储每个规模的测试结果
    map<int, vector<double>> testResults;

    // 标准时间（100000规模的平均时间）
    double standardTime = 0.0;

    cout << "开始测试不同数据规模的冒泡排序性能...\n" << endl;

    // 对每个数据规模进行测试
    for (int size : dataSizes)
    {
        cout << "\n测试数据规模: " << size << " 个元素" << endl;

        // 根据数据规模确定测试文件数量
        int numFiles;
        if (size == 100000)
        {
            numFiles = 5; // 对于100000量级使用5个数据文件
        }
        else if (size > 100000)
        {
            numFiles = 2;  // 超过100000量级使用2个数据文件
        }
        else
        {
            numFiles = 20; // 小于100000量级使用20个数据文件
        }

        string sizeFolder = "n_" + to_string(size);
        string folderPath = basePath + sizeFolder;

        for (int fileNum = 1; fileNum <= numFiles; fileNum++)
        {
            string filePath = folderPath + "\\data_" + to_string(fileNum) + ".txt";

            // 检查文件是否存在
            if (!fs::exists(filePath))
            {
                cerr << "  文件不存在: " << filePath << endl;
                continue;
            }

            double duration = testSortingFile(filePath);
            if (duration > 0)
            {
                testResults[size].push_back(duration);
                // 删除了超时后跳过剩余测试的代码
            }
        }
    }

    // 计算各规模的平均排序时间
    map<int, double> avgTimes;
    for (const auto &[size, times] : testResults)
    {
        if (!times.empty())
        {
            // 检查该规模是否有超时情况
            bool hasTimeout = false;
            for (double time : times)
            {
                if (time >= 12000000.0) {
                    hasTimeout = true;
                    break;
                }
            }
            
            if (hasTimeout) {
                // 如果有超时，使用超时上限作为平均时间
                avgTimes[size] = 12000000.0;
            } else {
                // 正常计算平均时间
                double total = 0.0;
                for (double time : times)
                {
                    total += time;
                }
                avgTimes[size] = total / times.size();
            }

            if (size == 100000 && !hasTimeout)
            {
                standardTime = avgTimes[size];
            }
        }
    }

    // 计算理论时间和差距
    cout << "\n\n========== 冒泡排序性能测试结果 ==========\n" << endl;
    cout << "数据规模\t平均时间(毫秒)\t理论时间(毫秒)\t时间差异\t差异率(%)" << endl;
    cout << "-----------------------------------------------------------------------------" << endl;

    for (int size : dataSizes)
    {
        if (avgTimes.find(size) != avgTimes.end())
        {
            double actualTime = avgTimes[size];
            
            // 检查是否超时
            if (actualTime >= 12000000.0) {
                cout << setw(8) << size << "\t超时(>12000秒)\tN/A\tN/A\tN/A" << endl;
                continue;
            }

            // 计算理论时间: O(n²)
            double theoreticalTime = standardTime * pow(static_cast<double>(size) / 100000.0, 2);

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
    exportSortingResults("bubbleSort", dataSizes, avgTimes, standardTime, isQuadratic);
    return 0;
}