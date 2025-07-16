#include <iostream>
#include <cstring>
using namespace std;

class INTEGER
{
    int num;
public:
    INTEGER(int n) : num(n)       //构造函数，参数整型
    {
        cout << "con " << num << " " << endl;
    }
    INTEGER(const char *str) : num(strlen(str))    //构造函数，参数字符串
    {
        cout << "con by str " << str << " " << num << endl;
    }
    void mem_fun(INTEGER anint)
    {
        num = anint.num;
        cout << "mem_fun " << num << endl;
    }
};

int main()
{
    INTEGER obj1 = INTEGER(1);   
                       //调用带整型参数的构造函数
    INTEGER obj2 = "ChengDu";   
                       //调用带字符串参数的构造函数
    int anint = 10;
    INTEGER obj3 = INTEGER(anint);   
                       //调用带整型参数的构造函数
    obj1 = 20;       //obj1=(INTEGER(20));  
           //整型到类类型转换，调用带整型参数的构造函数
    obj2.mem_fun(3); //obj2.mem_fun(INTEGER(3)); 
           //整型到类类型转换，调用带整型参数的构造函数

    system("pause");
    return 0;
}