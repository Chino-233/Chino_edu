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
void dfs(int edge[][100],int used[],int pos,int height){
    if(used[pos]==1||height>6){
        return;
    }
    used[pos]=1;
    for(int i=0;i<100;i++){
        if(edge[pos][i]==1){
            dfs(edge,used,i,height+1);
        }
    }
    return;
}
int main(){
    #ifndef ONLINE_JUDGE
    freopen("in.in", "r", stdin);
    freopen("out.out", "w", stdout);
    #endif
    int t,n;
    cin>>t;
        cin>>n;
        int edge[100][100];
        for(int i=0;i<100;i++){
            for(int j=0;j<100;j++){
                edge[i][j]=0;
            }
        }
        for(int i=0;i<n;i++){
            int a,b;
            cin>>a>>b;
            edge[a][b]=1;
            edge[b][a]=1;
        }
        int used[100]={0};
        int height=0;
        for(int i=1;i<t+1;i++){
            if(i==10){
                int a=0;
            }
            dfs(edge,used,i,height);
            double cut=0.0;
            for(int j=0;j<t+1;j++){
                if(used[j]==1){
                    cut++;
                }
            }
            cut*=100.0;
            cut/=t;
            cout << i << ": " << fixed << setprecision(2) << cut << "%" << endl;
            for(int j=1;j<t+1;j++){
                used[j]=0;
                height=0;
            }
        }

    return 0;
}