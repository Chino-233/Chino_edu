#include <bits/stdc++.h>
using namespace std;
const int N = 1e6 + 10;
//const int N = 10;
#define ll long long
int main(){
    #ifndef ONLINE_JUDGE
    freopen("in.in", "r", stdin);
    freopen("out.out", "w", stdout);
    #endif
    int t;
    cin>>t;
    while (t--)
    {
        int n;
        cin>>n;
        string s;
        cin>>s;
        long all=0;
        for(int i=0;i<s.length();i++){
            if(s[i]=='1'){
                all+=n-1;
            }
            else{
                all+=1;
            }
        }
        cout<<all<<endl;
    }
    
    return 0;
}