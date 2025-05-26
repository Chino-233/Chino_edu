#include <bits/stdc++.h>
using namespace std;

// const int N = 1e6 + 10;
const int N = 10;
#define ll long long
struct point{
    int x;
    int y;
    int num;
};
bool cmp(point a,point b){
    return a.num<b.num;
}
int main(){
    #ifndef ONLINE_JUDGE
    freopen("in.in", "r", stdin);
    freopen("out.out", "w", stdout);
    #endif
    int r,c;
    cin>>r>>c;
    vector<point> v;
    int dp[110][110];
    map<int,int> mp;
    map<int,point> mp1;
    for(int i=0;i<1000;i++){
        mp1[i]={-1,-1};
    }
    int high[110][110];
    for(int i=0;i<110;i++){
        for(int j=0;j<110;j++){
            high[i][j]=0;
            dp[i][j]=0;
        }
    }
    for(int i=1;i<=r;i++){
        for(int j=1;j<=c;j++){
            cin>>high[i][j];
            v.push_back({i,j,high[i][j]});
            dp[i][j]=1;
        }
    }
    sort(v.begin(),v.end(),cmp);
    for(auto &p:v){
        int x=p.x;
        int y=p.y;
            if(high[x-1][y]<high[x][y]){
                dp[x][y]=max(dp[x-1][y]+1,dp[x][y]);
            }
            if(high[x+1][y]<high[x][y]){
                dp[x][y]=max(dp[x+1][y]+1,dp[x][y]);
            }
            if(high[x][y-1]<high[x][y]){
                dp[x][y]=max(dp[x][y-1]+1,dp[x][y]);
            }
            if(high[x][y+1]<high[x][y]){
                dp[x][y]=max(dp[x][y+1]+1,dp[x][y]);
            }
    }
    int num=-1;
    for(int i=0;i<110;i++){
        for(int j=0;j<110;j++){
            if(dp[i][j]>num){
                num=dp[i][j];
            }
        }
    }
    cout<<num;
    return 0;
}