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
void dfs(int edge[][100],int pos,int vis[],int n){
    if(vis[pos]){
        return;
    }
    cout<<pos<<" ";
    vis[pos]=1;
    for(int i=0;i<n;i++){
        if(edge[pos][i]!=0){
            if(!vis[i]){
                dfs(edge,i,vis,n);
                //vis[i]=0;
            }
        }
    }
}
int main(){
    #ifndef ONLINE_JUDGE
	freopen("in.in", "r", stdin);
	freopen("out.out", "w", stdout);
    #endif
    int n;
    cin>>n;
    int edge[100][100];
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            cin>>edge[i][j];
        }
    }
    int m;
    cin>>m;
    int vis[100]={0};
    cout<<"DFS from "<<m<<": ";
    dfs(edge,m,vis,n);
    for(int i=0;i<n;i++){
        dfs(edge,i,vis,n);
    }
    return 0;
}