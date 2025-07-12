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
void dfs(double edge[][100],int used[],int pos,double money,int &flag,int first){
    if(used[pos]==1){
        //cout<<pos<<"used"<<money<<endl;
        if(money>1.0&&pos==first){
            flag=1;
        }
        return;
    }
    //cout<<"pos:"<<pos;
    used[pos]=1;
    for(int i=0;i<100;i++){
        if(edge[pos][i]!=0){
            dfs(edge,used,i,money*edge[pos][i],flag,first);
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
    int t,n;
    cin>>t;
    while(t--){
        int m;
        cin>>n>>m;
        double edge[100][100];
        string edgestone[100];
        for(int i=0;i<n;i++){
            cin>>edgestone[i];
        }
        for(int i=0;i<100;i++){
            for(int j=0;j<100;j++){
                edge[i][j]=0;
            }
        }
        for(int i=0;i<m;i++){
            int a,b;
            double change=0.0;
            string s1,s2;
            cin>>s1;
            cin>>change;
            cin>>s2;
            for(int i=0;i<100;i++){
                if(edgestone[i]==s1){
                    a=i;
                    break;
                }
            }
            for(int i=0;i<100;i++){
                if(edgestone[i]==s2){
                    b=i;
                    break;
                }
            }
            
            edge[a][b]=change;
        }
        int used[n]={0};
        double money=1;
        int flag=0;
        int i=0;
        for(int i=0;i<n;i++){
            dfs(edge,used,i,money,flag,i);
        }
        //cout<<endl;
        if(flag==1){
            cout<<"YES"<<endl;
        }
        else{
            cout<<"NO"<<endl;
        }
    }
    return 0;
}