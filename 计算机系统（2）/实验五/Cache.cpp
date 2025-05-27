#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdint.h>

#define ARRAY_SIZE (32 * 1024 * 1024)  

double now_sec() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec * 1e-6;
}

int main() {
    uint8_t *array = (uint8_t *)malloc(ARRAY_SIZE);
    if (!array) {
        perror("malloc");
        return 1;
    }

    printf("stride(bytes)\tavg latency (ns)\n");
    for (size_t stride = 4; stride <= 1024; stride *= 2) {
        // 预热
        for (size_t i = 0; i < ARRAY_SIZE; i += stride) {
            array[i]++;
        }
        // 测量
        double t0 = now_sec();
        size_t accesses = 0;
        volatile uint8_t sink;
        for (size_t i = 0; i < ARRAY_SIZE; i += stride) {
            sink = array[i];
            accesses++;
        }
        double t1 = now_sec();
        double avg_ns = (t1 - t0) * 1e9 / accesses;
        printf("%8zu\t%10.2f\n", stride, avg_ns);
    }

    free(array);
    return 0;
}
