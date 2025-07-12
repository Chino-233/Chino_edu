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
#define INF 1145141919
struct edges{
    int u;
    int v;
    int w;
};
int prim(int edge[][100],int start,int n,int vis[],map<int,string>mp2){
    int minu=0;
    int minv=0;
    int minedge=INF;
    int minall=0;
    vis[start]=1;
    edges stone[100];
    for(int i=0;i<n-1;i++){
        minedge=INF;
        for(int k=0;k<n;k++){
            if(vis[k]){
                for(int j=0;j<n;j++){
                    if(!vis[j]&&edge[k][j]<minedge){
                        minedge=edge[k][j];  
                        minu=k;
                        minv=j;
                    }
                }
            }
        }
        vis[minv]=1;
        stone[i].u=minu;
        stone[i].v=minv;
        stone[i].w=minedge;
        minall+=minedge;
    }
    cout<<minall<<endl;
    cout<<"prim:"<<endl;
    for(int i=0;i<n-1;i++){
        cout<<mp2[stone[i].u]<<" "<<mp2[stone[i].v]<<" "<<stone[i].w<<endl;
    }
    return 1;
}
bool cmp(edges e1,edges e2){
    return e1.w<e2.w;
}
int kruskal(edges kru[],int n,int m,int vis[],map<int,string>mp2){
    int namepos=1;
    sort(kru,kru+m,cmp);
    int stone[100]={0};
    int minall=0;
    int i=0;
        for(int j=0;j<m;j++){
            if(vis[kru[j].u]==0&&vis[kru[j].v]==0){
                vis[kru[j].u]=namepos;
                vis[kru[j].v]=namepos;
                namepos++;
                stone[i]=j;
                i++;
                minall+=kru[j].w;
               
            }
            else if(vis[kru[j].u]==0&&vis[kru[j].v]!=0){
                vis[kru[j].u]=vis[kru[j].v];
                stone[i]=j;
                i++;
                minall+=kru[j].w;
            }
            else if(vis[kru[j].u]!=0&&vis[kru[j].v]==0){
                vis[kru[j].v]=vis[kru[j].u];
                stone[i]=j;
                i++;
                minall+=kru[j].w;
                
            }
            else if(vis[kru[j].u]!=0&&vis[kru[j].v]!=0&&vis[kru[j].u]!=vis[kru[j].v]){
                int temp1=vis[kru[j].u];
                int temp2=vis[kru[j].v];
                for(int i=0;i<n;i++){
                    if(vis[i]==temp1||vis[i]==temp2){
                        vis[i]=min(vis[kru[j].u],vis[kru[j].v]);
                    }
                }
                minall+=kru[j].w;
                stone[i]=j;
                i++;
            }
            else{

            }
        }
    //cout<<minall<<endl;
    cout<<"kruskal:"<<endl;
    for(int j=0;j<n-1;j++){
        
    }
    for (int j = 0; j < i; j++) {
        int pos1 = 0; int pos2 = 0; // 定义两个点的编号
        for (int i = 0; i < n; i++) {
            pos1 = kru[stone[j]].u; // 获取点的编号
            pos2 = kru[stone[j]].v; // 获取点的编号
        }
        if (pos1 > pos2) {
            cout<<mp2[kru[stone[j]].v]<<" "<<mp2[kru[stone[j]].u]<<" "<<kru[stone[j]].w<<endl;
        } else {
            cout<<mp2[kru[stone[j]].u]<<" "<<mp2[kru[stone[j]].v]<<" "<<kru[stone[j]].w<<endl;
        }
    }
    return 1;
}
int main(){
    #ifndef ONLINE_JUDGE
	freopen("in.in", "r", stdin);
	freopen("out.out", "w", stdout);
    #endif
    int n;
    cin>>n;
    map<string,int>mp;
    map<int,string>mp2;
    edges kru[100];
    for(int i=0;i<n;i++){
        string s;
        cin>>s;
        mp[s]=i;
        mp2[i]=s;
        
    }
    int edge[100][100]={0};
    for(int i=0;i<100;i++){
        for(int j=0;j<100;j++){
            edge[i][j]=INF;
        }
    }
    int m;
    cin>>m;
    int minedge=INF;
    int minu=0;
    int minv=0;
    for(int i=0;i<m;i++){
        string s1,s2;
        cin>>s1>>s2;
        int w;
        cin>>w;
        kru[i].u=mp[s1];
        kru[i].v=mp[s2];
        kru[i].w=w;
        edge[mp[s1]][mp[s2]]=w;
        edge[mp[s2]][mp[s1]]=w;
        if(w<minedge){
            minedge=w;
            minu=mp[s1];
            minv=mp[s2];
        }
    }
    string s;
    cin>>s;
    int start=mp[s];
    int vis[100]={0};
    prim(edge,start,n,vis,mp2);
    int vis1[100]={0};
    kruskal(kru,n,m,vis1,mp2);
    return 0;
}