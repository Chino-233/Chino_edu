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
    int r;
    cin>>r;
    int arr[1110][1110];
    for(int i=0;i<1110;i++){
        for(int j=0;j<1110;j++){
            arr[i][j]=0;
        }
    }
    for(int i=1;i<=r;i++){
       for(int j=1;j<=i;j++){
        cin>>arr[i][j];
       } 
    }
    int dp[1110][1110];
    for(int i=0;i<1110;i++){
        for(int j=0;j<1110;j++){
            dp[i][j]=0;
        }
    }
    for(int i=1;i<=r;i++){
        for(int j=1;j<=i;j++){
            dp[i][j]=max(dp[i-1][j],dp[i-1][j-1])+arr[i][j];
        }
    }
    int nummax=-1;
    for(int i=1;i<=r;i++){
        if(nummax<dp[r][i]){
           nummax=dp[r][i]; 
        }
    }
    cout<<nummax;
    return 0;
}