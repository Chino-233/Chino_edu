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
    for(int i=0;i<n;i++){
        for(int j=i;j<n;j++){
            edge[i][j]=0;
            edge[j][i]=0;
        }
    }
    for(int i=0;i<n;i++){
        for(int j=i;j<n;j++){
            cin>>edge[i][j];
            edge[j][i]=edge[i][j];
        }
    }
    int t=20;
    while (t--)
    {
       for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(edge[i][j]==1){
                num[i]=max(num[j]+arr[j],num[i]);
            }
        }
       } 
    }
    int maxnum=0;
    for(int i=0;i<n;i++){
        if(num[i]>maxnum){
            maxnum=num[i];
        }
    }
    cout<<maxnum<<endl;
    return 0;
}