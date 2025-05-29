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
    int t,m;
    cin>>t>>m;
    int arr[m+10]={0};
    int weigth[m+10]={0};
    int dp[m+10][t+10];
    for(int i=0;i<m+10;i++){
        for(int j=0;j<t+10;j++){
            dp[i][j]=0;
        }
    }
    //dp[i][j]=max(dp[i-1][j],dp[i-1][j-w]+arr[i]);
    for(int i=1;i<=m;i++){
        cin>>weigth[i];
        cin>>arr[i];
    }
    for(int i=1;i<=m;i++){
        for(int j=0;j<=t;j++){
            if(j>=weigth[i]){
                dp[i][j]=max(dp[i-1][j],dp[i-1][j-weigth[i]]+arr[i]);
            }
            else{
                dp[i][j]=dp[i-1][j];
            }
        }
    }
    cout<<dp[m][t];
    return 0;
}