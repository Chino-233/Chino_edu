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
int judge(string s,ll n){
    if(n%2!=0){
        return 0;
    }
    else{
        ll n1=n/2;
        for(ll i=n1;i>=1;i--){
            if(s[i-1]!=s[n-i]){
                return 0;
            }
        }
        return 1;
    }
}
int main(){
    #ifndef ONLINE_JUDGE
	freopen("in.in", "r", stdin);
	freopen("out.out", "w", stdout);
    #endif
    string s;
    cin>>s;
    ll n=0;
    n=s.length();
    if(n==1){

    }
    else{
    while (judge(s,n))
    {
        n/=2;
    }
    }
    cout<<n<<endl;
    return 0;
}