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
void dfs(int edge[][100],int used[],int pos){
    if(used[pos]==1){
        return;
    }
    cout<<pos<<" ";
    used[pos]=1;
    for(int i=0;i<100;i++){
        if(edge[pos][i]==1){
            dfs(edge,used,i);
        }
    }
    //used[pos]=0;
    return;
}
int main(){
    #ifndef ONLINE_JUDGE
	freopen("in.in", "r", stdin);
	freopen("out.out", "w", stdout);
    #endif
    int t,n;
    cin>>t;
    while(t--){
        cin>>n;
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
        int used[n]={0};
        queue<int> q;
        for(int i=0;i<n;i++){
            //dfs(edge,used,i);
            q.push(i);
            while(!q.empty()){
                int pos=q.front();
                q.pop();
                if(used[pos]==1){
                    continue;
                }
                cout<<pos<<" ";
                used[pos]=1;
                for(int i=0;i<100;i++){
                    if(edge[pos][i]==1){
                        q.push(i);
                    }
                }
            }
        }
       
        cout<<endl;
    }
    return 0;
}