#include <bits/stdc++.h>
using namespace std;
//const int N = 2e6 + 10;
const int N = 10;
#define ll long long

int main(){
    #ifndef ONLINE_JUDGE
    freopen("in.in", "r", stdin);
    freopen("out.out", "w", stdout);
    #endif
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin>>t;
    ll a[N]={0};
    ll b[N]={0};
    while (t--)
    {
        int n;
        cin>>n;
        int k;
        cin>>k;
        for(int i=0;i<n;i++){
            cin>>a[i];
        }
        for(int i=0;i<n;i++){
            cin>>b[i];
        }
        ll addnum=-114514;
        ll maxnum=-114514191981921;
        ll minnum=114514191981921;
        int wrongflag=0;
        for(int i=0;i<n;i++){
            minnum=min(a[i],minnum);
            maxnum=max(a[i],maxnum);
            if(b[i]!=-1){
                if(addnum==-114514){
                    addnum=a[i]+b[i];
                }
                else if(addnum!=a[i]+b[i]){
                    cout<<0<<"\n";
                    wrongflag=1;
                    break;
                }
            }
        }
        if(!wrongflag){
            for(int i=0;i<n;i++){
                if(addnum-a[i]>k){
                    cout<<0<<"\n";
                    wrongflag=1;
                    break;
                }
                if(addnum!=-114514&&addnum<a[i]){
                    cout<<0<<"\n";
                    wrongflag=1;
                    break;
                }
            }
        }
        if(addnum==-114514&&!wrongflag){
            cout<<(k+minnum)-maxnum+1<<"\n";
        }
        else if(!wrongflag){
            cout<<1<<"\n";
        }
    }
    return 0;
}