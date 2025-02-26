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
    T* arr;
    int n;
    int m;
public:
    Matrix(int nval):n(nval){
        arr=new T[n];
        for(int i=0;i<n;i++){
            cin>>arr[i];
        }
    }
    Matrix(){};
    void print(){
        for(int i=0;i<n;i++){
            cout<<arr[i];
            if(i!=n-1){
                cout<<" ";
            } 
        }
        cout<<endl;
    }
    void classsort(){
        next_permutation(arr,arr+n);
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
            cin>>n;
            Matrix<int>cs(n);
            cs.classsort();
            cs.print();
        }
        else if(type=="S"){
            int n,m;
            cin>>n;
            Matrix<string>cs(n);
            cs.classsort();
            cs.print();
        }
        else if(type=="D"){
            int n,m;
            cin>>n;
            Matrix<double> cs(n);
            cs.classsort();
            cs.print();
        }
        else if(type=="C"){
            int n,m;
            cin>>n;
            Matrix<char>cs(n);
            cs.classsort();
            cs.print();
        }
        //cout<<endl;
    }
    
    return 0;
}