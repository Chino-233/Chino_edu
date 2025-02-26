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
int find(int arr[],int n,int i){
    for(int j=0;j<n;j++){
        if(arr[j]==i){
            return j;
        }
    }
    return -1;
}
int main(){
    #ifndef ONLINE_JUDGE
	freopen("in.in", "r", stdin);
	freopen("out.out", "w", stdout);
    #endif
    int n;
    int arr[100]={0};
    cin>>n;
    int m;
    cin>>m;
    for(int i=0;i<n;i++){
        cin>>arr[i];
    }
    for(int i=n/2-1;i>=0;i--){
        bulidHeapsort(arr,n,i);
    }
    //print(arr,n);
    while (m--)
    {
        int x,y;
        cin>>x;
        string s;
        cin>>s;
         if(s=="and"){
            cin>>y;
            cin>>s>>s;
            int pos1=find(arr,n,x);
            int pos2=find(arr,n,y);
            if((pos1-1)/2==(pos2-1)/2){
                cout<<"T"<<endl;
            }
            else{
                cout<<"F"<<endl;
            }
        }
        else{
            cin>>s;
            if(s=="a"){
                cin>>s>>s;
                cin>>y;
                int pos=find(arr,n,x);
                if(y==arr[(pos-1)/2]){
                    cout<<"T"<<endl;
                }
                else{
                    cout<<"F"<<endl;
                }
            }
            else{
                cin>>s;
                if(s=="root"){
                    if(x==arr[0]){
                        cout<<"T"<<endl;
                    }
                    else{
                        cout<<"F"<<endl;
                    }

                }
                else{
                    int pos=find(arr,n,x);
                    cin>>s;
                    cin>>y;
                    if(y==arr[2*pos+1]){
                        cout<<"T"<<endl;
                    }
                    else{
                        cout<<"F"<<endl;
                    }
                }
            }
        }
    }
    return 0;
}