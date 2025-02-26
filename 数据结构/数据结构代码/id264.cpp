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
    int n,m;
    cin>>n>>m;
    int edge[100][100];
    for(int i=0;i<m;i++){
        int a,b;
        cin>>a>>b;
        edge[a][b]=1;
        edge[b][a]=1;
    }
    int k;
    cin>>k;
    while (k--)
    {
        cin>>m;
        int arr[m]={0};
        for(int i=0;i<m;i++){
            cin>>arr[i];
        }
        if(m!=n+1||arr[0]!=arr[m-1]){
            cout<<"NO"<<endl;
            continue;
        }
        int flag=0;
        for(int i=0;i<m-1;i++){
            if(edge[arr[i]][arr[i+1]]==0){
                cout<<"NO"<<endl;
                flag=1;
                break;
                //continue;
            }
        }
        if(flag==1){
            continue;
        }
        sort(arr,arr+(m-1),greater<int>());
        for(int i=0;i<m-2;i++){
            if(arr[i+1]==arr[i]){
                cout<<"NO"<<endl;
                flag=1;
            }
        }
        if(flag==1){
            continue;
        }
        cout<<"YES"<<endl;
    }
    return 0;
}