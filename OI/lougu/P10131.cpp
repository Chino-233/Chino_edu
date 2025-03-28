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
    int t;
    cin>>t;
    while (t--)
    {
        int n;
        cin>>n;
        ll arr[101]={0};
        for(int i=0;i<n;i++){
            cin>>arr[i];
        }
        if(n==2){
            if(arr[0]==arr[1]){
                cout<<arr[0]<<endl;
            }
            else{
                cout<<-1<<endl;
            }
            continue;
        }
        ll flag=0;
        ll flag1=0;
        ll ans[101]={0};
        for(int i=1;i<n-1;i++){
            flag=0;
            if(i==4){
                int a=0;
            }
            if(arr[i-1]==arr[i]){
                flag=1;
            }
            else if(arr[i-1]==arr[i+1]){
                flag=2;
            }
            else if(arr[i]==arr[i+1]){
                flag=3;
            }
            if((flag==2||flag==1)&&ans[arr[i-1]]!=1){
                //cout<<arr[i-1]<<" ";
                ans[arr[i-1]]=1;
                flag1++;
            }
            else if(flag==3&&ans[arr[i]]!=1){
                //cout<<arr[i]<<" ";
                ans[arr[i]]=1;
                flag1++;
            }
        }
        ll flag2=0;
        for(int i=1;i<n+1;i++){
            if(ans[i]==1){
                if(flag2){
                    cout<<" ";
                }
                flag2=1;
                cout<<i;
            }
        }
        if(!flag1){
            cout<<-1;
        }
        cout<<endl;
    } 
    return 0;
}