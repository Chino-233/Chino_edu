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
int dfs(int edge[][100],int used[],int pos){
    if(used[pos]==1){
        return 0;
    }
    //cout<<pos<<" ";
    used[pos]=1;
    for(int i=0;i<100;i++){
        if(edge[pos][i]==1){
            dfs(edge,used,i);
        }
    }
    //used[pos]=0;
    return 1;
}
int main(){
    #ifndef ONLINE_JUDGE
	freopen("in.in", "r", stdin);
	freopen("out.out", "w", stdout);
    #endif
    int t;
    cin>>t;
    while(t--){
        int n;
        cin>>n;
        string point[100];
        int used[100]={0};
        for(int i=0;i<n;i++){
            used[i]=0;
            cin>>point[i];
        }
        for(int i=0;i<n;i++){
            cout<<point[i];
            if(i!=n-1){
                cout<<" ";
            }
            else{
                cout<<endl;
            }
        }
        int edge[100][100];
        for(int i=0;i<100;i++){
            for(int j=0;j<100;j++){
                edge[i][j]=0;
            }
        }
        int m;
        cin>>m;
        for(int i=0;i<m;i++){
            string a,b;
            cin>>a>>b;
            for(int j=0;j<n;j++){
                if(point[j]==a){
                    for(int k=0;k<n;k++){
                        if(point[k]==b){
                            edge[j][k]=1;
                            edge[k][j]=1;
                        }
                    }
                }
            }
        }
        int cut=0;
        for(int i=0;i<n;i++){
            if(used[i]==0){
                dfs(edge,used,i);
                cut++;
            }
        }
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                cout<<edge[i][j];
                if(j!=n-1){
                    cout<<" ";
                }
                else{
                    cout<<endl;
                }
            }
        }
        cout<<cut<<endl;
        cout<<endl;
    }
    return 0;
}