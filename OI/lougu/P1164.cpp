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
    int arr[n+10]={0};
    for(int i=1;i<=n;i++){
        cin>>arr[i];
    }
    ll dp[n+10][m+10];
    for(int i=0;i<=n+9;i++){
        for(int j=0;j<=m+9;j++){
           dp[i][j]=0;
        }
    }   
    //dp[i][j]=dp[i-1][j-w]+arr[i],dp[i-1][j];
    //dp[i][arr[i]]+=1;dp[i][j]=dp[i-1][j]+dp[i-1][j-w];
    for(int i=1;i<=n;i++){
        if(arr[i]<=m){
        dp[i][arr[i]]++;
        }
        for(int j=0;j<=m;j++){
            dp[i][j]+=dp[i-1][j];
            if(j>=arr[i]){
                dp[i][j]+=dp[i-1][j-arr[i]];
            }
        }
    }
    cout<<dp[n][m];
    return 0;
}