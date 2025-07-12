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
    string s1,s2;
    cin>>s1>>s2;
    int n = s1.size();
    int m = s2.size();
    int next[100];
    next[0] = -1;
    next[1] = 0;
    int i=1,j=0;
    s2+='0';
    while (i<=m)
    {
        if(j==-1||s2[i]==s2[j]){
            next[++i]=++j;
        }
        else{
            j=next[j];
        }
    }
    for(i=0,j=0;i<n;){
        if(s1[i]==s2[j]||j==-1){
            i++;
            j++;
        }
        else{
            j=next[j];
        }
        if(j==m){
            cout<<i-m+1<<endl;
            j=next[j];
        }
    }
    for(i=0;i<=m;i++){
        cout<<next[i]<<" ";
    }
    return 0;
}