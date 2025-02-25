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
int find(vector<int> &a,int find,int key){
    for(int i=a.size()-1;a[i]!=key;i--){
        if(a[i]==find){
            return i;
        }
        else if(a[i]==key){
            //cout<<"error"<<endl;
            return 0;
        }
    }
    return 0;
}
int main(){
    #ifndef ONLINE_JUDGE
	freopen("in.in", "r", stdin);
	freopen("out.out", "w", stdout);
    #endif
    int n;
    cin>>n;
    int arr[100];
    for(int i=0;i<n;i++){
        cin>>arr[i];
    }
    int t;
    cin>>t;
    while(t--){
        int findnum;
        cin>>findnum;
        int high=n;
        int low=0;
        int mid;
        while(low<high){
            mid=(low+high)/2;
            if(arr[mid]==findnum){
                cout<<mid+1<<endl;
                break;
            }
            else if(arr[mid]>findnum){
                high=mid;
            }
            else{
                low=mid+1;
            }
            if(low>=high){
                cout<<"error"<<endl;
            }
        }
    }
    return 0;
}