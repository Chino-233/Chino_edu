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

int main(){
	#ifndef ONLINE_JUDGE
	freopen("in.in","r",stdin);
	freopen("out.out","w",stdout);
	#endif
    int n,m,s;
    cin>>n>>m>>s;
    int head[10]={0};
    edge ed[10];
    int cut=0;
    for(int i=0;i<m;i++){
        int u,v,w;
        cin>>u>>v>>w;
        cut++;
        ed[cut].to=v;
        ed[cut].w=w;
        ed[cut].next=head[u];
        head[u]=cut;
    }//链式前向星
    int dis[10];
    	for(int i=1;i<=n;i++)
	{
		dis[i]=2147483647;
	}
    dis[s]=0;
    /*for(int u=1;u<=n;u++){
        if(head[u]==0){
            continue;
        }
        for(int i=head[u];i!=0;i=ed[i].next){
            if(dis[ed[i].to]>dis[u]+ed[i].w){
                dis[ed[i].to]=dis[u]+ed[i].w;
            }
        }
    }//基础写法
    */
    queue<int>q;
    q.push(s);
    int vis[10]={0};
    vis[s]=1;
    while (!q.empty())
    {   
        int u=q.front();
        vis[u]=0;
        q.pop();
        for(int i=head[u];i!=0;i=ed[i].next){
            if(dis[ed[i].to]>dis[u]+ed[i].w){
                dis[ed[i].to]=dis[u]+ed[i].w;
                if(vis[ed[i].to]==0){
                    vis[ed[i].to]=1;
                    q.push(ed[i].to);
                }   
            }
        }  
    }
    
    for(int i=1;i<=n;i++){
        cout<<dis[i]<<" ";
    }
}