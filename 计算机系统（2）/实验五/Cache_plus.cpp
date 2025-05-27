// capacity-test.c
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <chrono>   // 使用C++高精度计时
#include <random>   // 用于生成随机访问序列

// 使用C++的高精度计时器
double now_sec() {
    auto now = std::chrono::high_resolution_clock::now();
    auto now_ns = std::chrono::time_point_cast<std::chrono::nanoseconds>(now);
    auto epoch = now_ns.time_since_epoch();
    auto value = std::chrono::duration_cast<std::chrono::nanoseconds>(epoch);
    return value.count() * 1e-9;
}

int main() {
    const size_t max_size = 64 * 1024 * 1024; // 64 MB
    const size_t line_size = 64;              // 假设 Cache 行 64B
    const int repeats = 5;                    // 保持原有的重复次数
    
    // 初始化随机数生成器
    std::mt19937 gen(std::random_device{}());

    printf("working_set(KB)\tavg latency (ns)\n");
    for (size_t S = 1 << 10; S <= max_size; S <<= 1) {
        double total_ns = 0;
        
        for (int r = 0; r < repeats; r++) {
            uint8_t *A = (uint8_t *)malloc(S);
            if (!A) { 
                printf("malloc failed\n"); 
                return 1; 
            }
            
            // 触碰所有内存页面，确保物理内存分配
            for (size_t i = 0; i < S; i += 4096) {
                A[i] = 1;
            }

            // 初始化内存
            for (size_t i = 0; i < S; i++) {
                A[i] = (uint8_t)i;
            }
            
            // 创建随机访问序列
            size_t num_lines = S / line_size;
            size_t *indices = (size_t *)malloc(num_lines * sizeof(size_t));
            if (!indices) {
                printf("malloc failed for indices\n");
                free(A);
                return 1;
            }
            
            // 初始化访问序列
            for (size_t i = 0; i < num_lines; i++) {
                indices[i] = i * line_size;
            }
            
            // 随机打乱访问序列（借鉴参考代码的随机化思路）
            std::uniform_int_distribution<> dist(0, num_lines - 1);
            for (size_t i = 0; i < num_lines; i++) {
                size_t j = dist(gen) % num_lines;
                size_t temp = indices[i];
                indices[i] = indices[j];
                indices[j] = temp;
            }
            
            // 预热缓存，使用随机访问序列
            volatile uint8_t pre_sink;
            for (size_t i = 0; i < num_lines; i++) {
                pre_sink = A[indices[i]];
            }
            
            // 测量随机访问时间
            double t0 = now_sec();
            size_t accesses = 0;
            volatile uint8_t sink;
            
            // 根据数组大小调整迭代次数，小数组多次迭代
            int iterations = (S < (1 << 16)) ? 5 : 1;
            
            for (int iter = 0; iter < iterations; iter++) {
                for (size_t i = 0; i < num_lines; i++) {
                    sink = A[indices[i]];
                    accesses++;
                }
            }
            
            double t1 = now_sec();
            double elapsed = t1 - t0;
            
            if (accesses > 0 && elapsed > 0) {
                total_ns += (elapsed * 1e9) / accesses;
            }
            
            free(indices);
            free(A);
        }
        
        // 保持原来的输出格式
        printf("%12zu\t%12.2f\n", S / 1024, total_ns / repeats);
    }
    return 0;
}