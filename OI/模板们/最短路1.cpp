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
int main(){
	#ifndef ONLINE_JUDGE
	freopen("in.in","r",stdin);
	freopen("out.out","w",stdout);
	#endif
    int n,m;
    cin>>n>>m;
    int adj[10][10]={0};
    for(int i=0;i<m;i++){
        int u,v,w;
        cin>>u>>v>>w;
        adj[u][v]=w;
    }
    int f[10][10];
    memset(f,0x3f,sizeof(f));
    for(int i=1;i<=n;i++){
        for(int j=1;j<=n;j++){
            if(adj[i][j]){
                f[i][j]=adj[i][j];
                f[j][i]=adj[i][j];
            }
        }
    }
    for(int k=1;k<=n;k++){
        for(int x=1;x<=n;x++){
            for(int y=1;y<=n;y++){
                f[x][y]=min(f[x][y],f[x][k]+f[k][y]);
            }
        }
    }
    for(int i=1;i<=n;i++){
        f[i][i]=0;
    }
    for(int i=1;i<=n;i++){
        for(int j=1;j<=n;j++){
            if(f[i][j]>100000){
                cout<<0<<" ";
            }
            else{
            cout<<f[i][j]<<" ";
            }
        }
        cout<<endl;
    }
}