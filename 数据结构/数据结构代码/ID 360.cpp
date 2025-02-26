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
#define ll long long
class SqStack{
private:
    int* top;
    int* base;
public:
    bool IntiStack(SqStack &s){
        s.base = new int[1000];
        if(!s.base) return false;
        s.top = s.base;
        return true;
    }
    bool Push(SqStack &s,int e){
        //if(s.top==s.base) return false;
        *s.top++ = e;
        return true;
    }
    bool Pop(SqStack &s,int &e){
        if(s.top==s.base) return false;
        e = *--s.top;
        return true;
    }
    int GetTop(SqStack &s){
        if(s.top==s.base) return false;
        return *(s.top-1);
    }
};
int main(){
    #ifndef ONLINE_JUDGE
	freopen("in.in", "r", stdin);
	freopen("out.out", "w", stdout);
    #endif
    int n;
    cin>>n;
    SqStack s;
    s.IntiStack(s);
    while (n--)
    {
        string type;
        cin>>type;
        if(type=="push"){
            int m;
            cin>>m;
            s.Push(s,m);
            cout<<s.GetTop(s)<<endl;
        }
        else if(type=="pop"){
            int m;
            if(s.Pop(s,m)){
                if(s.GetTop(s)){
                    cout<<s.GetTop(s)<<endl;
                }
                else{
                    cout<<"empty"<<endl;
                }
            }
            else{
                cout<<"error"<<endl;
            }
        }

    }
    
    return 0;
}