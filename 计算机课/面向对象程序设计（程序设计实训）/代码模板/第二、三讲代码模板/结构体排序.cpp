#include <iostream>
#include <algorithm>
using namespace std;

struct sdate
{
    int year, month, day;
};

struct student
{
    int id;
    char  no[20];
    char  name[20];
    sdate birthday;
    int   math;
    int   english;
};

/*
void input(student &s)
{
    cin >> s.no  >> s.name
        >> s.birthday.year >> s.birthday.month >>s.birthday.day
        >> s.math >> s.english;
}
*/

student &input()
{
    static student s;
    cin >> s.no  >> s.name
        >> s.birthday.year >> s.birthday.month >>s.birthday.day
        >> s.math >> s.english;
    return s;
}

void output(const student &x)
{
    cout << x.no << " " << x.name << " "
        << x.birthday.year << "/" << x.birthday.month << "/" << x.birthday.day
        << " " << x.math << " " << x.english << " " << (x.math + x.english) / 2 << endl;
}

void oursort(student *c,int n)
{
    for(int i=1; i<n; i++)
        for(int j=0; j<n-i; j++)  //i=1, n-1
        {
            if(c[j].math+c[j].english > c[j+1].math + c[j+1].english)
            {
                /*
                student temp;
                temp = c[j];
                c[j] = c[j+1];
                c[j+1] = temp;
                */
                swap(c[j],c[j+1]);
            }
        }
}

// lhs  rhs
bool cmp(const student &lhs, const student &rhs)
{
    //return lhs.math+lhs.english <= rhs.math+rhs.english;
    if(lhs.birthday.year != rhs.birthday.year)
        return lhs.birthday.year > rhs.birthday.year;
    if(lhs.birthday.month != rhs.birthday.month)
        return lhs.birthday.month > rhs.birthday.month;
    if(lhs.birthday.day != rhs.birthday.day)
        return lhs.birthday.day > rhs.birthday.day;
    return lhs.id < rhs.id;
}

int main()
{
    int n;
    cin >> n;

    student c[n];  //{ {"20230001","lisi",{2005,1,1},90,100 } , {"20230002","zhaowu",{2005,9,20},90,95 }, {"20230003","wangming",{2005,10,1},80,100 }};

    for(int i=0; i<n; i++)
    {
        //input(c[i]);
        c[i] = input();
        c[i].id = i;
    }


    //oursort(c,n);
    sort(c,c+n,cmp);
    for(auto x: c)
    {
        output(x);
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
20230002 lisi 2005 1 1 90 80
20230003 zhaowu 2005 9 20 90 95
*/

