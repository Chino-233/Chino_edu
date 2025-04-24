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
    int t;
    cin>>t;
    while (t--)
    {
        int n;
        cin>>n;
        int x;
        cin>>x;
        for(int i=0;i<n;i++){
            if(i==n-1&&x<n){
                cout<<x;
                break;
            }
            if(i!=x){
                cout<<i<<" ";
            }
            else{
                cout<<n-1<<" ";
            }
        }
        cout<<endl;
    }
    
    return 0;
}