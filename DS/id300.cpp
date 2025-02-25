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
    int n;
    cin>>n;
    int arr[11];
    int num=0;
    int vis[11]={0};
    for(int i=0;i<n;i++){
        cin>>arr[i];
    }
    for(int j=0;j<n;j++){
        if(arr[j]==j||vis[j]){
            vis[j]=1;
            continue;
        }
        int temp=arr[j];
        int pos=j;
        int flag=0;
        while (vis[pos]==0)
        {
            num++;
            vis[pos]=1;
            pos=arr[pos];
            if(arr[pos]==0){
                flag=1;
            }
        }
        if(flag==1){
            num--;
            flag=0;
        }
        else{
            num++;
        }
    }
    cout<<num<<endl;
    return 0;
}