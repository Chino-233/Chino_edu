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
int cut=-1;
int head[10];
edge ed[10];
void addedge(int u,int v,int w){
	cut++;
	ed[cut].next=head[u];
	head[u]=cut;
	ed[cut].to=v;
	ed[cut].w=w;
}
int main(){
	#ifndef ONLINE_JUDGE
	freopen("in.in","r",stdin);
	freopen("out.out","w",stdout);
	#endif
	int n,m,s;
	cin>>n>>m>>s;
	for(int i=0;i<m;i++){
		int u,v,w;
		cin>>u,v,w;
		addedge(u,v,w);
	}
}