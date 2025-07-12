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
    //int t;
    //cin>>t;
    //while (t--)
    //{
    int n,m;
    cin>>n>>m;
    int whopos=0;
    cin>>whopos;
    int endpos=0;
    cin>>endpos;
    //string point[100];
    //for(int i=0;i<n;i++){
    //    cin>>point[i];
    //}
    int edge[100][100];
    int costroad[100][100];
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            edge[i][j]=100000;
            costroad[i][j]=100000;
        }
    }
    for(int i=0;i<m;i++){
        int a,b;
        cin>>a>>b;
        cin>>edge[a][b];
        cin>>costroad[a][b];
    }
    /*
    string point1;
    cin>>point1;
    
    for(int i=0;i<n;i++){
    if(point[i]==point1){
            point1=point[i];
            whopos=i;
            break;
        }
    }
    */
    int dis[1000];
    int vis[1000]={0};
    int cost[100]={0};
    vector<int> path[100];
    // dijkstra
    memset(dis,0x3f3f3f3f,sizeof(dis));
    for(int i=0;i<n;i++){
        path[i].push_back(whopos);
    }
    for(int i=0;i<n;i++){
        dis[i]=edge[whopos][i];
        cost[i]=costroad[whopos][i];
        if(dis[i]!=100000){
            path[i].push_back(i);
        }
    }
    dis[whopos]=0;
    vis[whopos]=1;
    cost[whopos]=0;
    for(int i=0;i<n;i++){
        int minn=100000;
        int minpos;
        for(int j=0;j<n;j++){
            if(vis[j]==0&&dis[j]<minn){
                minn=dis[j];
                minpos=j;
            }
        }
        vis[minpos]=1;
        for(int j=0;j<n;j++){
            if(vis[j]==0&&dis[j]>dis[minpos]+edge[minpos][j]){
                dis[j]=dis[minpos]+edge[minpos][j];
                cost[j]=cost[minpos]+costroad[minpos][j];
                vector<int> temp=path[minpos];
                temp.push_back(j);
                path[j]=temp;
            }
            if(vis[j]==0&&dis[j]==dis[minpos]+edge[minpos][j]){
                if(cost[j]>cost[minpos]+costroad[minpos][j]){
                    cost[j]=cost[minpos]+costroad[minpos][j];
                    vector<int> temp=path[minpos];
                    temp.push_back(j);
                    path[j]=temp;
                }
            }
        }
    }
    cout<<dis[endpos]<<" "<<cost[endpos]<<endl;
//}
    return 0;
}