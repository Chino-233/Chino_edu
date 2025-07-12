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
class polygon{
protected:
	int number;
private:
	int* side_length;
public:
	polygon(int nval,int *sideval):number(nval){
		side_length=new int[nval];
		side_length=sideval;
	}
	polygon(int nval):number(nval){};
	virtual int perimeter(){
		int sum=0;
		for(int i=0;i<number;i++){
			sum+=side_length[i];
		}
		return sum;
	}
	void display(){
		cout<<number<<" "<<perimeter()<<endl;
	}
};
class rectangle:public polygon{
private:
	int height;
	int width;
public:
	rectangle(int hval,int wval):polygon(4),height(hval),width(wval){};
	int perimeter(){
		return (height+width)*2;
	}
};
class equal_polygon:public polygon{
private:
	int side_len;
public:
	equal_polygon(int nval,int lenval):polygon(nval),side_len(lenval){};
	int perimeter(){
		return side_len*number;
	}
};
// 补齐三个类
/********** Write your code here! **********/




/*******************************************/
int main()
{
	#ifndef ONLINE_JUDGE
    freopen("in.in", "r", stdin);
    freopen("out.out", "w", stdout);
    #endif
    int t;
    string type;
    int side_length[100], number, side_len, width, height;    // 各变量含义见类属性
    polygon *p;

    cin >> t;
    while (t--)
    {
        cin >> type;
        if (type == "0")    // 多边形
        {
            number = 0;     // 边数计数
            while (cin >> side_length[number++])   // 输入各边长
            {
                if (side_length[number - 1] == -1)
                    break;
            }
            number--;
            p = new polygon(number, side_length);   // 定义多边形对象
        }
        else if (type == "1")   // 矩形
        {
            cin >> height >> width;    //  高、长
            p = new rectangle(height, width);  // 定义矩形对象
        }
        else if (type == "2")   // 等边多边形
        {
            cin >> number >> side_len;    // 边数和边长
            p = new equal_polygon(number, side_len);   // 定义等边多边形对象
        }
        p->display();   // 输出边数和周长
        delete p;
    }
    return 0;
}

