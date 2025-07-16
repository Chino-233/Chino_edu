// coding: UTF-8 
#include<iostream>
using namespace std;

class CSample1
{
public:
    CSample1() = default;         //空构造函数
    void SetI(int t)
    {
        i = t;
    }
    int MemberFunc()       //成员函数
    {
        return i;
    }

    //友元函数直接访问CSample1类的私有数据成员i,
    //即一个类可以声明一个友元关系，一起来共享类中的所有成员
    //增加一个对象指针参数
    friend void FriendFunc(CSample1* cp, int a)
    {
        cp->i = a;    //对象指针参数为i指明当前所属对象
    }
private:
    int i;
    friend class  CSample2;   //友元类
};

class CSample2
{
public:
    CSample2() = default;

    void ShowCSample1()
    {
        CSample1 ss1;
        ss1.SetI(4);
        cout << "ss1.i=" << ss1.i << endl;
    }
    void ShowCSample1(CSample1 s1)
    {
        cout << "s1.i=" << s1.i << endl;
    }
private:
};

int main(void)
{
    CSample1 s1;
    s1.SetI(1);
    CSample1* p1 = new CSample1;         //创建一个对象指针并初始化

    //友元函数不需要通过对象或对象指针来调用，可以直接调用即可。
    FriendFunc(p1, 2);                 //p指针所指的对象的i被赋值为2
    cout << "i=" << p1->MemberFunc() << endl;
    delete p1;                         //释放指针p1

    CSample2 s2;
    s2.ShowCSample1();
    s2.ShowCSample1(s1);
    //cout << "s1.i=" << s1.i << endl;
    system("pause");
}