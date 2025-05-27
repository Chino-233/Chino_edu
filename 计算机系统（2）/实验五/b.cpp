#include <sys/time.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <stdio.h> 
#include <algorithm> 
#define BLOCK_SIZE 64

int main(int argc, char *argv[]) 
{
    float *a, *b, *c, *b_transpose;
    long int i, j, k, size, m;
    struct timeval time1, time2; 
    
    if(argc<2) { 
        printf("\n\tUsage:%s <Row of square matrix>\n", argv[0]); 
        exit(-1); 
    }

    size = atoi(argv[1]);
    m = size*size;
    a = (float*)malloc(sizeof(float)*m); 
    b = (float*)malloc(sizeof(float)*m); 
    c = (float*)malloc(sizeof(float)*m);
    b_transpose = (float*)malloc(sizeof(float)*m); 

    // 初始化矩阵
    for(i=0; i<size; i++) { 
        for(j=0; j<size; j++) { 
            a[i*size+j] = (float)(rand()%1000/100.0); 
            b[i*size+j] = (float)(rand()%1000/100.0);
            c[i*size+j] = 0.0f;  
        } 
    }
    
    // 转置矩阵b来提高缓存局部性
    for(i=0; i<size; i++) {
        for(j=0; j<size; j++) {
            b_transpose[j*size+i] = b[i*size+j];
        }
    }
    
    gettimeofday(&time1, NULL);
    
    // 外层循环按块遍历
    for (i = 0; i < size; i += BLOCK_SIZE) {
        for (j = 0; j < size; j += BLOCK_SIZE) {
            for (k = 0; k < size; k += BLOCK_SIZE) {
                // 计算当前块的实际大小（处理边界情况）
                int i_end = std::min(i + BLOCK_SIZE, size);
                int j_end = std::min(j + BLOCK_SIZE, size);
                int k_end = std::min(k + BLOCK_SIZE, size);
                
                // 计算子块的矩阵乘法
                for (int ii = i; ii < i_end; ii++) {
                    for (int jj = j; jj < j_end; jj++) {
                        float sum = c[ii*size+jj]; 
                        for (int kk = k; kk < k_end; kk++) {
                            // 利用转置矩阵提高内层循环的缓存命中率
                            sum += a[ii*size+kk] * b_transpose[jj*size+kk];
                        }
                        c[ii*size+jj] = sum;
                    }
                }
            }
        }
    }
    
    gettimeofday(&time2, NULL);     
    
    time2.tv_sec -= time1.tv_sec; 
    time2.tv_usec -= time1.tv_usec; 
    if (time2.tv_usec < 0L) { 
        time2.tv_usec += 1000000L; 
        time2.tv_sec -= 1; 
    } 
   
    printf("Executiontime=%ld.%06ld seconds\n", time2.tv_sec, time2.tv_usec); 
    
    free(a);
    free(b);
    free(c);
    free(b_transpose);
    
    return 0;
}
