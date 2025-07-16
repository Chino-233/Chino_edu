#include <iostream>
#include <cstring>
using namespace std;

class X
{
public : 
    X(int e) : val(e) { cout << "X cons " << val << endl; }
private:
    int val;
};
class Y
{
public: 
    Y(X &r) : xobj(r) { cout << "Y cons" << endl;}
private:
    X  xobj;
};

int main()
{
    X objX = 1;   //ok, X有传整型参数构造函数
    Y objY = objX;   //ok, Y有传X对象构造函数
    //Y a = 1; //错误，Y无传整型参数构造函数
    
    system("pause");
    return 0;
}
