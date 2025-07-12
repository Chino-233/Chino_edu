//补充头文件、点类和矩形类
/********** Write your code here! **********/
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
template<class T>
class Matrix{
private:
    T** arr;
    int n;
    int m;
public:
    Matrix(int nval,int mval):n(nval),m(mval){
        arr=new T*[n];
        for(int i=0;i<n;i++){
            arr[i]=new T[m];
        }
        for(int i=0;i<n;i++){
            for(int j=0;j<m;j++){
                cin>>arr[i][j];
            }
        }
    }
    Matrix(){};
    void print(){
        for(int i=0;i<n;i++){
            for(int j=0;j<m;j++){
                cout<<arr[i][j];
                if(j!=m-1){
                    cout<<" ";
                }
            }
            cout<<endl;
        }
    }
    void transport(){
        T** temp;
        temp=new T*[m];
        for(int i=0;i<m;i++){
            temp[i]=new T[m];
        }
        for(int i=0;i<n;i++){
            for(int j=0;j<m;j++){
                temp[j][i]=arr[i][j];
            }
        }
        arr=new T*[m];
        for(int i=0;i<m;i++){
            arr[i]=new T[m];
        }
        arr=temp;
        swap(n,m);
    }
};
int main()
{
    #ifndef ONLINE_JUDGE
    freopen("in.in", "r", stdin);
    freopen("out.out", "w", stdout);
    #endif
    int t;
    cin>>t;
    while (t--)
    {
        string type;
        cin>>type;
        if(type=="I"){
            int n,m;
            cin>>n>>m;
            Matrix<int>cs(n,m);
            cs.transport();
            cs.print();
        }
        else if(type=="string"){
            int n,m;
            cin>>n>>m;
            Matrix<int>cs(n,m);
            cs.transport();
            cs.print();
        }
        else if(type=="D"){
            int n,m;
            cin>>n>>m;
            Matrix<double> cs(n,m);
            cs.transport();
            cs.print();
        }
        else if(type=="C"){
            int n,m;
            cin>>n>>m;
            Matrix<char>cs(n,m);
            cs.transport();
            cs.print();
        }
        //cout<<endl;
    }
    
    return 0;
}