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
    int n;
    cin>>n;
    int arr[50]={0};
    int num[50]={0};
    for(int i=0;i<n;i++){
        cin>>arr[i];
        num[i]=arr[i];
    }
    int edge[50][50];
    for(int i=0;i<50;i++){
        for(int j=0;j<50;j++){
            edge[i][j]=0;
            edge[j][i]=0;
        }
    }
    for(int i=0;i<n;i++){
        for(int j=i+1;j<n;j++){
            cin>>edge[i][j];
            edge[j][i]=edge[i][j];
        }
    }
    int t=20;
    int maxnum=0;
    int dp[50][50];
    for(int i=0;i<50;i++){
        for(int j=0;j<50;j++){
            dp[i][j]=0;
            if(j==1){
                dp[i][j]=-1;
            }
        }
    }
    for(int i=0;i<n;i++){
        dp[i][0]=arr[i];
        for(int j=0;j<i;j++){
            if(edge[i][j]==1&&dp[i][0]<dp[j][0]+arr[i]){
                dp[i][0]=dp[j][0]+arr[i];
                dp[i][1]=j;
            }
        }

    }
    int maxpos=0;
    for(int i=0;i<n;i++){
        if(maxnum<dp[i][0]){
            maxpos=i;
            maxnum=dp[i][0];
        }
    }
    int pos=maxpos;
    int route[50]={0};
    route[0]=maxpos;
    int k=1;
    while(dp[pos][1]!=-1){
        pos=dp[pos][1];
        route[k]=pos;
        k++;
    }
    for(int i=k-1;i>=0;i--){
       cout<<route[i]+1<<" "; 
    }
    cout<<endl<<maxnum;
    return 0;
}