/*
 * CS:APP Data Lab
 *
 * <请在此处填写你的姓名和用户ID>
 *
 * bits.c - 包含您对实验解答的源文件。
 *          这是您将提交给教师的文件。
 *
 * 警告：请不要包含 <stdio.h> 头文件；它会让 dlc
 * 编译器混淆。您仍然可以在不包含 <stdio.h> 的情况下使用 printf 进行调试，
 * 尽管可能会收到编译器警告。一般来说，
 * 忽略编译器警告不是好的做法，但在这种情况下是可以的。
 */

/*
 * 给学生的指导：
 *
 * 第1步：仔细阅读以下说明。
 */

// 您将通过编辑此源文件中的函数集合来提供 Data Lab 的解决方案。

// 整数编码规则：

//   用一行或多行C代码替换每个函数中的"return"语句，以实现该函数。您的代码
//   必须符合以下风格：

//   int Funct(arg1, arg2, ...) {
//       /* 简要描述您的实现如何工作 */
//       int var1 = Expr1;
//       ...
//       int varM = ExprM;

//       varJ = ExprJ;
//       ...
//       varN = ExprN;
//       return ExprR;
//   }

//   每个"Expr"是仅使用以下内容的表达式：
//   1. 整数常量0到255（0xFF）。您不允许
//      使用大常量，如0xffffffff。
//   2. 函数参数和局部变量（没有全局变量）。
//   3. 一元整数运算 ! ~
//   4. 二进制整数运算 & ^ | + << >>

//   有些问题进一步限制了允许使用的运算符集。
//   每个"Expr"可以由多个运算符组成。您不限于每行一个运算符。

//   明确禁止您：
//   1. 使用任何控制结构，如 if, do, while, for, switch 等。
//   2. 定义或使用任何宏。
//   3. 在此文件中定义任何其他函数。
//   4. 调用任何函数。
//   5. 使用任何其他运算，如 &&, ||, -, 或 ?:
//   6. 使用任何形式的类型转换。
//   7. 使用除 int 之外的任何数据类型。这意味着您不能使用数组、结构体或联合体。

//   您可以假设您的机器：
//   1. 使用2的补码，32位表示整数。
//   2. 执行算术右移。
//   3. 当移位量小于0或大于31时，移位行为是不可预测的。

// 可接受的编码风格示例：
//   /*
//    * pow2plus1 - 返回 2^x + 1，其中 0 <= x <= 31
//    */
//   int pow2plus1(int x) {
//      /* 利用移位能力计算2的幂 */
//      return (1 << x) + 1;
//   }

//   /*
//    * pow2plus4 - 返回 2^x + 4，其中 0 <= x <= 31
//    */
//   int pow2plus4(int x) {
//      /* 利用移位能力计算2的幂 */
//      int result = (1 << x);
//      result += 4;
//      return result;
//   }

// 浮点编码规则

// 对于要求您实现浮点运算的问题，编码规则不那么严格。
// 允许使用循环和条件控制。允许使用 int 和 unsigned。
// 可以使用任意整数和无符号常量。可以对 int 或 unsigned 数据使用任何算术、
// 逻辑或比较运算。

// 明确禁止您：
//   1. 定义或使用任何宏。
//   2. 在此文件中定义任何其他函数。
//   3. 调用任何函数。
//   4. 使用任何形式的类型转换。
//   5. 使用除 int 或 unsigned 之外的任何数据类型。这意味着您
//      不能使用数组、结构体或联合体。
//   6. 使用任何浮点数据类型、运算或常量。

// 注意：
//   1. 使用 dlc（数据实验检查器）编译器（在讲义中描述）检查
//      您解决方案的合法性。
//   2. 每个函数都有最大运算符数量（整数、逻辑或比较）限制，
//      您可以在实现该函数时使用。dlc 会检查最大运算符数量。
//      注意，赋值（'='）不计算在内；您可以根据需要使用任意多的赋值而不受惩罚。
//   3. 使用 btest 测试工具检查您函数的正确性。
//   4. 使用 BDD 检查器正式验证您的函数。
//   5. 每个函数的最大操作数在每个函数的头注释中给出。如果在讲义和此文件中
//      关于最大操作数有任何不一致，请以此文件为权威来源。

/*
 * 第2步：根据编码规则修改以下函数。
 *
 *   重要提示。为避免评分意外：
 *   1. 使用 dlc 编译器检查您的解决方案是否符合编码规则。
 *   2. 使用 BDD 检查器正式验证您的解决方案是否产生正确的答案。
 */
// 1
/*
 * bitXor - 仅允许使用~和&来实现异或
 *   例子: bitXor(4, 5) = 1
 *   允许的操作符: ~ &
 *   最多操作符数目: 14
 *   分值: 1
 */
int bitXor(int x,int y){
    return ~(~(~x & y) & ~(x & ~y));
}


/*
 * tmin - 返回最小的二进制补码
 *   允许的操作符: ! ~ & ^ | + << >>
 *   最多操作符数目: 4
 *   分值: 1
 */
int tmin(void)
{
    return 1 << 31;
}
// 2
/*
 * isTmax - 如果x是最大的二进制补码，返回1；否则，返回0
 *   允许的操作符: ! ~ & ^ | +
 *   最多操作符数目: 10
 *   分值: 2
 */
int isTmax(int x)
{
    return !((x + 1) ^ (~x)) & !!(x + 1);
}


/*
 * allOddBits - 如果所有奇数位都为1则返回1;否则返回0
 *   例子： allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
 *   允许的操作符: ! ~ & ^ | + << >>
 *   最多操作符数目: 12
 *   分值: 2
 */
int allOddBits(int x)
{
    int mask = 0xAA;
    mask = mask | (mask << 8);
    mask = mask | (mask << 16);
    return !((x & mask) ^ mask);
}


/*
 * negate - 返回-x
 *   例子: negate(1) = -1.
 *   允许的操作符: ! ~ & ^ | + << >>
 *   最多操作符数目: 5
 *   分值: 2
 */
int negate(int x)
{
    return ~x + 1;
}


// 3
/*
 * isAsciiDigit - 如果x是ascii码中的0~9，返回1;否则返回0
 *   例子: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   允许的操作符: ! ~ & ^ | + << >>
 *   最多操作符数目: 15
 *   分值: 3
 */
int isAsciiDigit(int x)
{
    int lowerBound = x + (~0x30 + 1); //x-0x30
    int upperBound = 0x39 + (~x + 1); //x-0x39
    return !(lowerBound >> 31) & !(upperBound >> 31);
}


/*
 * conditional - 实现x?y:z
 *   例子: conditional(2,4,5) = 4
 *   允许的操作符: ! ~ & ^ | + << >>
 *   最多操作符数目: 16
 *   分值: 3
 */
int conditional(int x, int y, int z)
{
    int mask = (!x) + (~0); 
    return (mask & y) | (~mask & z);
}


/*
 * isLessOrEqual - 如果x<=y返回1否则返回0
 *   例子: isLessOrEqual(4,5) = 1.
 *   允许的操作符: ! ~ & ^ | + << >>
 *   最多操作符数目: 24
 *   分值: 3
 */
int isLessOrEqual(int x, int y)
{
    int signX = x >> 31;
    int signY = y >> 31;
    return (signX & !signY) | ((!(signX ^ signY)) & ((x + ~y) >> 31 & 1)) | !(x ^ y); 
}


// 4
/*
 * logicalNeg - 实现！运算符的功能
 *   例子: logicalNeg(3) = 0, logicalNeg(0) = 1
 *   允许的操作符: ~ & ^ | + << >>
 *   最多操作符数目: 12
 *   分值: 4
 */

int logicalNeg(int x)
{
    return ((x | (~x + 1)) >> 31) + 1;
}


/* howManyBits - 返回将X表示为补码所需的最小有效位数。
 *  例子: howManyBits(12) = 5
 *            howManyBits(298) = 10
 *            howManyBits(-5) = 4
 *            howManyBits(0)  = 1
 *            howManyBits(-1) = 1
 *            howManyBits(0x80000000) = 32
 *  允许的操作符: ! ~ & ^ | + << >>
 *  最多操作符数目: 90
 *  分值: 4
 */
int howManyBits(int x) {
    int shift1, shift2, shift4, shift8, shift16;
    int sum;
    int t = ((!x) << 31) >> 31;     //如果x为0，则t全为1，否则全为0
    int t2 = ((!~x) << 31) >> 31;   //如果x为-1，则t2全为1，否则全为0
    int op = x ^ ((x >> 31));       //如果x为负，则取反；如果x为正，保持不变
    shift16 = (!!(op >> 16)) << 4;
    op = op >> shift16;
    shift8 = (!!(op >> 8)) << 3;
    op = op >> shift8;
    shift4 = (!!(op >> 4)) << 2;
    op = op >> shift4;
    shift2 = (!!(op >> 2)) << 1;
    op = op >> shift2;
    shift1 = (!!(op >> 1));
    op = op >> shift1;
    sum = 2 + shift16 + shift8 + shift4 + shift2 + shift1;
    //对于特殊情况：0返回1，-1返回1，最小负数返回32
    return (t2 & 1) | ((~t2) & ((t & 1) | ((~t) & sum)));
}

// float
/*
 * floatScale2 - 以unsinged表示的浮点数二进制的二倍的二进制unsigned型
 *   参数和结果都会被作为unsigned返回，但是会表示为二进制的单精度浮点值。
 *   允许的操作符: 任何整数或者无符号数操作符包括： ||, &&. also if, while
 *   最多操作符数目: 30
 *   分值: 4
 */
unsigned floatScale2(unsigned uf)
{
    unsigned sign = uf & 0x80000000;
    unsigned exp = (uf & 0x7F800000) >> 23;
    unsigned frac = uf & 0x007FFFFF;
    if (exp == 0xFF)
    {
        return uf; 
    }
    if (exp == 0)
    {
        frac <<= 1;
        return sign | frac;
    }
    exp++;
    if (exp == 0xFF)
    {
        return sign | 0x7F800000; 
    }
    return sign | (exp << 23) | frac;
}


/*
 * floatFloat2Int - 返回unsigned uf的整型数的二进制形式
 *   参数和结果都会被作为unsigned返回，但是会表示为二进制的单精度浮点值
 *   任何超过范围的数都应该返回 0x80000000u.
 *   允许的操作符: 任何整数或者无符号数操作符包括： ||, &&. also if, while
 *   最多操作符数目: 30
 *   分值: 4
 */
int floatFloat2Int(unsigned uf)
{
    unsigned sign = uf & 0x80000000;
    unsigned exp = (uf & 0x7F800000) >> 23;
    unsigned frac = uf & 0x007FFFFF;
    if (exp == 0xFF)
    {
        return 0x80000000u; 
    }
    if (exp < 127)
    {
        return 0; 
    }
    int e = exp - 127;
    if (e >= 31)
    {
        return 0x80000000u; 
    }
    frac |= 0x00800000; 
    if (e > 23)
    {
        frac <<= (e - 23);
    }
    else
    {
        frac >>= (23 - e);
    }
    if (sign)
    {
        return ~frac + 1;
    }
    else
    {
        return frac;
    }
}


/*
 * floatPower2 - 返回2的x次方的浮点数值
 *   (无符号)二进制表示形式
 *   如果结果太小而无法表示，返回0
 *   如果结果太大而无法表示，返回+inf
 *   允许的操作符: 任何整数/无符号运算 包括 ||, &&. also if, while
 *   最多操作符数目: 30
 *   分值: 4
 */

unsigned floatPower2(int x) {
    unsigned result;
    if (x < -126) {
        return 0;
    }
    if (x > 127) {
        result = 0x7F800000;
        return result;
    }

    if (x >= 0) {
        result = (x + 127) << 23;
    } else {
        result = (x + 126) << 23;
    }
    return result;
}
