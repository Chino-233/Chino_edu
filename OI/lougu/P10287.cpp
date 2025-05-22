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
    int v,n;
    cin>>v>>n;
    int arr[N]={0};
    //dp[i][j]=dp[i-1][j-w]+arr[i],dp[i-1][j];
    for(int i=1;i<=n;i++){
        cin>>arr[i];
    }
    ll dp[n+10][v+10];
    for(int i=0;i<n+10;i++){
        for(int j=0;j<v+10;j++){
            dp[i][j]=0;
        }
    } 
    for(int i=1;i<=n;i++){
        for(int j=0;j<=v;j++){
            if(j<arr[i]){
                dp[i][j]=dp[i-1][j];
            }
            else{
                dp[i][j]=max(dp[i-1][j-arr[i]]+arr[i],dp[i-1][j]);
            }
        }
    } 
    cout<<v-dp[n][v]<<endl; 
    return 0;
}