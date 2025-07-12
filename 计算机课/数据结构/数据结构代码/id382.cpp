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
void print(int arr[], int n){
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
int main(){
    #ifndef ONLINE_JUDGE
	freopen("in.in", "r", stdin);
	freopen("out.out", "w", stdout);
    #endif
    int t;
    cin >> t;
    while(t--){
        int n;  
        cin >> n;
        int arr[100]={0};
        for(int i = 0; i < n; i++){
            cin >> arr[i];
        }
        //print(arr, n);
        for(int i=0;i<n-1;i++){
            int mi=arr[i];
            int mipos=i;
            for(int j=i+1;j<n;j++){
                if(arr[j]<mi){
                    mi=arr[j];
                    mipos=j;
                }
            }
            swap(arr[i],arr[mipos]);
            print(arr, n);  
        }
        cout<<endl;
    }
    return 0;
}