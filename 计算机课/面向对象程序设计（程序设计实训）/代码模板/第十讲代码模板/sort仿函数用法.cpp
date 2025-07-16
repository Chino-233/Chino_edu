#include <iostream>
#include <algorithm>
using namespace std;
class CDate
{
    int year, month, day;
    friend bool cmp(const CDate &, const CDate &);
    friend class  datecmp;

public:
    CDate() = default;
    CDate(int y, int m, int d) : year(y), month(m), day(d) {}
    bool operator<(const CDate &rhs)
    {
        return year * 10000 + month * 100 + day < rhs.year * 10000 + rhs.month * 100 + rhs.day;
    }
    operator int() const
    {
        return year * 10000 + month * 100 + day;
    }
    friend istream &operator>>(istream &in, CDate &rhs)
    {
        in >> rhs.year >> rhs.month >> rhs.day;
        return in;
    }
    friend ostream &operator<<(ostream &out, const CDate &rhs)
    {
        out  << rhs.year << "/" << rhs.month << "/" << rhs.day;
        return out;
    }
};

struct datecmp
{
    bool operator()(const CDate &lhs, const CDate &rhs)
    {
        return int(lhs) > int(rhs);
    }
};

bool cmp(const CDate &lhs, const CDate &rhs)
{
    return lhs.year * 10000 + lhs.month * 100 + lhs.day < rhs.year * 10000 + rhs.month * 100 + rhs.day;
}
void output(CDate *d, int n)
{
    for(size_t i=0; i<n; i++)
        cout << d[i] << endl;
}
/*******************************************/
int main()
{
    #ifndef ONLINE_JUDGE
        freopen("in.txt", "r", stdin);
        freopen("out.txt", "w", stdout);
    #endif

    int n;
    cin >> n;
    CDate d[100];
    
    for(size_t i=0; i<n; i++)
        cin >> d[i];
        
    sort(d,d+n);   //operator < ， 升序排序
    output(d,n);
    cout << "---------------" << endl;
    sort(d,d+n, cmp);  //cmp函数， 升序排序
    output(d,n);
    cout << "---------------"  << endl;
    sort(d,d+n, datecmp());  //仿函数，函数对象, 降序排序
    output(d, n);
}


// 4
// 2024 5 1
// 2023 12 31
// 2000 1 1
// 2022 3 4