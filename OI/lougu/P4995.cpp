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
    sort(arr,arr+n);
    ll left=0;
    ll right=n-1;
    ll all=0;
    ll pos=right;
    all+=pow(arr[right],2);
    pos=right;
    while(left<=right){
        if(pos==left){
            all+=pow((arr[right]-arr[left]),2);
            pos=right;
            left++;
        }
        else{
            all+=pow((arr[right]-arr[left]),2);
            pos=left;
            right--;
        }
    }
    cout<<all;
    return 0;
}