#include <iostream>
#include <string>
using namespace std;

class  student
{
    string name;
public:
    student()   //= default;
    {
        cout << this << " student cons: " << name << endl;
    }
    explicit student(string nameval) :name(nameval)          //显示的，remove explicit
    {
        cout << this << " student cons: " << name << endl;
    }
    ~student()
    {
        cout << this << " student des " << endl;
    }
    student& operator=(const student& s)
    {
        name = s.name;
        cout << this << " student = " << name << endl;
        return *this;
    }

};


void fn(const student& s)
{

}

int main()
{
    cout << "------------ref1----------------" << endl;
    const student& ref1 = student{ "lisi" };   
    cout << &ref1 << endl;

    cout << "------------ref2----------------" << endl << endl;
    student&& ref2 = student("wangwu");  //右值引用
    cout << &ref2 << endl;

    cout << "--------------s1----------------" << endl << endl;
    student s1 = "zhaoliu"s;              //remove explicit, ok ; add explicit, error
    cout << &s1 << endl;

    cout << "--------------s2----------------" << endl << endl;
    student s2 = student("liwei");
    cout << &s2 << endl;

    cout << "--------------s3----------------" << endl << endl;
    student s3;
    s3 = "zhouming"s;
    cout << &s3 << endl;

    cout << "--------------fn----------------" << endl << endl;
    fn(student("wangming"));
    fn("tom"s);

    system("pause");
    return 0;
}
