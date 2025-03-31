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
const int N = 1e5 + 10;
typedef double db;
#define ll long long

int main(){
    #ifndef ONLINE_JUDGE
	freopen("in.in", "r", stdin);
	freopen("out.out", "w", stdout);
    #endif
    ll n,m,k=0;
    cin>>n>>m>>k;
    ll bread[1000]={0};
    ll hot[1000]={0};
    ll milk[1000]={0};
    for(int i=0;i<n;i++){
        cin>>bread[i];
    }
    for(int i=0;i<m;i++){
        cin>>hot[i];
    }
    for(int i=0;i<k;i++){
        cin>>milk[i];
    }
    ll num=((bread[0]*hot[0])%milk[0]);
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            //for(int k=0;k<m;k++){//你无敌了，上面用k这里还能用k
            //    if(num<((bread[i]*hot[j])%milk[k])){
            //        num=((bread[i]*hot[j])%milk[k]);
            //    }
            //}
            for(int c=0;c<k;c++){
                if(num<((bread[i]*hot[j])%milk[c])){
                    num=((bread[i]*hot[j])%milk[c]);
                }
            }
        }
    }
    cout<<num;
    return 0;
}