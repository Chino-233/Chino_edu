#include <bits/stdc++.h>
using namespace std;
//  const int N = 1e6 + 10;
  const int N = 10;
#define ll long long

int main(){
    #ifndef ONLINE_JUDGE
	freopen("in.in", "r", stdin);
	freopen("out.out", "w", stdout);
    #endif
    string s;
    cin>>s;
    char s1[102];
    int flag=0;
    for(int i=0;i<(s.length()/2);i++){
        int temp=min(s[i],s[s.length()-i]);
        s1[i]=temp;
        s[s.length()-i]=temp;
        if(s[i]!=s[s.length()-i]){
            flag=1;
        }
    }
    if(!flag){
        if(s.length()%2==1){
            s1[(s.length()/2)+1]=s[(s.length()/2)+1]+1;
        }
    }
    s1[s.length()]=0;

    cout<<s1<<endl;
    return 0;
}