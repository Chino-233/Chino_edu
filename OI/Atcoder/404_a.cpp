#include <bits/stdc++.h>
using namespace std;

// const int N = 1e6 + 10;
const int N = 10;
#define ll long long

int main(){
    #ifndef ONLINE_JUDGE
    freopen("in.in", "r", stdin);
    freopen("out.out", "w", stdout);
    #endif
    string s;
    cin>>s;
    int arr[100]={0};
    for(int i=0;i<s.length();i++){
        arr[s[i]-'a']=1;
    }
    for(int i=0;i<26;i++){
        if(!arr[i]){
            cout<<(char)('a'+i)<<endl;
            break;
        }
    }
    return 0;
}