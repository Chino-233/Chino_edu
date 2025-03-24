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
int main(){
    #ifndef ONLINE_JUDGE
	freopen("in.in", "r", stdin);
	freopen("out.out", "w", stdout);
    #endif
    string s;
    cin>>s;
    //int num=0;
    int dp[10][10]={0};
    int n=s.length();
    for(int i=0;i<n-1;i++){
        if(s[i]>s[i+1]){
            dp[i][i+1]=1;
        }
        else{
            dp[i][i+1]=0;
        }
    }
    if(n<4){
        for(int l=2;l<n;l++){
            for(int i=0;i<n-l;i++){
                dp[i][i+l]=dp[i][i+l-1]+dp[i+1][i+l]-dp[i+1][i+l-1]+(s[i]>s[i+l]);
            }
        }
    }
    else{
        for(int l=2;l<3;l++){
            for(int i=0;i<n-l;i++){
                dp[i][i+l]=dp[i][i+l-1]+dp[i+1][i+l]-dp[i+1][i+l-1]+(s[i]>s[i+l]);
            }
        }
        for(int l=3;l<4;l++){
            for(int i=0;i<n-l;i++){
                dp[i][i+l]=dp[i][i+l-1]+dp[i+1][i+l]-dp[i+1][i+l-1];
                if(s[i]>s[i+l]){
                    dp[i][i+l]++;
                }
                else if(s[i]==s[i+l]){
                    dp[i][i+l]+=dp[i+1][i+l-1];
                }
            }
        }
        for(int l=4;l<n;l++){
            for(int i=0;i<n-l;i++){
                dp[i][i+l]=dp[i][i+l-1]+dp[i+1][i+l]-dp[i+1][i+l-1];
                if(s[i]>s[i+l]){
                    dp[i][i+l]++;
                }
                else if(s[i]==s[i+l]){
                    dp[i][i+l]+=(dp[i+1][i+l-1]+dp[i+2][i+l-2]-dp[i+2][i+l-1]-dp[i+1][i+l-2]);
                }
            }
        }
    }
    cout<<dp[0][n-1];
    //cout<<num<<endl;
    return 0;
}