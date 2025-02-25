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
    used[pos]=1;
    for(int i=0;i<100;i++){
        if(edge[pos][i]==1){
            dfs(edge,used,i);
        }
    }
    return;
}
int main(){
    #ifndef ONLINE_JUDGE
    freopen("in.in", "r", stdin);
    freopen("out.out", "w", stdout);
    #endif
        int n,m;
        cin>>n;
        cin>>m;
        int edge[100][100];
        for(int i=0;i<100;i++){
            for(int j=0;j<100;j++){
                edge[i][j]=0;
            }
        }
        for(int i=0;i<m;i++){
            int a,b;
            cin>>a>>b;
            edge[a][b]=1;
            edge[b][a]=1;
        }
        int used[100]={0};
        int allcut=0;
        for(int i=0;i<n;i++){
            if(used[i]==0){
                dfs(edge,used,i);
                allcut++;
            }
        }
        int t;
        cin>>t;
        for(int i=0;i<t;i++){
            int a;
            cin>>a;
            used[a]=114514;
            for(int j=0;j<100;j++){
                edge[a][j]=0;
                edge[j][a]=0;
            }
            for(int j=0;j<100;j++){
                if(used[j]!=114514){
                    used[j]=0;
                }
            }
            int cut=0;
            for(int i=0;i<n;i++){
                if(used[i]==0){
                    dfs(edge,used,i);
                    cut++;
                }
            }
            if(cut>allcut){
                cout<<"Red Alert: City "<<a<<" is lost!"<<endl;
            }
            else{
                cout<<"City "<<a<<" is lost."<<endl;
            }
            allcut=cut;
            int num=0;
            for(int j=0;j<n;j++){
                if(used[j]==114514){
                    num++;
                }
                if(num==n){
                    cout<<"Game Over."<<endl;
                    return 0;
                }
            }
        }
        
    return 0;
}