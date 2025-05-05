#include <bits/stdc++.h>
using namespace std;
// const int N = 2e6 + 10;
const int N = 11;
#define ll long long

int main(){
    #ifndef ONLINE_JUDGE
    freopen("in.in", "r", stdin);
    freopen("out.out", "w", stdout);
    #endif
    int n,k;
    cin>>n>>k;
    int temp=1;
    for(int i=1;i<n;i++){
        int temp2=0;
        cin>>temp2;
        temp+=temp2;
    }
    cout<<max(1,k-(n*k-temp));
    return 1;
}