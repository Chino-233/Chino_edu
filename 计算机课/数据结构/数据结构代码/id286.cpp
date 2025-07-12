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
    while(n--){
        double findnum;
        cin>>findnum;
        int findtwice=0;
        double high=findnum;
        double low=0;
        double mid;
        while(low-high<=1e-6){
            findtwice++;
            mid=(low+high)/2;
            if(abs(pow(mid,2)-findnum)<=1e-5){
                cout<<findtwice<<" "<<fixed<<setprecision(3)<<mid<<endl;
                break;
            }
            else if(pow(mid,2)-findnum<=-1e-6){
                low=mid;
            }
            else{  
                high=mid;
            }
        }
    }
    return 0;
}