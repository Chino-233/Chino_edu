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
void multiply(vector<int> &digits, int m) {
    int carry = 0;
    for (int i = 0; i < digits.size(); i++) {
        int temp = digits[i] * m + carry;
        digits[i] = temp % 10;
        carry = temp / 10;
    }
    while (carry > 0) {
        digits.push_back(carry % 10);
        carry /= 10;
    }
}
int main(){
    #ifndef ONLINE_JUDGE
	freopen("in.in", "r", stdin);
	freopen("out.out", "w", stdout);
    #endif
    string n;
    cin>>n;
    ll k=0;
    cin>>k;
    ll edge[10][10]={0};
    for(ll i=0;i<k;i++){
        ll m,a=114514;
        cin>>m>>a;
        edge[m][a]=1;
    }
    queue<ll>q;
    ll num[10]={0};
    for(ll d=0;d<10;d++){
        while (!q.empty())
        {
            q.pop();
        } 
    q.push(d);
    ll flag[10]={0};
    flag[d]=1;
    while (!q.empty())
    {
        ll a=q.front();
        q.pop();
        for(ll i=0;i<10;i++){
            if(edge[a][i]==1&&flag[i]!=1){
                flag[i]=1;
                q.push(i);
            }
        }
    }
    for(ll i=0;i<10;i++){
        num[d]+=flag[i];
    }
    //num[d]++;
    }
    /*
    for(ll i=0;i<10;i++){
        cout<<i<<":"<<num[i]<<endl;
    }
    */
    vector<int>v={1};
    for(ll i=0;i<n.length();i++){
        int d=num[n[i]-'0'];
        multiply(v,d);
    }
    for (int i = v.size() - 1; i >= 0; i--) {
        cout << v[i];
    }
    return 0;
}