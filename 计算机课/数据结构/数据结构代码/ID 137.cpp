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
    int n;
    cin>>n;
    queue<int> a;
    queue<int> b;
    for(int i=0;i<n;i++){
        int id;
        cin>>id;
        if(id%2==1){
            a.push(id);
        }
        else{
            b.push(id);
        }
    }
    while(!(a.empty()&&b.empty())){
        if(!a.empty()){
            cout<<a.front()<<" ";
            a.pop();
        }
        if(!a.empty()){
            cout<<a.front()<<" ";
            a.pop();
        }
        if(!b.empty()){
            cout<<b.front()<<" ";
            b.pop();
        }
    }
    return 0;
}