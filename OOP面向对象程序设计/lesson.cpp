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
#include<bitset>
//#define int long long
using namespace std;
const int N = 1e5 + 10;
typedef double db;
template <class T>
class Judgecode{
private:
    T* data;
public:
    Judgecode(){
        data=new T[6];
        for(int i=0;i<6;i++){
            cin>>data[i];
        }
    }
    int judge(){
        for(int i=0;i<5;i++){
            if(data[i]>data[i+1]){
                cout<<"Invalid"<<"\n";
                return 1;
            }
        }
        cout<<"Valid"<<"\n";
        return 0;
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
        if(type=="c"){
            Judgecode<char> a;
            a.judge();
        }
        else if(type=="i"){
            Judgecode<int> a;
            a.judge();
        }
        else if(type=="f"){
            Judgecode<float> a;
            a.judge();
        }
    }
    
}
