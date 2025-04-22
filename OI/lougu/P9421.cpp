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
    int n;
    cin>>n;
    int arr[N]={0};
    for(int i=0;i<n;i++){
        int temp=0;
        cin>>temp;
        arr[temp]++;
    }
    int singlenum=0;
    int doubleupnum=0;
    for(int i=0;i<N;i++){
        if(arr[i]==1){
            singlenum++;
        }
        else if(arr[i]>2){
            doubleupnum+=arr[i]-2;
        }
    }
    int all=0;
    if(singlenum>doubleupnum){
        all+=(doubleupnum);
        singlenum-=doubleupnum;
        all+=(singlenum/2);
    }
    else{
        all+=doubleupnum;
    }
    cout<<all<<endl;
    return 0;
}