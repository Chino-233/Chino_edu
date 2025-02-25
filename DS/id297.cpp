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
    vector<int> a(n+1);
    for(int i=1;i<n+1;i++){
        cin>>a[i];
    }
    int t;
    cin>>t;
    int key=114514;
    a[0]=key;
    while(t--){
        int findnum=0;
        cin>>findnum;
        int pos=find(a,findnum,key);
        if(pos==0){
            cout<<"error"<<endl;
        }
        else{
            cout<<pos<<endl;
        }
    }
    return 0;
}