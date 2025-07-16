#include <iostream>
#include <fstream>
using namespace std;

class Increase
{
    int val;

public:
    Increase() = default;
    //overload >>
    friend istream &operator>>(istream &in, Increase & rhs);
    // overload << 
    friend ostream& operator<<(ostream &out, const Increase &rhs);
    // a--
    friend Increase operator--(Increase &lhs, int);
    //--a
    friend Increase &operator--(Increase &lhs);

    Increase(int v) : val(v) {}
    // a++ , *this++
    Increase operator++(int)
    {
        Increase old(*this);  //10
        val++;  //11
        return old;   //10
    }
    void display()
    {
        cout << val << endl;
    }
    //++a , ++*this
    Increase &operator++()
    {
        val++;
        return *this;
    }
};

//  cout << cnt
ostream &operator<<(ostream & out, const Increase & rhs)
{
    out << rhs.val << endl;
    return out;
}

// cin >> cnt
istream &operator>>(istream &in, Increase & rhs)
{
    in >> rhs.val;
    return in;
}

// a--, 后减减
Increase operator--(Increase &lhs, int)
{
    Increase old(lhs);
    lhs.val--;
    return old;
}
//--a, 前减减  --(--a)
Increase &operator--(Increase &lhs)
{
    lhs.val--;
    return lhs;
}

int main()
{
/*#ifndef ONLINE_JUDGE
    freopen("in.txt", "r", stdin);
    freopen("out.txt", "w", stdout);
#endif*/
    /*int val;

    cin >> val;
    Increase cnt(val);*/
    ofstream oFile("out.txt", ios::out);
    
    Increase cnt,cnt1;

    cin >> cnt >> cnt1;

    (cnt++).display();
    cnt.display();
    (++(++cnt)).display();

    int a = 10;
    ++(++a);  //11,12
    oFile << a << endl;

    /*(cnt--).display();   // 10,  9
    (--(--cnt)).display();  //8, 7*/
    cout << cnt++ << endl;  //10,  11
    cout << --(--cnt);   //10, 9 

    cout <<  cnt++ << endl;  //10,  11
    cout << --(--cnt);   //10, 9 
    system("pause");

    return 0;
}
