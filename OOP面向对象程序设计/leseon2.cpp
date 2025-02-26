#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <cstring>
#include <algorithm>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <iomanip>
#include <cmath>
#include <map>
using namespace std;
const int N = 1e5 + 10;
typedef double db;
#define int long long
class Shape{
private:
    string type;
public:
    Shape(){};
    Shape(string tval):type(tval){};
    string getType(){
        return type; 
    }
    virtual double getArea()=0;
};
class Circle:public Shape{
private:
    double r;
public:
    Circle(){};
    Circle(double rval,string tval):r(rval),Shape(tval){};
    double getArea(){
        return 3.14*r*r;
    }

};
class Square:public Shape{
private:
    double a;
public:
    Square(){};
    Square(double rval,string tval):a(rval),Shape(tval){};
    double getArea(){
        return a*a;
    }
    
};
class Pillar{
private:
    Shape *bottom;
    double heigth;
    static int count;
public:
    Pillar(){
        count++;
    };
    Pillar(Shape *bval,double hval):bottom(bval),heigth(hval){
        count++;
    };
    double getVolume(){
        return bottom->getArea()*heigth;
    }
    void set(int type,double a,double h){
        if(type==1){
            bottom=new Square(a,"Square");
        }
        else{
            bottom=new Circle(a,"Circle");
        }
    }
};
int Pillar::count=0;
signed main()
{
    #ifndef ONLINE_JUDGE
    freopen("in.in","r",stdin);
    freopen("out.out","w",stdout);
    #endif
    int a;
    cin>>a;
    cout<<a<<endl;
    
}