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
    int arr[N]={0};
    int n=0;int q=0;
    cin>>n>>q;
    int temp=0;
    int sum=0;
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            cin>>temp;
            if(i==j){
                arr[i]=temp;
                sum+=temp;
            }
        }
    }
    sum%=2;
    while (q--)
    {
        int temp1=0;
        cin>>temp1;
        if(temp1==1||temp1==2){
            cin>>temp1;
            sum=(sum+1)%2;
        }
        else{
            cout<<sum;
        }
    }
    
    return 0;
}