#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstring>
#include <algorithm>
using namespace std;

template <class T>
T getmax(T **mat,int n, int m)
{
    T maxvalue = mat[0][0];
    for(int i=0; i<n; i++)
        for(int j=0; j<m; j++)
            maxvalue = maxvalue > mat[i][j] ? maxvalue : mat[i][j];
    return maxvalue;
}

class CDate
{
    int y,m,d;
public:
    void set(int yval, int mval, int dval);
    bool operator > (const CDate &rhs)
    {
        return y*10000+m*100+d > rhs.y*10000 + rhs.m*100+d;
    }
    friend ostream &operator<<(ostream &out, const CDate &rhs)
    {
        return out;
    }


    
};


int main()
{
#ifndef ONLINE_JUDGE
    freopen("in.txt", "r", stdin);
    freopen("out.txt", "w", stdout);
#endif
    int t, y, m, d, n, month;
    cin >> t;
    while (t--)
    {
        char tp;
        cin >> tp >> n >> m;
        if (tp == 'D')
        {
            CDate **mat;
            mat = new CDate*[n];
            for(int i=0; i<n;i++)
            {
                mat[i] = new CDate[m];
                for(int j=0; j<m; j++)
                {
                    cin >> y >> month >> d;
                    mat[i][j].set(y,m,d);
                }
            }
            cout << getmax(mat,n,m) << endl;
        }
        else if (tp == 'I')
        {

        }
    }
    system("pause");
    return 0;
}