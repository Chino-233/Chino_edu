#include <iostream>
#include <cstring>
#include <algorithm>
using namespace std;

class CPerson   //类界面
{
private:
    char name[20];   //20
    char sex;        //1
    size_t  age;     //4

public:
    CPerson()   // = default;
    {
        cout << this << " cons without para" << endl;
    }
    CPerson(char *name, char sex,int age );
    CPerson(const char *name);
    void set(char *name, char sex,int age);
    int getage() const;
    const char *getname() const;
    char getsex() const;
    void print() const;
    ~CPerson()
    {
        cout << this << " des" << endl;
    }
};

//类实现
CPerson::CPerson(const char *name)
{
    cout << this << " cons with one para" << endl;
    strcpy(this->name,name);
}

    
CPerson::CPerson(char *name, char sexval,int ageval):sex(sexval),age(ageval)
{
    cout << this << " cons with para" << endl;
    strcpy(this->name,name);
}

void CPerson::set(char *name,char sex,int age)
{
    strcpy(this->name,name);
    this->sex = sex;
    this->age = age;
}

int CPerson::getage() const
{
    return age;
}
const char *CPerson::getname() const
{
    return name;
}

char CPerson::getsex() const
{
    return sex;
}
void CPerson::print() const
{
    cout << getname() << " " << sex << " " << age << endl;
}


int main()
{
    CPerson p1, p2;   //no para , call default cons
    
    char name[20],sex;
    int age;

    p1.print();
    p2.print();
    
    cin >> name >> sex >> age;
    //CPerson *p3 = new CPerson(name,sex,age);  
    CPerson p3 = {name,sex,age};
    p3.print();

    cin >> name >> sex >> age;
    //CPerson *p4 = new CPerson(name);  //para
    CPerson p4 = "wangwu";
    p4.print();

    //delete p3;
    //delete p4;
    system("pause");
}


/*
zhao F 18
li M 20

*/