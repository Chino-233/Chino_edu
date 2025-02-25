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
void merge(string r[],string t[],int low,int mid,int high,int n){
    int i=low,j=mid+1,k=low;
    while(i<=mid&&j<=high&&j<n){
        if(r[i]>r[j]){
            t[k++]=r[i++];
        }
        else{
            t[k++]=r[j++];
        }
    }
    while(i<=mid){
        t[k++]=r[i++];
    }
    while(j<=high&&j<n){
        t[k++]=r[j++];
    }
}
void print(string r[],int n){
    for(int i=0;i<n;i++){
        cout<<r[i];
        if(i!=n-1){
            cout<<" ";
        }
        else{
            cout<<endl;
        }
    }
    //cout<<endl;
}
int main(){
    #ifndef ONLINE_JUDGE
	freopen("in.in", "r", stdin);
	freopen("out.out", "w", stdout);
    #endif
    string arr[100]={""};
    int t;
    cin >> t;
    while(t--){
        int n;
        cin >> n;
        for(int i = 0; i < n; i++){
            cin >> arr[i];
        }
        string temp[100]={""};
        int a=1;
        while (a<=n)
        {
            for(int i=0;i<n;i+=2*a){
                merge(arr,temp,i,i+a-1,i+2*a-1,n);
            }
            for(int i=0;i<n;i++){
                arr[i]=temp[i];
            }
            print(arr,n);
            a*=2;
        }
        cout<<endl;
    }
    return 0;
}