#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <cstring>
#include <algorithm>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <iomanip>
#include <cmath>
#include <map>
#include <stack>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <bitset>
#include <list>
#include <deque>
#include <forward_list>
#include <array>
#include <tuple>
#include <utility>
#include <memory>
#include <functional>
#include <iterator>
#include <numeric>
using namespace std;
const int N = 1e6 + 10;
typedef double db;
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
    ll del[N]={0};
    del[0]=1;
    ll maxpos=0;
    ll nownum=0;
    ll uppos=0;
    for(int i=0;i<n;i++){
        nownum+=del[i];
        if(nownum<=0){
            del[maxpos]+=(-nownum)+1;
        }
        if(arr[i+1]>arr[i]){
            del[i+1]=1;
            uppos=i+1;
            if(nownum>1&&del[i]<0){
                del[maxpos+1]-=(nownum-1);
            }
        }
        else if(arr[i+1]==arr[i]){
            del[i+1]=0;
        }
        else{
            if(del[i]>=0){
                maxpos=uppos;
            }
            del[i+1]=-1;
        }
        if(i==n-1){
            if(nownum>1&&del[i]<=0){
                del[maxpos+1]-=(nownum-1);
            }
        }
    }
    nownum=0;
    ll allnum=0;
    for(int i=0;i<n;i++){
        nownum+=del[i];
        allnum+=nownum;
        //cout<<i<<":"<<nownum<<endl;
    }
    cout<<allnum;
    return 0;
}