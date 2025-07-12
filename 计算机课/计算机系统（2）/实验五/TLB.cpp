#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <windows.h>

double now_sec() {
    LARGE_INTEGER frequency, counter;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&counter);
    return (double)counter.QuadPart / frequency.QuadPart;
}

int main() {
    // 获取系统页面大小
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    const size_t page_size = sysInfo.dwPageSize;
    
    const size_t max_pages = 4096;  // 最多测试到 4096 页
    const int repeats = 5;

    printf("num_pages\tlatency (ns)\n");
    for (size_t np = 8; np <= max_pages; np *= 2) {
        double total_ns = 0;
        for (int r = 0; r < repeats; r++) {
            size_t bytes = np * page_size;
            uint8_t *A = (uint8_t*)VirtualAlloc(NULL, bytes,
                                MEM_COMMIT | MEM_RESERVE,
                                PAGE_READWRITE);
            if (A == NULL) { 
                fprintf(stderr, "VirtualAlloc failed: %lu\n", GetLastError()); 
                return 1; 
            }
            
            // 预热
            for (size_t i = 0; i < bytes; i += page_size)
                A[i]++;

            double t0 = now_sec();
            size_t accesses = 0;
            volatile uint8_t sink;
            for (size_t i = 0; i < bytes; i += page_size) {
                sink = A[i];
                accesses++;
            }
            double t1 = now_sec();
            total_ns += (t1 - t0) * 1e9 / accesses;

            VirtualFree(A, 0, MEM_RELEASE);
        }
        printf("%9zu\t%12.2f\n", np, total_ns / repeats);
    }
    return 0;
}
