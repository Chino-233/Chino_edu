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
class BoundArray{
private:
    T* arr;
    int n;
    int beg;
    int end;
public:
    BoundArray(int nval):n(nval){
        arr=new T[n];
        char a;
        cin>>a;
        cin>>beg>>a>>end>>a>>a;
        if(beg<0){
            beg=0;
        }
        if(end<0){
            end=0;
        }
        if(end>n){
            end=n;
        }
        if(beg>n){
            beg=n;
        }
        if(beg>end){
            swap(beg,end);
        }
        for(int i=0;i<n;i++){
            cin>>arr[i];
            //cout<<arr[i]<<"look"<<endl;
        }
    }
    BoundArray(){};
    /*void sort(){
        for(int i=0;i<n;i++){
            for(int j=0;j<n-1-i;j++){
                if(arr[j]>arr[j+1]){
                    swap(arr[j],arr[j+1]);
                }
            }
        }
    }*/
    void arrsort(){
        sort(arr+beg,arr+end);
    }
    int search(T a){
        for(int i=0;i<n;i++){
            if(arr[i]==a){
                return i+1;
            }
        }
        return 0;
    }
    void print(){
        cout<<"[";
        for(int i=0;i<n;i++){
            cout<<arr[i];
            if(i!=n-1){
            cout<<", ";
            }
        }
        cout<<"]"<<endl;
    }
    void floatprint(){
        cout<<"[";
        for(int i=0;i<n;i++){
            cout<<fixed<<setprecision(1)<<arr[i];
            if(i!=n-1){
            cout<<", ";
            }
        }
        cout<<"]"<<endl;
    }
    bool judge(){
        for(int i=0;i<n-1;i++){
            if(arr[i+1]<arr[i]){
                return 0;
            }
        }
        return 1;
    }
    int getn(){
        return n;
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
        if(type=="int"){
            int n;
            cin>>n;
            BoundArray<int> cs(n);
            cs.arrsort();
            cs.print();
        }
        else if(type=="string"){
            int n;
            cin>>n;
            BoundArray<string> cs(n);
            cs.arrsort();
            cs.print();
        }
        else if(type=="float"){
            int n;
            cin>>n;
            BoundArray<float> cs(n);
            cs.arrsort();
            cs.floatprint();
        }
        else if(type=="C"){
            int n;
            cin>>n;
            BoundArray<char> cs(n);
            cs.arrsort();
            cs.print();
        }
        //cout<<endl;
    }
    
    return 0;
}