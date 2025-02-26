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
void print(int arr[], int n){
    cout<<n<<" ";
    for(int i = 0; i < n; i++){
        cout << arr[i];
        if(i != n-1){
            cout << " ";
        }
        else{
            cout << endl;
        }
    }
    //cout << endl;
}
void bulidHeapsort(int arr[],int n,int i){
    if(2*i+1>=n){
        return;
    }
    int child=2*i+1;
    int minnum=arr[i];
    if(arr[2*i+1]<arr[i]){
        minnum=arr[2*i+1];
    }
    if(2*i+2<n){
        if(arr[2*i+1]>arr[2*i+2]){
            minnum=arr[2*i+2];
            child++;
        }
    }
    if(minnum<arr[i]){
        swap(arr[i],arr[child]);
        bulidHeapsort(arr,n,child);
    }
    return;
}
void bulidsort(int arr[],int n,int i){
    if(2*i+1>=n){
        return;
    }
    int child=2*i+1;
    int minnum=arr[i];
    if(arr[i]>arr[child]){
        minnum=arr[child];
    }
    if(2*i+2<n){
        if(arr[2*i+2]<arr[2*i+1]){
            child++;
            minnum=arr[2*i+2];
        }
    }
    if(minnum<arr[i]){
        swap(arr[i],arr[child]);
        bulidsort(arr,n,child);
    }
    return;
}
int main(){
    #ifndef ONLINE_JUDGE
	freopen("in.in", "r", stdin);
	freopen("out.out", "w", stdout);
    #endif
    int n;
    int arr[100]={0};
    cin>>n;
    for(int i=0;i<n;i++){
        cin>>arr[i];
    }
    for(int i=n/2-1;i>=0;i--){
        //bulidHeapsort(arr,n,i);
        bulidsort(arr,n,i);
    }
    print(arr,n);
    //cout<<endl;
    for(int i=n-1;i>0;i--){
        swap(arr[0],arr[i]);
        //print(arr,n);
        //cout<<"swap"<<endl;
        //bulidHeapsort(arr,i,0);
        bulidsort(arr,i,0);
        print(arr,n);
        //cout<<endl;
    }
    return 0;
}