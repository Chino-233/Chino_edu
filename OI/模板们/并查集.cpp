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
int arr[10];
int find(int x){
    if(arr[x]==x){
        return arr[x];
    }
    else{
        return arr[x]=find(arr[x]);
    }
}
int main(){
	#ifndef ONLINE_JUDGE
	freopen("in.in","r",stdin);
	freopen("out.out","w",stdout);
	#endif
    int n,m;
    cin>>n>>m;
    for(int i=1;i<=n;i++){
        arr[i]=i;
    }
    for(int i=0;i<m;i++){
        int z,x,y;
        cin>>z>>x>>y;
        if(z==1){
            arr[find(arr[x])]=find(y);  
        }
        else if(z==2){
            if(find(x)==find(y))
            {
                cout<<"Y"<<endl;
            }
            else{
                cout<<"N"<<endl;
            }
        }
    }
}