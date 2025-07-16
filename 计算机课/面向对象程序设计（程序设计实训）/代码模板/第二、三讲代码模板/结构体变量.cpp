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
    student s;  //{"20230001","lisi",{2005,1,1},90,100 };
    student *ps = &s;

    /*
    cin >> s.no  >> s.name
        >> s.birthday.year >> s.birthday.month >>s.birthday.day
        >> s.math >> s.english;
    */

    cin >> (*ps).no  >> (*ps).name
        >> (*ps).birthday.year >> (*ps).birthday.month >>(*ps).birthday.day
        >> (*ps).math >> (*ps).english;

    cout << ps->no << " " << ps->name << " "
        << ps->birthday.year << "/" << ps->birthday.month << ps->birthday.day
        << " " << ps->math << " " << ps->english << " " << (ps->math + ps->english) / 2 << endl;

}
/*
20230001 wangming 2005 1 1 90 100
*/

