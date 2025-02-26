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
    while (t--)
    {
        int n;
        cin>>n;
        int vis[100]={0};
        int edge[100][100];
        for(int i=0;i<100;i++){
            for(int j=0;j<100;j++){
                edge[i][j]=0;
            }
        }
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                cin>>edge[i][j];
            }
        }
        for(int k=0;k<n;k++){
            for(int i=0;i<n;i++){
                int sum=0;
                if(vis[i]){
                    continue;
                }
                for(int j=0;j<n;j++){
                    sum+=edge[j][i];
                }
                if(!sum){
                    cout<<i<<" ";
                    for(int j=0;j<n;j++){
                        edge[i][j]=0;
                    }
                    vis[i]=1;
                    break;
                }
            }
        }
        cout<<endl;
    }
    return 0;
}