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
    int n,m;
    cin>>n>>m;
    int dp[n+10]={0};
    bool edge[n+10][n+10];
    for(int i=0;i<n+10;i++){
        for(int j=0;j<n+10;j++){
            edge[i][j]=0;
        }
    }
    for(int i=0;i<m;i++){
        int x=0,y=0;
        cin>>x>>y;
        edge[x][y]=1;
    }
    for(int j=1;j<=n;j++){
        int flag=0;
        for(int i=0;i<=n;i++){
            if(edge[i][j]==1){
                flag=1;
                dp[j]%=80112002;
                dp[i]%=80112002;
                dp[j]+=dp[i];
                dp[j]%=80112002;
            }
        }
        if(!flag){
            dp[j]=1;
        }
    }
    int maxnum=-1;
    for(int i=1;i<=n;i++){
        maxnum=max(maxnum,dp[i]);
    }
    cout<<maxnum<<endl;
    return 0;
}