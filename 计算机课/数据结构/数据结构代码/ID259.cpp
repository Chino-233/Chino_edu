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
void dfs(int edge[][100],int used[],int pos,int &height,int &max){
    if(used[pos]==1){
        return;
    }
    if(height>max){
        max=height;
    }
    cout<<pos<<" ";
    used[pos]=height;
    for(int i=0;i<100;i++){
        if(edge[pos][i]==1){
            height++;
            dfs(edge,used,i,height,max);
            height--;
        }
    }
    //used[pos]=0;
    return;
}
int main(){
    #ifndef ONLINE_JUDGE
	freopen("in.in", "r", stdin);
	freopen("out.out", "w", stdout);
    #endif
    long long n,m;
    cin >> n >> m;
    long long edge[300][300];
    for(long long i=0;i<300;i++){
        for(long long j=0;j<300;j++){
            edge[i][j]=0;
        }
    }
    for(long long i=0;i<m;i++){
        long long a,b;
        cin >> a >> b;
        edge[a][b]=1;
        edge[b][a]=1;
    }
    long long k;
    cin >> k;
    for(long long i=0;i<k;i++){
        long long pos;
        cin >> pos;
        long long used[10000]={0};
        for(long long i=0;i<10000;i++){
            used[i]=0;
        }
        queue<long long> q,q1;
        //dfs(edge,used,i);
        q.push(pos);
        long long height=0;
        used[pos]=height;
        while(!q.empty()){
            long long pos1=q.front();
            q.pop();
            //cout<<pos<<" ";
            for(long long i=0;i<200;i++){
                if(edge[pos1][i]==1&&used[i]==0&&i!=pos){
                    q.push(i);
                    used[i]=used[pos1]+1;
                }
            }
        }
        double sum=0.0;
        int cut=0;
        for(long long i=1;i<=n;i++){
            sum+=used[i];
            if(used[i]!=0){
                cut++;
            }
        }
        sum/=(double)(n-1);
        cout<<"Cc("<<pos<<")=";
        if(sum==0.00){
            cout<<"0.00"<<endl;
        }
        else{
            sum=1.0/sum;
            cout<<fixed<<setprecision(2)<<sum<<endl;
        }
    }
    return 0;
}