/*
 * CS:APP Data Lab
 */

/* 声明不同的函数类型 */
typedef int (*funct_t) (void);
typedef int (*funct1_t)(int);
typedef int (*funct2_t)(int, int); 
typedef int (*funct3_t)(int, int, int); 

/* 将有关函数及其测试的所有信息组合为结构 */
typedef struct {
    char *name;             /* 字符串名称 */
    funct_t solution_funct; /* 函数 */
    funct_t test_funct;     /* 测试函数 */
    int args;               /* 函数参数数量 */
    char *ops;              /* 合法操作符列表。特殊情况："$" 表示浮点数 */
    int op_limit;           /* 解决方案中允许的最大操作数 */
    int rating;             /* 问题评分（1 -- 4） */
    int arg_ranges[3][2];   /* 参数范围。始终为3个参数定义，即使 */
                            /* 函数接受的参数更少。特殊情况：第一个参数 */
                /* 对于浮点谜题必须设置为 {1,1} */
} test_rec, *test_ptr;

extern test_rec test_set[];