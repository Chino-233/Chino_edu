#include <bits/stdc++.h>
using namespace std;

// const int N = 1e6 + 10;
const int N = 10;
#define ll long long
struct farmer{
    int price;
    int maxproduct;
};
bool cmp(farmer a,farmer b){
    return a.price<b.price;
}
int main(){
    #ifndef ONLINE_JUDGE
    freopen("in.in", "r", stdin);
    freopen("out.out", "w", stdout);
    #endif
    int n=0,m=0;
    cin>>n>>m;
    farmer arr[N];
    for(int i=0;i<m;i++){
        cin>>arr[i].price;
        cin>>arr[i].maxproduct;
    }
    sort(arr,arr+m,cmp);
    long all=0;
    for(int i=0;i<m;i++){
        if(n>=arr[i].maxproduct){
            n-=arr[i].maxproduct;
            all+=arr[i].maxproduct*arr[i].price;
        }
        else{
            all+=arr[i].price*n;
            break;
        }
    }
    cout<<all<<endl;
    return 0;
}