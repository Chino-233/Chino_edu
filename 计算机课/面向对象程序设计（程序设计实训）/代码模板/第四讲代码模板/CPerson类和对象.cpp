#include <iostream>
#include <cstring>
#include <algorithm>
using namespace std;

int age = 20;   //全局变量

class CPerson   //类界面
{
private:
    char name[20] = "liming";   //20
    char sex = 'M';        //1
    int  age = 18;        //4

public:
    CPerson(char *nameval,char sexval,int ageval)
    {
       strcpy(this->name,name);
       this->sex = sex;
       this->age = age;
    }
    void set(char *nameval,char sexval,int ageval);
    int getage() ;
    const char *getname() const;
    char getsex();
    void print() const;
};

//类实现
void CPerson::set(char *name,char sex,int age)
{
    strcpy(this->name,name);
    this->sex = sex;
    this->age = age;
}

int CPerson::getage()
{
    return age;
}
const char *CPerson::getname() const
{
    return name;
}

char CPerson::getsex()
{
    return sex;
}
void CPerson::print() const
{
    //printf("%d\n",this);  输出当前目标对象地址
    //printf("%d %d %d\n",&name,&sex,&age);   输出姓名地址
    //cout << (void *)&(CPerson::set) << " "<< (void *)&(CPerson::print) << endl;   输出成员函数地址
    cout << getname() << " " << sex << " " << ::age << endl;
}


int main()
{
    CPerson p1,p2;
    char name[20],sex;
    int age;

    p1.print();
    p2.print();
    printf("%d %d\n",&p1,&p2);
    cin >> name >> sex >> age;
    p1.set(name,sex,age);
    p1.print();

    cin >> name >> sex >> age;
    p2.set(name,sex,age);
    p2.print();


}


/*
zhao F 18
li M 20

*/
