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

// 冒泡排序算法实现
// 参数：待排序数组、开始时间点、超时秒数限制
// 返回：true表示正常完成排序，false表示排序超时
bool bubbleSort(vector<int> &arr, const chrono::steady_clock::time_point &startTime, const double timeoutSeconds)
{
    int n = arr.size();
    for (int i = 0; i < n - 1; i++)
    {
        // 外循环每轮检查是否超过了允许的执行时间
        auto currentTime = chrono::steady_clock::now();
        double elapsedSeconds = chrono::duration<double>(currentTime - startTime).count();
        if (elapsedSeconds > timeoutSeconds)
        {
            return false; // 超时中断排序并返回失败标志
        }

        bool swapped = false;
        for (int j = 0; j < n - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }

            // 每处理1000个元素检查一次是否超时，减少时间检查开销
            if (j % 1000 == 0)
            {
                currentTime = chrono::steady_clock::now();
                elapsedSeconds = chrono::duration<double>(currentTime - startTime).count();
                if (elapsedSeconds > timeoutSeconds)
                {
                    return false; // 超时返回失败
                }
            }
        }
        
        // 优化：如果本轮没有交换操作，说明数组已经有序
        if (!swapped)
        {
            break;
        }
    }
    return true; // 排序成功完成
}

// 从指定文件路径读取整数数据
// 参数：文件路径字符串
// 返回：包含文件中所有整数的向量
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

// 对指定文件中的数据进行排序并计时
// 参数：待测试的数据文件路径
// 返回：排序耗时(毫秒)，超时或失败返回特定值
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

    // 记录排序开始时间点
    auto startTime = chrono::steady_clock::now();
    
    // 设置最大允许排序时间为200分钟
    const double timeoutSeconds = 12000.0;

    // 执行冒泡排序并监测是否超时
    bool completed = bubbleSort(data, startTime, timeoutSeconds);

    if (!completed)
    {
        cout << "  排序已超时（超过" << timeoutSeconds << "秒）" << endl;
        return timeoutSeconds * 1000; // 返回超时时间上限（毫秒）
    }

    // 记录排序结束时间点
    auto endTime = chrono::steady_clock::now();

    // 计算排序总耗时（微秒转换为毫秒）
    auto durationMicros = chrono::duration_cast<chrono::microseconds>(endTime - startTime).count();
    double durationMs = static_cast<double>(durationMicros) / 1000.0;

    // 验证排序结果正确性
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

// 将排序测试结果保存到文件并进行算法性能分析
// 参数：算法名称、数据规模列表、平均耗时映射、标准时间、是否为平方级复杂度
void exportSortingResults(const string &algorithmName,const vector<int> &dataSizes,const map<int, double> &avgTimes,double standardTime,bool isQuadratic)
{
    // 创建结果输出目录
    string resultDir = "d:\\chino_edu\\makenum\\result\\";
    fs::create_directories(resultDir);

    // 为当前算法创建单独的结果文件
    string resultFile = resultDir + algorithmName + "_results.txt";
    ofstream outFile(resultFile);

    if (!outFile.is_open())
    {
        cerr << "无法创建结果文件: " << resultFile << endl;
        return;
    }

    // 写入结果文件表头
    outFile << "========== " << algorithmName << "性能测试结果 ==========\n"
            << endl;
    outFile << "时间复杂度: " << (isQuadratic ? "O(n²)" : "O(n log n)") << endl
            << endl;
    outFile << "数据规模\t平均时间(毫秒)\t理论时间(毫秒)\t时间差异\t差异率(%)" << endl;
    outFile << "-----------------------------------------------------------------------------" << endl;

    // 写入各规模的详细测试数据
    for (int size : dataSizes)
    {
        if (avgTimes.find(size) != avgTimes.end())
        {
            double actualTime = avgTimes.at(size);
            
            // 处理超时情况的特殊显示
            if (actualTime >= 12000000.0)
            {
                outFile << setw(8) << size << "\t"
                        << "超时(>12000秒)" << "\t"
                        << "N/A\t"
                        << "N/A\t"
                        << "N/A" << endl;
                continue;
            }

            // 根据复杂度计算理论执行时间
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

            // 计算实际时间与理论时间的差异
            double timeDiff = actualTime - theoreticalTime;
            double diffRate = (theoreticalTime > 0) ? (timeDiff / theoreticalTime) * 100.0 : 0.0;

            // 格式化输出结果
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

    // 同时更新算法性能对比汇总文件
    string comparisonFile = resultDir + "sorting_algorithms_comparison.txt";
    bool fileExists = fs::exists(comparisonFile);

    ofstream compOutFile(comparisonFile, ios::app);
    if (!compOutFile.is_open())
    {
        cerr << "无法创建综合比较文件: " << comparisonFile << endl;
        return;
    }

    // 如果对比文件不存在，添加表头
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

    // 添加当前算法的性能数据到对比文件
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
    // 需要测试的数据规模集合
    vector<int> dataSizes = {250000, 500000, 750000};

    // 测试数据文件所在目录
    string basePath = "d:\\chino_edu\\makenum\\testnum\\";

    // 存储每个数据规模的所有测试结果
    map<int, vector<double>> testResults;

    // 基准时间（用于理论时间计算的参考值）
    double standardTime = 0.0;

    cout << "开始测试不同数据规模的冒泡排序性能...\n" << endl;

    // 对每个数据规模进行测试
    for (int size : dataSizes)
    {
        cout << "\n测试数据规模: " << size << " 个元素" << endl;

        // 根据数据规模决定使用的测试文件数量
        int numFiles;
        if (size == 100000)
        {
            numFiles = 5; // 100000规模使用5个测试文件
        }
        else if (size > 100000)
        {
            numFiles = 1;  // 大规模数据集使用1个测试文件
        }
        else
        {
            numFiles = 20; // 较小规模使用20个测试文件
        }

        string sizeFolder = "n_" + to_string(size);
        string folderPath = basePath + sizeFolder;

        for (int fileNum = 1; fileNum <= numFiles; fileNum++)
        {
            string filePath = folderPath + "\\data_" + to_string(fileNum) + ".txt";

            // 验证测试文件是否存在
            if (!fs::exists(filePath))
            {
                cerr << "  文件不存在: " << filePath << endl;
                continue;
            }

            double duration = testSortingFile(filePath);
            if (duration > 0)
            {
                testResults[size].push_back(duration);
                // 注意：这里原本有超时后跳出的代码被删除了
            }
        }
    }

    // 计算每个数据规模的平均排序时间
    map<int, double> avgTimes;
    for (const auto &[size, times] : testResults)
    {
        if (!times.empty())
        {
            // 检查该规模的测试中是否有超时情况
            bool hasTimeout = false;
            for (double time : times)
            {
                if (time >= 12000000.0) {
                    hasTimeout = true;
                    break;
                }
            }
            
            if (hasTimeout) {
                // 如果有任何一次测试超时，将整个规模标记为超时
                avgTimes[size] = 12000000.0;
            } else {
                // 计算所有测试的平均时间
                double total = 0.0;
                for (double time : times)
                {
                    total += time;
                }
                avgTimes[size] = total / times.size();
            }

            // 将100000规模的结果作为标准时间（若未超时）
            if (size == 100000 && !hasTimeout)
            {
                standardTime = avgTimes[size];
            }
        }
    }

    // 输出排序性能测试结果表格
    cout << "\n\n========== 冒泡排序性能测试结果 ==========\n" << endl;
    cout << "数据规模\t平均时间(毫秒)\t理论时间(毫秒)\t时间差异\t差异率(%)" << endl;
    cout << "-----------------------------------------------------------------------------" << endl;

    for (int size : dataSizes)
    {
        if (avgTimes.find(size) != avgTimes.end())
        {
            double actualTime = avgTimes[size];
            
            // 处理超时情况
            if (actualTime >= 12000000.0) {
                cout << setw(8) << size << "\t超时(>12000秒)\tN/A\tN/A\tN/A" << endl;
                continue;
            }

            // 计算基于O(n²)复杂度的理论时间
            double theoreticalTime = standardTime * pow(static_cast<double>(size) / 100000.0, 2);

            // 计算实际时间与理论时间的差异及百分比
            double timeDiff = actualTime - theoreticalTime;
            double diffRate = (theoreticalTime > 0) ? (timeDiff / theoreticalTime) * 100.0 : 0.0;

            // 格式化输出结果行
            cout << fixed << setprecision(2);
            cout << setw(8) << size << "\t"
                 << setw(12) << actualTime << "\t"
                 << setw(12) << theoreticalTime << "\t"
                 << setw(10) << timeDiff << "\t"
                 << setw(8) << diffRate << "%" << endl;
        }
    }
    
    // 导出详细结果到文件（冒泡排序是O(n²)复杂度）
    bool isQuadratic = true;
    exportSortingResults("bubbleSort", dataSizes, avgTimes, standardTime, isQuadratic);
    return 0;
}