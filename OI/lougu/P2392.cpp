#include <bits/stdc++.h>
using namespace std;

//const int N = 1e6 + 10;
const int N = 10;
#define ll long long
int homeworktime(int n){
    int v;
    v=0;
    int arr[N]={0};
    //dp[i][j]=dp[i-1][j-w]+arr[i],dp[i-1][j];
    for(int i=1;i<=n;i++){
        cin>>arr[i];
        v+=arr[i];
    }
    ll dp[n+10][v+10];
    for(int i=0;i<n+10;i++){
        for(int j=0;j<v+10;j++){
            dp[i][j]=0;
        }
    } 
    for(int i=1;i<=n;i++){
        for(int j=0;j<=v/2;j++){
            if(j<arr[i]){
                dp[i][j]=dp[i-1][j];
            }
            else{
                dp[i][j]=max(dp[i-1][j-arr[i]]+arr[i],dp[i-1][j]);
            }
        }
    }
    return v-dp[n][v/2];
}
int main(){
    #ifndef ONLINE_JUDGE
    freopen("in.in", "r", stdin);
    freopen("out.out", "w", stdout);
    #endif
    int t=4;
    int sum=0;
    int s1=0,s2=0,s3=0,s4=0;
    cin>>s1>>s2>>s3>>s4;
    sum+=homeworktime(s1);
    sum+=homeworktime(s2);
    sum+=homeworktime(s3);
    sum+=homeworktime(s4);
    cout<<sum<<endl;
    return 0;
}