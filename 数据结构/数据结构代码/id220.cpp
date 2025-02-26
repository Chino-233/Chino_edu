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
    string point[100];
    for(int i=0;i<n;i++){
        cin>>point[i];
    }
    int edge[100][100];
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            cin>>edge[i][j];
            if(edge[i][j]==0){
                edge[i][j]=100000;
            }
        }
    }
    string point1;
    cin>>point1;
    int whopos=0;
    for(int i=0;i<n;i++){
        if(point[i]==point1){
            point1=point[i];
            whopos=i;
            break;
        }
    }
    int dis[1000];
    int vis[1000]={0};
    vector<int> path[100];
    // dijkstra
    memset(dis,100000,sizeof(dis));
    for(int i=0;i<n;i++){
        path[i].push_back(whopos);
    }
    for(int i=0;i<n;i++){
        dis[i]=edge[whopos][i];
        if(dis[i]!=100000){
            path[i].push_back(i);
        }
    }
    dis[whopos]=0;
    vis[whopos]=1;
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
                vector<int> temp=path[minpos];
                temp.push_back(j);
                path[j]=temp;
            }
        }
    }
    for(int i=0;i<n;i++){
        if(i==whopos){
            continue;
        }
        cout<<point[whopos]<<"-"<<point[i]<<"-";
        if(dis[i]>=100000){
            cout<<-1<<endl;
        }
        else{
            cout<<dis[i]<<"----"<<"[";
            for(int j=0;j<path[i].size();j++){
                cout<<point[path[i][j]]<<" ";
                }
            cout<<"]"<<endl;
        }
    }
}
    return 0;
}