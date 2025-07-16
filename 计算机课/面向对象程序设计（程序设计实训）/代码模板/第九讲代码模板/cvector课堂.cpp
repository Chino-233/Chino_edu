#include <iostream>
using namespace std;

class cvector
{
    int n;
    int *data;
public:
    cvector(int val, int *d = nullptr):n(val)
    {
        data = new int[n];
        if(d)
            memcpy(data, d, sizeof(int)*n);
    }
    cvector(const cvector &rhs)
    {
        n = rhs.n;
        data = new int[n];
        memcpy(data, rhs.data, sizeof(int)*n);
    }
    friend istream &operator>>(istream &in, cvector &rhs)
    {
        for(int i=0; i<rhs.n; i++)
            in >> rhs.data[i];
        return in;
    }
    friend ostream &operator<<(ostream &out, const cvector &rhs)
    {
        out << rhs.n << " ";
        for(int i=0; i<rhs.n; i++)
            out << rhs.data[i] << " ";
        out << endl;
        return out;
    }
    // *this  + rhs
    cvector operator+(const cvector &rhs)
    {
        int *res = new int[n];
        for(int i=0; i<n; i++)
            res[i] = data[i] + rhs.data[i];
        return cvector(n, res);
    }
    // lhs - rhs
    friend cvector operator-(const cvector &lhs, const cvector &rhs);
    //*this > rhs
    bool operator>(const cvector &rhs)
    {
        int n1 = 0, n2 = 0;
        for(int i=0; i<n; i++)
        {
            n1 += data[i] > rhs.data[i];
            n2 += data[i] < rhs.data[i];
        }
        return n1 > n2;
    }
    // *this[i] , data[i]
    int &operator[](int i)
    {
        return data[i];
    }
    // int(*this)
    operator int()
    {
        int sum = 0;
        for(int i=0; i<n; i++)
            sum += data[i];
        return sum;
    }
    // *this = rhs;  //*this  = *this
    cvector &operator=(const cvector &rhs)
    {
        if(this == &rhs)
            return *this;
        if(data)
            delete []data;
        
        n = rhs.n;
        data = new int[n];
        memcpy(data, rhs.data, sizeof(int)*n);
        return *this;
    }
    int operator()(int start,int end)
    {
        int sum = 0;
        for(size_t i= start; i<=end; i++)
            sum += data[i];
        return sum;
    }
    ~cvector()
    {
        cout << this << " " << data << endl;
        delete []data;
    }
    // *this, rhs  ,return rhs
    cvector operator,(const cvector &rhs)
    {
        return rhs;
    }
};

cvector operator-(const cvector &lhs, const cvector &rhs)
{
    int *res = new int[lhs.n];
    for(int i=0; i<lhs.n; i++)
        res[i] = lhs.data[i] - rhs.data[i];
    return cvector(lhs.n, res);
}
/*
10) 重载，运算符，取第二个表达式值。
*/

int main()
{
    #ifndef ONLINE_JUDGE
        freopen("in.txt", "r", stdin);
        freopen("out.txt", "w", stdout);
    #endif
    int n;

    cin >> n;
    cvector v1(n), v2(n);
    cout << &v1 << " " << &v2 << endl;
    cin >> v1 >> v2;
    cout << v1+ v2 << endl;
    cout << v1 - v2 << endl;

    v1 = v2;  //v1.data = v2.data;  10,20,30,40,50
    cout << v1 << endl;

    cout << v1(1,3) << endl;  //10,20,30,40,50
    
    cvector v3 = v1+v2;  // 20,40,60,80,100
    cout << (v1,v2,v3) << endl;

    /*cout << v1[0]  << endl;
    for(int i=0; i<n; i++)
        cin >> v1[i];
    cout << v1 << endl;
    int a = v1;
    cout << a << endl;
    
    cout << int(v1) << endl;*/
    return 0;
}

