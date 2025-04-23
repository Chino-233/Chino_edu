#include <bits/stdc++.h>
using namespace std;

// const int N = 1e6 + 10;
const int N = 10;
#define ll long long
struct ablity{
    int t;
    int l;
};
bool cmp(ablity a,ablity b){
    return a.t+a.l<b.t+b.l;
}
int main(){
    #ifndef ONLINE_JUDGE
    freopen("in.in", "r", stdin);
    freopen("out.out", "w", stdout);
    #endif
    int n;
    cin>>n;
    ablity arr[N];
    for(int i=0;i<n;i++){
        cin>>arr[i].t;
        cin>>arr[i].l;
    }
    sort(arr,arr+n,cmp);
    int nowtime=0;
    int all=0;
    for(int i=0;i<n;i++){
        if(nowtime<=arr[i].t){
            nowtime=arr[i].t+arr[i].l;
            all++;
        }
    }
    cout<<all<<endl;
    return 0;
}