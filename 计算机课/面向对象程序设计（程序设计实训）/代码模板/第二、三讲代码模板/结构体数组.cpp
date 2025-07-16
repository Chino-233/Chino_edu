#include <iostream>
using namespace std;

struct sdate
{
    int year, month, day;
};

struct student
{
    char  no[20];
    char  name[20];
    sdate birthday;
    int   math;
    int   english;
};

int main()
{
    int n;
    cin >> n;

    //student c[n];  //{ {"20230001","lisi",{2005,1,1},90,100 } , {"20230002","zhaowu",{2005,9,20},90,95 }, {"20230003","wangming",{2005,10,1},80,100 }};
    int *c = new int(n);

    for(int i=0; i<n; i++)
        cin >> c[i].no  >> c[i].name
        >> c[i].birthday.year >> c[i].birthday.month >>c[i].birthday.day
        >> c[i].math >> c[i].english;


    for(auto x: c)
    {
        cout << x.no << " " << x.name << " "
        << x.birthday.year << "/" << x.birthday.month << x.birthday.day
        << " " << x.math << " " << x.english << " " << (x.math + x.english) / 2 << endl;
    }

    /*
    for(int i=0; i<3; i++)   //c[i]
    {
        cout << c[i].no << " " << c[i].name << " "
        << c[i].birthday.year << "/" << c[i].birthday.month << c[i].birthday.day
        << " " << c[i].math << " " << c[i].english << " " << (c[i].math + c[i].english) / 2 << endl;
    }*/
}
/*
3
20230001 wangming 2005 1 1 90 100
20230002 lisi 2005 3 31 90 80
20230003 zhaowu 2005 9 20 90 95
*/

