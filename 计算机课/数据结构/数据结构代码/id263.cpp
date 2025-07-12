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
struct point
{
    int x;
    int y;
    bool out;
};
void dfs(int edge[][100],int used[],int pos,bool &out,point po[]){
    if(used[pos]==1){
        return;
    }
    //cout<<pos<<" ";
    used[pos]=1;
    for(int i=0;i<100;i++){
        if(edge[pos][i]==1){
            if(po[i].out==true){
                out=true;
            }
            dfs(edge,used,i,out,po);
        }
    }
    used[pos]=0;
    return;
}
int main(){
    #ifndef ONLINE_JUDGE
	freopen("in.in", "r", stdin);
	freopen("out.out", "w", stdout);
    #endif
    //int t,n;
    //cin>>t;
    //while(t--){
        int n,d;
        cin>>n>>d;
        int edge[1000][1000];
        point po[1000];
        po[0].x=0;po[0].y=0;po[0].out=false;
        for(int i=1;i<n+1;i++){
            cin>>po[i].x>>po[i].y;
        }
        for(int i=0;i<1000;i++){
            for(int j=0;j<1000;j++){
                edge[i][j]=0;
            }
        }
        for(int i=1;i<=n;i++){
            for(int j=1;j<=n;j++){
                if(i==j){
                    edge[i][j]=0;
                }else{
                    if(sqrt(abs(po[i].x-po[j].x)*abs(po[i].x-po[j].x)+abs(po[i].y-po[j].y)*abs(po[i].y-po[j].y))<=d&&i!=j){
                        edge[i][j]=1;
                        edge[j][i]=1;
                    }
                    else{
                        edge[i][j]=0;
                        edge[j][i]=0;
                    }
                }
            }
        }
        for(int i=1;i<=n;i++){
            if(sqrt(abs(po[i].x)*abs(po[i].x)+abs(po[i].y)*abs(po[i].y))<=d+7.5){
                edge[0][i]=1;
                edge[i][0]=1;
            }
            else{
                edge[0][i]=0;
                edge[i][0]=0;
            }
        }
        for(int i=1;i<=n;i++){
            if(d>=50-abs(po[i].x) || d>=50-abs(po[i].y)){
                po[i].out=true;
            }
            else{
                po[i].out=false;
            }
        }
        /*
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                cin>>edge[i][j];
            }
        }
        */
        int used[n]={0};
        int out=0;
        //for(int i=0;i<n;i++){
            //dfs(edge,used,i,out,po);
        //}
        queue<int> q;
            //dfs(edge,used,i);
            q.push(0);
            while(!q.empty()){
                if(out==1){
                    break;
                }
                int pos=q.front();
                if(po[pos].out==true){
                    out=1;
                }
                q.pop();
                if(used[pos]==1){
                    continue;
                }
                //cout<<pos<<" ";
                used[pos]=1;
                for(int i=0;i<100;i++){
                    if(edge[pos][i]==1){
                        q.push(i);
                    }
                }
            }
        if(out==1){
            cout<<"Yes"<<endl;
        }
        else{
            cout<<"No"<<endl;
        }
    //}
    return 0;
}