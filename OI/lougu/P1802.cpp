#include <bits/stdc++.h>
using namespace std;

// const int N = 1e6 + 10;
const int N = 10;
#define ll long long

int main(){
    #ifndef ONLINE_JUDGE
    freopen("in.in", "r", stdin);
    freopen("out.out", "w", stdout);
    #endif
    ll n,x;
    cin>>n>>x;
    ll lose[N]={0};
    ll win[N]={0};
    ll use[N]={0};
    ll eff[N]={0};
    ll loseall=0;
    for(ll i=0;i<n;i++){
        cin>>lose[i]>>win[i]>>use[i];
        eff[i]=win[i]-lose[i];
        loseall+=lose[i];
    }
    ll dp[n+10][x+10];
    for(ll i=0;i<=n+9;i++){
        for(ll j=0;j<=x+9;j++){
            dp[i][j]=0;
        }
    }
    for(ll i=1;i<=n;i++){
        for(ll j=0;j<=x;j++){
            if(j<use[i-1]){
                dp[i][j]=dp[i-1][j];
                continue;
            }
            dp[i][j]=max(dp[i-1][j-use[i-1]]+eff[i-1],dp[i-1][j]);
        }
    }
    cout<<5*(dp[n][x]+loseall)<<endl;
    return 0;
}