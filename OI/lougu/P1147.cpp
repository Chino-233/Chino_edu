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
    ll m;
    cin>>m;
    ll k1=1;
    ll k2=1;
    ll num=0;
    while (k1<m&&k2<m)
    {
        if(num<m){
            num+=k1;
            k1++;
        }
        else if(num>m){
            num-=k2;
            k2++;
        }
        else if(num==m){
            cout<<k2<<" "<<k1-1<<endl;
            num+=k1;
            k1++;
        }
    }
    
    return 0;
}