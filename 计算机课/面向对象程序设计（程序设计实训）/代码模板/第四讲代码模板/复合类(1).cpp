//澶嶅悎绫?
#include <iostream>
using namespace std;


//定义点类CPoint。属性：x,y坐标。构造函数及print成员函
//数。
class CPoint
{
    int x, y;
public:
    CPoint()
    {
        cout << this << "CPoint cons default" << endl;
    }
    CPoint(int xval, int yval) :x(xval), y(yval)
    {
        cout << this << "CPoint cons with para" << endl;
    }
    int getx()
    {
        return x;
    }
    int gety()
    {
        return y;
    }
    void print()
    {
        cout << x << " " << y << endl;
    }
    ~CPoint()
    {
        cout << this << "CPoint des" << endl;
    }
};

/*定义圆类CCircle，属性：点对象圆心，半径。构造函数
及print成员函数。*/
class CCircle
{
private:
    CPoint center;
    int r;
public:
    CCircle():center(),r(10)
    {
         cout << this << "CCircle cons default" << endl;
    }
    CCircle(int x,int y,int rval):center(x,y),r(rval)
    {
         cout << this << "CCircle cons with para" << endl;
    }
    ~CCircle()
    {
        cout << this << "CCircle des" << endl;
    }
    void print()
    {
        cout << "(" << center.getx() <<","<< center.gety() << ") " << r << endl;
    }
};

/*定义圆柱体类CCylinder，属性：2个圆，半径，高。构造
函数及print成员函数。*/
class CCylinder
{
    CCircle upcircle,downcircle;
    int r, high;
public:
    void print()
    {
        upcircle.print();
        downcircle.print();
        cout << high << endl;
    }
    CCylinder()
    {
         cout << this << "CCylinder cons without para" << endl;
    }
    CCylinder(int x1,int y1,int x2,int y2,int r,int h):downcircle(x1,y1,r),upcircle(x2,y2,r),high(h)
    {
              cout << "UP:" << &upcircle << endl;
        cout << "DOWN:" << &downcircle << endl;
        cout << this << "CCylinder cons with para" << endl;
    }
    ~CCylinder()
    {
         cout << this << "CCylinder des" << endl;
    }
};


int main()
{
    CCircle circle; //(10,10,20);

    circle.print();

    CCylinder  c(10,20,30,20,40,50);
    
    c.print();

    system("pause");
}

