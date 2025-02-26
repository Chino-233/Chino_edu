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
ll mi[20];
ll dp[20];
ll arr1[20];
ll arr2[20];
void solve(ll num,ll *arr){
    ll posnum[20]={0};
    ll x=num;
    ll len=0;
    while (x)
    {
        posnum[++len]=x%10;
        x/=10;
        //len++;
    }
    x=num;
    for(int i=len;i>=1;i--){
        for(int j=0;j<10;j++){
            arr[j]+=dp[i-1]*posnum[i];
        }
        for(int j=0;j<posnum[i];j++){
            arr[j]+=mi[i-1];
        }
        x-=pow(10,i-1)*posnum[i];
        arr[posnum[i]]+=(x+1);
        arr[0]-=mi[i-1];
    }
    
}
void Chino(){
    int a,b;
    cin>>a>>b;
    dp[0]=0;
    mi[0]=1ll;
    for(int i=1;i<=13;i++){
        dp[i]=dp[i-1]*10+mi[i-1];
        mi[i]=10ll*mi[i-1];
    }
    solve(a-1,arr1);
    solve(b,arr2);
    for(int i=0;i<10;i++){
        cout<<arr2[i]-arr1[i]<<" ";
    }
}
int main(){
    #ifndef ONLINE_JUDGE
	freopen("in.in", "r", stdin);
	freopen("out.out", "w", stdout);
    #endif
    Chino();
    return 0;
}