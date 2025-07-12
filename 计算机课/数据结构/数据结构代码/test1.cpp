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
void ShellSort(int gap,int arr[],int n){
    for(int k=0;k<gap;k++){
    for(int i=k;i<n;i+=gap){
        int temp=arr[i];
        for(int j=i-gap;j>=0;j-=gap){
            if(temp>arr[j]){
                swap(arr[j],arr[j+gap]);
            }
        }
    }
    }
    for(int i=1;i<n;i++){
        int temp=arr[i];
        for(int j=i-1;j>=0;j--){
            if(temp<arr[j]){
                swap(arr[j],arr[j+1]);
            }
        }
    }
}
int main(){
    #ifndef ONLINE_JUDGE
	freopen("in.in", "r", stdin);
	freopen("out.out", "w", stdout);
    #endif
    /*
    int n;
    cin>>n;
    int arr[100];
    for(int i=0;i<n;i++){
        cin>>arr[i];
    }
    for(int i=1;i<n;i++){
        int temp=arr[i];
        for(int j=i-1;j>=0;j--){
            if(temp<arr[j]){
                swap(arr[j],arr[j+1]);
            }
        }
        for(int i=0;i<n;i++){
            cout<<arr[i];
            if(i!=n-1){
                cout<<" ";
            }
            else{
                cout<<endl;
            }
        }
    }
    */
    int t;
    cin>>t;
    while (t--)
    {
        int n;
        cin>>n;
        int arr[100]={0};
        for(int i=0;i<n;i++){
            cin>>arr[i];
        }
        for(int gap=n/2;gap>=1;gap/=2){
            ShellSort(gap,arr,n);
            for(int i=0;i<n;i++){
            cout<<arr[i];
            if(i!=n-1){
                cout<<" ";
            }
            else{
                cout<<endl;
            }
        }
        }
        cout<<endl;
    }
    return 0;
}