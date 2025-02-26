#include<iostream>
#include<string>
#include<algorithm>
#include<vector>
#include<stack>
#include<queue>
#include<cstring>
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
using namespace std;
struct edge
{
	int to;
	int next;
	int w;
};
struct point
{
    int id;
    int dis;
    bool operator <(const point &x)const
    {
        return x.dis<dis;
    }
};
int main(){
	#ifndef ONLINE_JUDGE
	freopen("in.in","r",stdin);
	freopen("out.out","w",stdout);
	#endif
    int cut=0;
    int head[10]={0};
    edge ed[10];
	int n,m,s;
	cin>>n>>m>>s;
	for(int i=0;i<m;i++){
		int u,v,w;
		cin>>u>>v>>w;
		cut++;
	    ed[cut].next=head[u];
	    head[u]=cut;
	    ed[cut].to=v;
	    ed[cut].w=w;
	}
    int vis[10]={0};
    int curr=s;
    int dis[10];
    for(int i=1;i<=m;i++){
        dis[i]=2147483647;
    }
    dis[s]=0;
    priority_queue<point>q;
    q.push((point){s,0});
    while (!q.empty())
    {
        point temp;
        temp=q.top();
        q.pop();
        curr=temp.id;
        long long minn=2147483647;
        if(!vis[curr]){
            vis[curr]=1;
            for(int i=head[curr];i;i=ed[i].next){
                    if(dis[ed[i].to]>dis[curr]+ed[i].w){
                        dis[ed[i].to]=min(dis[ed[i].to],dis[curr]+ed[i].w);
                        if(!vis[ed[i].to]){
                            q.push(point{ed[i].to,dis[ed[i].to]});
                        }
                    
                }
            }
        }
        
    }
    for(int i=1;i<=n;i++){
        cout<<dis[i]<<" ";
    }
    
}