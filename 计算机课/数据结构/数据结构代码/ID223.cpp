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
    int t;
    cin>>t;
    while(t--){
        int n,k;
        cin>>n>>k;
        string a[n];
        vector<vector<int>> v(n);
        for(int i=0;i<n;i++){
            cin>>a[i];
        }
        for(int i=0;i<k;i++){
            string s1,s2;
            cin>>s1>>s2;
            for(int j=0;j<n;j++){
                if(s1==a[j]){
                    for(int l=0;l<n;l++){
                        if(s2==a[l]){
                            v[j].push_back(l);
                        }
                    }
                }
            }
        }
        for(int i=0;i<n;i++){
            cout<<i<<" "<<a[i]<<"-";
            for(int j=0;j<v[i].size();j++){
                cout<<v[i][j]<<"-";
            }
            cout<<"^"<<endl;
        }
        
    }
    return 0;
}