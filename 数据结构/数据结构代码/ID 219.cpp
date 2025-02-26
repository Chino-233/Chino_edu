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
int n,m;
int num;
void dfs(int x,int y,int used[][100],int a[][100]){
    if(x<0||x>=n||y<0||y>=m||used[y][x]==1||a[y][x]==1) return;
    used[y][x]=1;
    num++;
    dfs(x+1,y,used,a);
    dfs(x-1,y,used,a);
    dfs(x,y+1,used,a);
    dfs(x,y-1,used,a);
}
int main(){
    #ifndef ONLINE_JUDGE
	freopen("in.in", "r", stdin);
	freopen("out.out", "w", stdout);
    #endif
    int t;
    cin >> t;
    while(t--){
        cin >> m >> n;
        int a[100][100];
        memset(a,0,sizeof(a));
        for(int i = 0; i < m; i++){
            for(int j = 0; j < n; j++){
                cin >> a[i][j];
            }
        }
        int used[100][100];
        memset(used,0,sizeof(used));
        num=0;
        for(int i = 0; i < n; i++){
            dfs(i,0,used,a);
        }
        for(int i = 0; i < n; i++){
            dfs(i,m-1,used,a);
        }
        for(int i = 0; i < m; i++){
            dfs(0,i,used,a);
        }
        for(int i = 0; i < m; i++){
            dfs(n-1,i,used,a);
        }
        int cut=0;
        for(int i=0;i<m;i++){
            for(int j=0;j<n;j++){
                cut+=a[i][j];
            }
        }
        /*
        for(int i=0;i<m;i++){
            for(int j=0;j<n;j++){
                cout << used[i][j] << " ";
            }
            cout << endl;
        }
        cout<<endl;
        */
        cout <<(n*m)-cut-num<< endl;
    }
    return 0;
}