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
    int n,k;
    cin>>n>>k;
    int temp=0;
    for(int i=0;i<n;i++){
        int temp1=0;
        cin>>temp1;
        temp+=temp1;
    }
    cout<<max(0,k-(n*k-temp));
    return 0;
}