#include <bits/stdc++.h>
using namespace std;

// const int N = 1e6 + 10;
const int N = 10;
#define ll long long
ll binpow(ll a,ll b,ll m=10000){
    a%=m;
	ll res=1;
	while(b>0){
		if(b&1)res=res*a%m;
		a=a*a%m;
		b>>=1;
	}
	return res;
}
int main(){
    #ifndef ONLINE_JUDGE
    freopen("in.in", "r", stdin);
    freopen("out.out", "w", stdout);
    #endif
    int n;
    cin>>n;
    while (n--)
    {
        int a,b;
        cin>>a>>b;
        ll sum[10010]={0};
        for(int i=1;i<=10000;i++){
            sum[i]+=(sum[i-1]+binpow(i,b,10000))%10000;
        }
        ll ans=(a/10000*sum[10000]+sum[a%10000])%10000;
        cout<<ans<<endl;
    }
    
    return 0;
}
