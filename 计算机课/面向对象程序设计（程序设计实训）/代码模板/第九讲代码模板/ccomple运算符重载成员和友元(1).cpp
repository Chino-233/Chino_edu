#include <iostream>
using namespace std;


class ccomplex
{
    int real,imag;
public:
    ccomplex(int b):real(b),imag(b){}
    friend ccomplex operator-(int ,const ccomplex &);
    friend ccomplex operator-(const ccomplex &,const ccomplex &);
    ccomplex(int r, int i):real(r),imag(i){}
    // *this + rhs,  member function
    ccomplex add(const ccomplex &rhs)
    {
        return ccomplex(real+rhs.real, imag+rhs.imag);
    }
    // + , lhs + rhs, lhs = *this, member function
    ccomplex operator+(const ccomplex &rhs)
    {
        return ccomplex(real+rhs.real, imag+rhs.imag);
    }

    // this-d
    ccomplex operator-(int d)
    {
        return ccomplex(d-real, d-imag);
    }

    //*this +=  rhs , member function
    ccomplex &operator+=(const ccomplex &rhs)
    {
        real += rhs.real;
        imag += rhs.imag;
        return *this;
    }
    //member function
    ccomplex &operator+=(int b)
    {
        real += b;
        imag += b;
        return *this;
    }
    // overload >   ,*this > rhs, member function
    bool operator>(const ccomplex &rhs)
    {
        return real>rhs.real && imag > rhs.imag;
    }
    void display()
    {
        cout.unsetf(ios::showpos);
        if(!real && !imag)
        {
            cout  << "0" << endl;
            return;
        }
        if(real)
            cout << real;
        if(imag)
            cout << showpos << imag << "i" << endl;
    }
    
};

// lhs - rhs
ccomplex operator-(const ccomplex &lhs,const ccomplex &rhs)
{
    return ccomplex(lhs.real-rhs.real, lhs.imag-rhs.imag);
}

// a - rhs
ccomplex operator-(int b,const ccomplex &rhs)
{
    cout << "-" << endl;
    return ccomplex(b-rhs.real, b-rhs.imag);
}



int main()
{
#ifndef ONLINE_JUDGE
	freopen("in.txt", "r", stdin);
	freopen("out.txt", "w", stdout);
#endif
    int r1,i1,r2,i2;

    cin >> r1 >> i1 >> r2 >> i2;
    ccomplex c1(r1,i1), c2(r2,i2);

    (10 - c1).display();
    (c1-c2).display();
    (c1+c2).display();
    c1.operator+(c2).display();
    if(c1>c2)
        cout << "c1 > c2" << endl;
    else
        cout << "c1 <= c2" << endl;
        

    cout << "----" << endl;
    /*(c1 += c2) += c2;
    c1.display();*/
    (c1+=10)+=20;
    c1.display();

    int a = 10;
    (a+=10)+=20;
    cout <<  a << endl;
    return 0;
}
