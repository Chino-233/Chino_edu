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
    int data;
    SqStack* next;
public:
    bool IntiStack(SqStack &s){
        s.next = NULL;
        return true;
    }
    bool Push(SqStack &s,int e){
        SqStack* p = new SqStack;
        p->data = e;
        p->next = s.next;
        s.next = p;
        return true;
    }
    bool Pop(SqStack &s,int &e){
        if(s.next==NULL) return false;
        SqStack* p = s.next;
        e = p->data;
        s.next = p->next;
        delete p;
        return true;
    }
    int GetTop(SqStack &s){
        if(s.next==NULL) return false;
        return s.next->data;
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
        int m;
        cin>>m;
        s.Push(s,m);
        if(s.GetTop(s)){
            cout<<s.GetTop(s)<<endl;
        }
        else{
            cout<<"error"<<endl;
        }
    }
    
    return 0;
}