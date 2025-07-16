#include <iostream>
using namespace std;

//增量计数器类
class Increase
{
public:
    Increase(int x) : value(x) {} //构造函数
    Increase &operator++();       //前增量 ， ++*this
    Increase operator++(int);     //后增量,   *this++
    void display();               //输出
private:
    int value;
};

//前加加，++*this, C语言中int a = 10, 支持++(++a), 即返回的是空间，对象引用
Increase &Increase::operator++()
{
    value++;      //先增量
    return *this; //再返回原对象
}

//后加加，*this++, C语言中int a = 10, 不支持(a++)++，即返回的是a的值，a再加1
Increase Increase::operator++(int)
{
    Increase temp(*this);   //返回原来的值，先做原对象备份
    value++;                //返回前加1
    return temp;            //返回备份值，即加1前的值
}


//输出函数
void Increase::display()
{
    cout << "the value is " << value << endl;
}

//主函数
int main()
{
    //测试C语言中前++，后++用法，理解重载自增运算符的返回值
    int a = 10;

    ++(++a);
    cout << a << endl;    //输出12
    //(a++)++;   //编译错误
    cout << a << endl;

    Increase  count1(10);

    (++(++count1)).display();   //连续加两次，输出12
    (count1++).display();       //先输出，再加。输出12，值变为13
    count1.display();           //输出13

    return 0;

}