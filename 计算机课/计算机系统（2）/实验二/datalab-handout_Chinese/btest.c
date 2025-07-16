/* 
 * CS:APP Data Lab 
 * 
 * btest.c - 测试工具，用于检查学生在 bits.c 中的解决方案的正确性。
 *
 * 版权所有 (c) 2001-2011, R. Bryant and D. O'Hallaron，保留所有权利。
 * 未经许可不得使用、修改或复制。
 *
 * 这是 btest 的改进版本，它测试围绕整数谜题的零、tmin 和 tmax 的大窗口，
 * 以及围绕浮点谜题的零、规范化和非规范化边界。
 * 
 * 注意：不是64位安全的。始终使用 gcc -m32 选项编译。
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <signal.h>
#include <setjmp.h>
#include <math.h>
#include "btest.h"

/* 在某些 stdlib.h 文件中未声明，因此在此处定义 */
float strtof(const char *nptr, char **endptr);

/*************************
 * 配置常量
 *************************/

/* 通过设置执行时间的上限（以秒为单位）来处理无限循环 */
#define TIMEOUT_LIMIT 10

/* 对于具有单个参数的函数，在最小和最大测试值之上和之下，以及在
   零上下生成 TEST_RANGE 个值。具有两个或三个参数的函数将分别使用
   此值的平方根和立方根，以避免组合爆炸 */
#define TEST_RANGE 500000

/* 这定义了任何测试值数组的最大大小。gen_vals() 例程
   为 TEST_RANGE 的每个值创建 k 个测试值，因此 MAX_TEST_VALS 
   必须至少为 k*TEST_RANGE */
#define MAX_TEST_VALS 13*TEST_RANGE

/**********************************
 * 在其他模块中定义的全局变量 
 **********************************/
/* 这描述了要测试的谜题集合。
   在 decl.c 中定义，并从 ./puzzles 目录中的模板生成 */
extern test_rec test_set[]; 

/************************************************
 * 由命令行参数定义的只写一次全局变量
 ************************************************/

/* 以适合自动评分的格式输出结果，不显示
   和反例 */
static int grade = 0;

/* 在这么多秒后超时 */
static int timeout_limit = TIMEOUT_LIMIT; /* -T */

/* 如果非 NULL，则仅测试一个函数（-f） */
static char* test_fname = NULL;  

/* 仅使用固定参数的特殊情况（-1、-2 或 -3） */
static int has_arg[3] = {0,0,0};
static unsigned argval[3] = {0,0,0};

/* 为评级使用固定权重，如果是，应该是什么？（-r） */
static int global_rating = 0;

/******************
 * 辅助函数
 ******************/

/*
 * Signal - 安装信号处理程序
 */
typedef void handler_t(int);

handler_t *Signal(int signum, handler_t *handler) 
{
    struct sigaction action, old_action;

    action.sa_handler = handler;  
    sigemptyset(&action.sa_mask); /* 阻塞正在处理类型的信号 */
    action.sa_flags = SA_RESTART; /* 如果可能的话重新启动系统调用 */

    if (sigaction(signum, &action, &old_action) < 0)
    perror("Signal error");
    return (old_action.sa_handler);
}

/* 
 * timeout_handler - SIGALARM 处理程序 
 */
sigjmp_buf envbuf;
void timeout_handler(int sig) {
    siglongjmp(envbuf, 1);
}

/* 
 * random_val - 返回 min 和 max 之间的随机整数值 
 */
static int random_val(int min, int max)
{
    double weight = rand()/(double) RAND_MAX;
    int result = min * (1-weight) + max * weight;
    return result;
}

/* 
 * gen_vals - 生成我们将用于测试函数的整数值 
 */
static int gen_vals(int test_vals[], int min, int max, int test_range, int arg)
{
    int i;
    int test_count = 0;

    /* 特殊情况：如果用户使用 -1、-2 或 -3 标志指定了特定函数
       参数，则简单地使用此参数并返回 */
    if (has_arg[arg]) {
    test_vals[0] = argval[arg];
    return 1;
    }

    /* 
     * 特殊情况：为浮点函数生成测试值，
     * 其中输入参数是浮点数的无符号位级
     * 表示。对于这种情况，我们想测试
     * 零周围区域、最小标准化和最大非标准化数字、
     * 一，以及最大标准化数，以及 inf 和 nan。
     */
    if ((min == 1 && max == 1)) { 
    unsigned smallest_norm = 0x00800000;
    unsigned one = 0x3f800000;
    unsigned largest_norm = 0x7f000000;
    
    unsigned inf = 0x7f800000;
    unsigned nan =  0x7fc00000;
    unsigned sign = 0x80000000;

    /* 测试范围最多应该是一个指数值范围的一半 */
    if (test_range > (1 << 23)) {
        test_range = 1 << 23;
    }
    
    /* 函数的输入参数是浮点数的无符号位级
       表示。该循环体内生成的测试次数是
       全局变量 MAX_TEST_VALS 注释中引用的值 k。*/

    for (i = 0; i < test_range; i++) {
        /* 零周围的非规范化数 */
        test_vals[test_count++] = i; 
        test_vals[test_count++] = sign | i;
        
        /* 规范到非规范过渡区域 */
        test_vals[test_count++] = smallest_norm + i;
        test_vals[test_count++] = smallest_norm - i;
        test_vals[test_count++] = sign | (smallest_norm + i);
        test_vals[test_count++] = sign | (smallest_norm - i);
        
        /* 1周围区域 */
        test_vals[test_count++] = one + i;
        test_vals[test_count++] = one - i;
        test_vals[test_count++] = sign | (one + i);
        test_vals[test_count++] = sign | (one - i);
        
        /* 最大规范化数字以下区域 */
        test_vals[test_count++] = largest_norm - i; 
        test_vals[test_count++] = sign | (largest_norm - i); 
    }
    
    /* 特殊值 */
    test_vals[test_count++] = inf;        /* 无穷大 */
    test_vals[test_count++] = sign | inf; /* 负无穷大 */
    test_vals[test_count++] = nan;        /* 非数 */
    test_vals[test_count++] = sign | nan; /* 负非数 */

    return test_count;
    }


    /*
     * 正常情况：为整数函数生成测试值
     */

    /* 如果范围足够小，则详尽测试 */
    if (max - MAX_TEST_VALS <= min) {
    for (i = min; i <= max; i++)
        test_vals[test_count++] = i;
    return test_count;
    }

    /* 否则，需要抽样。在边界附近、零周围
       以及一些随机情况进行测试。 */
    for (i = 0; i < test_range; i++) {

    /* 测试边界周围 */
    test_vals[test_count++] = min + i;
    test_vals[test_count++] = max - i;

    /* 如果零落在 min 和 max 之间，也测试零周围 */
    if (i >= min && i <= max)
        test_vals[test_count++] = i;
    if (-i >= min && -i <= max)
        test_vals[test_count++] = -i;

    /* min 和 max 之间的随机情况 */
    test_vals[test_count++] = random_val(min, max);

    }
    return test_count;
}

/* 
 * test_0_arg - 测试没有参数的函数 
 */
static int test_0_arg(funct_t f, funct_t ft, char *name)
{
    int r = f();
    int rt = ft();
    int error =  (r != rt);

    if (error && !grade)
    printf("错误：测试 %s() 失败...\n...给出 %d[0x%x]。应该是 %d[0x%x]\n", name, r, r, rt, rt);

    return error;
}

/* 
 * test_1_arg - 测试具有一个参数的函数 
 */
static int test_1_arg(funct_t f, funct_t ft, int arg1, char *name)
{
    funct1_t f1 = (funct1_t) f;
    funct1_t f1t = (funct1_t) ft;
    int r, rt, error;

    r = f1(arg1);
    rt = f1t(arg1);
    error = (r != rt);
    if (error && !grade)
    printf("错误：测试 %s(%d[0x%x]) 失败...\n...给出 %d[0x%x]。应该是 %d[0x%x]\n", name, arg1, arg1, r, r, rt, rt);

    return error;
}

/* 
 * test_2_arg - 测试具有两个参数的函数 
 */
static int test_2_arg(funct_t f, funct_t ft, int arg1, int arg2, char *name)
{
    funct2_t f2 = (funct2_t) f;
    funct2_t f2t = (funct2_t) ft;
    int r = f2(arg1, arg2);
    int rt = f2t(arg1, arg2);
    int error = (r != rt);

    if (error && !grade)
    printf("错误：测试 %s(%d[0x%x],%d[0x%x]) 失败...\n...给出 %d[0x%x]。应该是 %d[0x%x]\n", name, arg1, arg1, arg2, arg2, r, r, rt, rt);

    return error;
}

/* 
 * test_3_arg - 测试具有三个参数的函数 
 */
static int test_3_arg(funct_t f, funct_t ft, 
              int arg1, int arg2, int arg3, char *name)
{
    funct3_t f3 = (funct3_t) f;
    funct3_t f3t = (funct3_t) ft;
    int r = f3(arg1, arg2, arg3);
    int rt = f3t(arg1, arg2, arg3);
    int error = (r != rt);

    if (error && !grade)
    printf("错误：测试 %s(%d[0x%x],%d[0x%x],%d[0x%x]) 失败...\n...给出 %d[0x%x]。应该是 %d[0x%x]\n", name, arg1, arg1, arg2, arg2, arg3, arg3, r, r, rt, rt);

    return error;
}

/* 
 * test_function - 测试一个函数。返回错误数量 
 */
static int test_function(test_ptr t) {
    int test_counts[3];    /* 每个参数的测试值数量 */
    int args = t->args;    /* 函数参数的数量 */
    int arg_test_range[3]; /* 每个参数的测试范围 */
    int i, a1, a2, a3;        
    int errors = 0;

    /* 这些是每个参数的测试值。使用静态属性声明，
       以便数组将被分配在 bss 而不是堆栈中 */
    static int arg_test_vals[3][MAX_TEST_VALS]; 

    /* 对参数数量的健全性检查 */
    if (args < 0 || args > 3) {
    printf("配置错误：函数 %s 的参数数量无效 (%d)\n", args, t->name);
    exit(1);
    }

    /* 分配参数测试值的范围，以保持测试总数独立于参数数量 */
    if (args == 1) {
    arg_test_range[0] = TEST_RANGE;
    }
    else if (args == 2) {
    arg_test_range[0] = pow((double)TEST_RANGE, 0.5);  /* 平方根 */
    arg_test_range[1] = arg_test_range[0];
    }
    else {
    arg_test_range[0] = pow((double)TEST_RANGE, 0.333); /* 立方根 */
    arg_test_range[1] = arg_test_range[0];
    arg_test_range[2] = arg_test_range[0];
    }

    /* 对范围的健全性检查 */
    if (arg_test_range[0] < 1)
    arg_test_range[0] = 1;
    if (arg_test_range[1] < 1) 
    arg_test_range[1] = 1;
    if (arg_test_range[2] < 1) 
    arg_test_range[2] = 1;

    /* 为每个参数创建测试集 */
    for (i = 0; i < args; i++) {
    test_counts[i] =  gen_vals(arg_test_vals[i], 
                   t->arg_ranges[i][0], /* 最小值 */
                   t->arg_ranges[i][1], /* 最大值 */
                   arg_test_range[i],   
                   i);

    }

    /* 处理测试代码中的超时 */
    if (timeout_limit > 0) {
    int rc;
    rc = sigsetjmp(envbuf, 1);
    if (rc) {
        /* 如果有超时，控制将到达这里 */
        errors = 1;
        printf("错误：测试 %s 失败。\n  在 %d 秒后超时（可能是无限循环）\n", t->name, timeout_limit);
        return errors;
    }
    alarm(timeout_limit);
    }


    /* 测试函数没有参数 */
    if (args == 0) {
    errors += test_0_arg(t->solution_funct, t->test_funct, t->name);
    return errors;
    } 

    /* 
     * 测试函数至少有一个参数 
     */
      
    /* 迭代第一个参数的值 */

    for (a1 = 0; a1 < test_counts[0]; a1++) {
    if (args == 1) {
        errors += test_1_arg(t->solution_funct, 
                 t->test_funct,
                 arg_test_vals[0][a1],
                 t->name);

        /* 如果有错误则停止测试 */
        if (errors)
        return errors;
    } 
    else {
        /* 如有必要，迭代第二个参数的值 */
        for (a2 = 0; a2 < test_counts[1]; a2++) {
        if (args == 2) {
            errors += test_2_arg(t->solution_funct, 
                     t->test_funct,
                     arg_test_vals[0][a1], 
                     arg_test_vals[1][a2],
                     t->name);

            /* 如果有错误则停止测试 */
            if (errors)
            return errors;
        } 
        else {
            /* 如有必要，迭代第三个参数的值 */
            for (a3 = 0; a3 < test_counts[2]; a3++) {
            errors += test_3_arg(t->solution_funct, 
                         t->test_funct,
                         arg_test_vals[0][a1], 
                         arg_test_vals[1][a2],
                         arg_test_vals[2][a3],
                         t->name);
            
            /* 如果有错误则停止测试 */
            if (errors)
                return errors;
            } /* a3 */
        }
        } /* a2 */
    }
    } /* a1 */

    
    return errors;
}

/* 
 * run_tests - 运行一系列测试。返回错误数量 
 */ 
static int run_tests() 
{
    int i;
    int errors = 0;
    double points = 0.0;
    double max_points = 0.0;

    printf("分数\t评级\t错误\t函数\n");

    for (i = 0; test_set[i].solution_funct; i++) {
    int terrors;
    double tscore;
    double tpoints;
    if (!test_fname || strcmp(test_set[i].name,test_fname) == 0) {
        int rating = global_rating ? global_rating : test_set[i].rating;
        terrors = test_function(&test_set[i]);
        errors += terrors;
        tscore = terrors == 0 ? 1.0 : 0.0;
        tpoints = rating * tscore;
        points += tpoints;
        max_points += rating;

        if (grade || terrors < 1)
        printf(" %.0f\t%d\t%d\t%s\n", 
               tpoints, rating, terrors, test_set[i].name);

    }
    }

    printf("总分：%.0f/%.0f\n", points, max_points);
    return errors;
}

/* 
 * get_num_val - 从字符串中提取十六进制/十进制/或浮点值 
 */
static int get_num_val(char *sval, unsigned *valp) {
    char *endp;

    /* 看看它是整数还是浮点数 */
    int ishex = 0;
    int isfloat = 0;
    int i;
    for (i = 0; sval[i]; i++) {
    switch (sval[i]) {
    case 'x':
    case 'X':
        ishex = 1;
        break;
    case 'e':
    case 'E':
        if (!ishex)
        isfloat = 1;
        break;
    case '.':
        isfloat = 1;
        break;
    default:
        break;
    }
    }
    if (isfloat) {
    float fval = strtof(sval, &endp);
    if (!*endp) {
        *valp = *(unsigned *) &fval;
        return 1;
    }
    return 0;
    } else {
    long long int llval = strtoll(sval, &endp, 0);
    long long int upperbits = llval >> 31;
    /* 负数将给出 -1，正数将给出 0 或 1 */
    if (!*valp && (upperbits == 0 || upperbits == -1 || upperbits == 1)) {
        *valp = (unsigned) llval;
        return 1;
    }
    return 0;
    }
}


/* 
 * usage - 显示使用信息
 */
static void usage(char *cmd) {
    printf("使用方法：%s [-hg] [-r <n>] [-f <name> [-1|-2|-3 <val>]*] [-T <time limit>]\n", cmd);
    printf("  -1 <val>  指定第一个函数参数\n");
    printf("  -2 <val>  指定第二个函数参数\n");
    printf("  -3 <val>  指定第三个函数参数\n");
    printf("  -f <name> 仅测试指定的函数\n");
    printf("  -g        用于评分的简洁输出（没有错误消息）\n");
    printf("  -h        打印此消息\n");
    printf("  -r <n>    为所有问题赋予统一权重 n\n");
    printf("  -T <lim>  设置超时限制为 lim\n");
    exit(1);
}


/************** 
 * 主程序 
 **************/

int main(int argc, char *argv[])
{
    char c;

    /* 解析命令行参数 */
    while ((c = getopt(argc, argv, "hgf:r:T:1:2:3:")) != -1)
        switch (c) {
        case 'h': /* 帮助 */
        usage(argv[0]);
        break;
    case 'g': /* 自动评分的评分选项 */
        grade = 1;
        break;
    case 'f': /* 只测试一个函数 */
        test_fname = strdup(optarg);
        break;
    case 'r': /* 为每个问题设置全局评级 */
        global_rating = atoi(optarg);
        if (global_rating < 0)
        usage(argv[0]);
        break;
    case '1': /* 获取第一个参数 */
        has_arg[0] = get_num_val(optarg, &argval[0]);
        if (!has_arg[0]) {
        printf("错误参数 '%s'\n", optarg);
        exit(0);
        }
        break;
    case '2': /* 获取第二个参数 */
        has_arg[1] = get_num_val(optarg, &argval[1]);
        if (!has_arg[1]) {
        printf("错误参数 '%s'\n", optarg);
        exit(0);
        }
        break;
    case '3': /* 获取第三个参数 */
        has_arg[2] = get_num_val(optarg, &argval[2]);
        if (!has_arg[2]) {
        printf("错误参数 '%s'\n", optarg);
        exit(0);
        }
        break;
    case 'T': /* 设置超时限制 */
        timeout_limit = atoi(optarg);
        break;
    default:
        usage(argv[0]);
    }

    if (timeout_limit > 0) {
    Signal(SIGALRM, timeout_handler);
    }

    /* 测试每个函数 */
    run_tests();

    return 0;
}