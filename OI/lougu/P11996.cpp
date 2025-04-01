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
    ll a=2;
    ll b=5;
    ll numa=1;
    ll numb=1;
    string s;
    ll num=0;
    ll arr[20]={0};
    ll pos=0;
    for(ll i=1;i<20;i++){
        numa*=a;
        numb*=b;
        s=to_string(numa);
        //cout<<2<<"^"<<i<<":"<<s[0]<<endl;//2 4 8 1 3 6 5
        ll temp=s[0]-'0';
        ll flag=0;
        s=to_string(numb);
        //cout<<5<<"^"<<i<<":"<<s[0]<<endl;//5 2 1 6 3 7 9 4
        for(ll j=0;j<20;j++){
            if(arr[j]==temp*(s[0]-'0')){
                flag=1;
            }
            if(j==9){
                arr[pos]=temp*(s[0]-'0');
                pos++;
                break;
            }
        }
        if(!flag){
            num+=temp*(s[0]-'0');
        }
    }
    cout<<num<<endl;
    return 0;
}