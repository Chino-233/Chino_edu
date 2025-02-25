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
string numtochar(int n){
    if(n<10){
        return to_string(n);
    }
    else{
        return string(1,'A'+n-10);
    }
}
string changecout(double n,int k){
    ll intPart=(ll)n;
    double floatPart=n-intPart;
    stack<int> intstack;
    if(intPart==0){
        intstack.push(0);
    }
    else{
        while(intPart>0){
            intstack.push(intPart%k);
            intPart/=k;
        }
    }
    queue<int> floatqueue;
    string result;
    while(!intstack.empty()){
        result+=numtochar(intstack.top());
        intstack.pop();
    }
    result+='.';
    int count=0;
    if(floatPart==0){
        floatqueue.push(0);
    }
    else{
        while(floatPart>0){
            floatPart*=k;
            int floatintPart=(int)floatPart;
            floatqueue.push(floatintPart);
            floatPart-=floatintPart;
            count++;
            if(count==3){
                break;
            }
        }
    }
    count=0;
    while(!floatqueue.empty()){
        result+=numtochar(floatqueue.front());
        floatqueue.pop();
        count++;
        if(count==3){
            break;
        }
    }
    while(count<3){
        result+='0';
        count++;
    }
    return result;
}
int main(){
    #ifndef ONLINE_JUDGE
	freopen("in.in", "r", stdin);
	freopen("out.out", "w", stdout);
    #endif
    int t;
    cin>>t;
    while (t--)
    {
        double n;
        int k;
        cin>>n>>k;
        cout<<changecout(n,k)<<endl;
    }
    
    return 0;
}