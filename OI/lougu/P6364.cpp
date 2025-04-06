#include <bits/stdc++.h>
using namespace std;
//  const int N = 1e6 + 10;
  const int N = 10;
#define ll long long

int main(){
    #ifndef ONLINE_JUDGE
	freopen("in.in", "r", stdin);
	freopen("out.out", "w", stdout);
    #endif
    ll n;
    cin>>n;
    ll arr[N]={0};
    for(int i=0;i<n;i++){
        cin>>arr[i];
    }
    ll had[N]={0};
    for(int i=0;i<n;i++){
        had[i]++;
    }
    for(int i=1;i<n;i++){
        if(arr[i]>arr[i-1]){
            had[i]=max(had[i],had[i-1]+1);
        }
        else if(arr[i]==arr[i-1]){
            had[i]=max(had[i],had[i-1]);
        }
    }
    for(int i=n-2;i>=0;i--){
        if(arr[i]>arr[i+1]){
            had[i]=max(had[i],had[i+1]+1);
        }
        else if(arr[i]==arr[i+1]){
            had[i]=max(had[i],had[i+1]);
        }
    }
    ll allnum=0;
    for(int i=0;i<n;i++){
        allnum+=had[i];
    }
    cout<<allnum;
    return 0;
}