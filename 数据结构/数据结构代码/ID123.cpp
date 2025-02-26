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

int main(){
    #ifndef ONLINE_JUDGE
	freopen("in.in", "r", stdin);
	freopen("out.out", "w", stdout);
    #endif
    int n;
    cin>>n;
    queue<int> a,b,c;
    queue<string> typestone;
    for(int i=0;i<n;i++){
        string type;
        int time=0;
        cin>>type;
        typestone.push(type);
    }
    for(int i=0;i<n;i++){
        int time=0;
        cin>>time;
        if(typestone.front()=="A"){
            a.push(time);
        }
        else if(typestone.front()=="B"){
            b.push(time);
        }
        else{
            c.push(time);
        }
        typestone.pop();
    }
    int sum=0;
    int count=0;
    while (!a.empty())
    {
        sum+=a.front();
        count++;
        a.pop();
    }
    if(count==0){
        cout<<0<<endl;
    }
    else{
        cout<<sum/count<<endl;
    }
    sum=0;
    count=0;
    while (!b.empty())
    {
        sum+=b.front();
        count++;
        b.pop();
    }
    if(count==0){
        cout<<0<<endl;
    }
    else{
        cout<<sum/count<<endl;
    }
    sum=0;
    count=0;
    while (!c.empty())
    {
        sum+=c.front();
        count++;
        c.pop();
    }
    if(count==0){
        cout<<0<<endl;
    }
    else{
        cout<<sum/count<<endl;
    }
    return 0;
}