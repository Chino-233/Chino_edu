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
void Shellsort(int arr[], int n,int gap){
    int con = 0;
    for(int i=0;i<gap;i++){
    for(int endindex = gap+i;endindex<n;endindex+=gap)
    {
        int temp=arr[endindex];
        //cout<<"temp: "<<temp<<endl;
        int con=endindex-gap;
        for(;con>=0;con-=gap){
            if(temp>arr[con]){
                arr[con+gap]=arr[con];
            }
            else{
                break;
            }
        }
        arr[con+gap]=temp;
        
        //cout<<endl;
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
int main(){
    #ifndef ONLINE_JUDGE
	freopen("in.in", "r", stdin);
	freopen("out.out", "w", stdout);
    #endif
    int t;
    cin>>t;
    while(t--){
        int n;
        int arr[100];
        cin>>n;
        for(int i=0;i<n;i++){
            cin>>arr[i];
        }
        for(int gap=n/2;gap>0;gap/=2){
            Shellsort(arr,n,gap);
        }
        cout<<endl;
    }
    return 0;
}