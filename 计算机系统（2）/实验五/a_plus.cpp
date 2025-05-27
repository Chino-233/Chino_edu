#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Windows环境下运行一次测试并返回执行时间
double run_test(const char* program, int size) {
    char command[100];
    char result[100] = {0};
    long int seconds, microseconds;
    
    // 构建命令行（Windows环境不需要./前缀）
    sprintf(command, "%s %d", program, size);
    
    // 打开管道以捕获命令输出
    FILE* pipe = _popen(command, "r");
    if (!pipe) {
        printf("无法执行命令: %s\n", command);
        return -1;
    }
    
    // 读取命令输出
    fgets(result, sizeof(result), pipe);
    _pclose(pipe);
    
    // 解析输出中的时间（格式：Executiontime=X.XXXXXX seconds）
    sscanf(result, "Executiontime=%ld.%06ld seconds", &seconds, &microseconds);
    return seconds + microseconds/1000000.0;
}

int main() {
    int sizes[] = {2000};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    int test_times = 3; // 每个大小测试3次
    double normal_times[7] = {0}, optimized_times[7] = {0}, speedup[7] = {0};
    int i, j;
    
    printf("矩阵乘法性能测试开始...\n\n");
    
    // 编译程序（Windows环境）
    system("gcc -o a_test.exe a.cpp");
    system("gcc -o b_test.exe b.cpp");
    
    // 测试不同大小的矩阵
    for(i=0; i<num_sizes; i++) {
        int size = sizes[i];
        double normal_total = 0.0, optimized_total = 0.0;
        
        printf("测试矩阵大小: %d x %d\n", size, size);
        
        // 运行三次普通矩阵乘法，计算平均时间
        printf("  运行普通矩阵乘法...\n");
        for(j=0; j<test_times; j++) {
            double time = run_test("a_test.exe", size);
            printf("    测试 %d: %ld.%06ld 秒\n", j+1, 
                   (long)time, (long)((time - (long)time) * 1000000));
            normal_total += time;
        }
        normal_times[i] = normal_total / test_times;
        printf("  普通矩阵乘法平均时间: %ld.%06ld 秒\n", 
               (long)normal_times[i], (long)((normal_times[i] - (long)normal_times[i]) * 1000000));
        
        // 运行三次优化矩阵乘法，计算平均时间
        printf("  运行优化矩阵乘法...\n");
        for(j=0; j<test_times; j++) {
            double time = run_test("b_test.exe", size);
            printf("    测试 %d: %ld.%06ld 秒\n", j+1, 
                   (long)time, (long)((time - (long)time) * 1000000));
            optimized_total += time;
        }
        optimized_times[i] = optimized_total / test_times;
        printf("  优化矩阵乘法平均时间: %ld.%06ld 秒\n", 
               (long)optimized_times[i], (long)((optimized_times[i] - (long)optimized_times[i]) * 1000000));
        
        // 计算加速比
        speedup[i] = normal_times[i] / optimized_times[i];
        printf("  加速比: %.4f\n\n", speedup[i]);
    }
    
    // 输出最终结果表格
    printf("\n最终结果:\n");
    printf("表1、普通矩阵乘法与优化后矩阵乘法之间的性能对比\n");
    printf("矩阵大小\t100\t500\t1000\t1500\t2000\t2500\t3000\n");
    
    printf("一般算法执行时间\t");
    for(i=0; i<num_sizes; i++) {
        printf("%ld.%06ld\t", (long)normal_times[i], 
               (long)((normal_times[i] - (long)normal_times[i]) * 1000000));
    }
    printf("\n");
    
    printf("优化算法执行时间\t");
    for(i=0; i<num_sizes; i++) {
        printf("%ld.%06ld\t", (long)optimized_times[i], 
               (long)((optimized_times[i] - (long)optimized_times[i]) * 1000000));
    }
    printf("\n");
    
    printf("加速比(speedup)\t");
    for(i=0; i<num_sizes; i++) {
        printf("%.4f\t", speedup[i]);
    }
    printf("\n");
    
    return 0;
}