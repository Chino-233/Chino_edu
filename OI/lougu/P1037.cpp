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
int main(){
    #ifndef ONLINE_JUDGE
	freopen("in.in", "r", stdin);
	freopen("out.out", "w", stdout);
    #endif
    string n;
    cin>>n;
    int k=0;
    cin>>k;
    int edge[10][10]={0};
    for(int i=0;i<k;i++){
        int m,n=114514;
        cin>>m>>n;
        edge[m][n]=1;
    }
    queue<int>q;
    int num[10]={0};
    for(int d=0;d<10;d++){
    q.push(d);
    int flag[10]={0};
    while (!q.empty())
    {
        int a=q.front();
        q.pop();
        for(int i=0;i<10;i++){
            if(edge[a][i]==1&&flag[i]!=1){
                flag[i]=1;
                q.push(i);
            }
        }
    }
    for(int i=0;i<10;i++){
        num[d]+=flag[i];
    }
    num[d]++;
    }
    int result=1;
    for(int i=0;i<n.length();i++){
        result*=(num[n[i]-'0']);
    }
    cout<<result;
    return 0;
}