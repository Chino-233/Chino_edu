#include <iostream>
#include <cstring>
using namespace std;

// 函数模板， 可以带默认参数
template <class T = double>
T max1(T a, T b)
{
    cout << "template T" << endl;
    return (a > b) ? a : b;
}

// 重载的非模板函数
int max1(int a, int b)
{
    cout << "int" << endl;
    return (a > b) ? a : b;
}

// 函数模板的特化
template <>
char *max1<char *>(char *a, char *b)
{
    cout << "template char *" << endl;
    return strcmp(a, b) > 0 ? a : b;
}

void fun(int num, char cha)
{
    cout << max1(num, num) << endl; // 调用 max(int,  int)
    cout << max1(cha, cha) << endl; // 调用 max(T,  T)
    cout << max1(num, cha) << endl; // 调用 max(int,  int)
    cout << max1(cha, num) << endl; // 调用 max(int,  int)
}

int main()
{
    fun(10, 'a');
    char s1[] = "abc",s2[] = "good";
    cout << max1(s1, s2) << endl;
    system("pause");
    return 0;
}