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
    string s[8]={"Bessie","Buttercup","Belinda","Beatrice","Bella","Blue","Betsy","Sue"};
    sort(s,s+8);
    map<string,int> mp;
    vector<int> arr;
    int j=0;
    for(auto i:s){
        //cout<<i<<endl;
        mp[i]=j;
        arr.push_back(j);
        j++;
    }
    /*3
Buttercup must be milked beside Bella
Blue must be milked beside Bella
Sue must be milked beside Beatrice*/
    int n;
    cin>>n;
    int edge[10][10];
    int edge1[10][10];
    for(int i=0;i<n;i++){
        int m,k;
        string temp;
        cin>>temp;
        m=mp[temp];
        cin>>temp;
        cin>>temp;
        cin>>temp;
        cin>>temp;
        cin>>temp;
        k=mp[temp];
        edge[i][0]=m;
        edge[i][1]=k;
        edge1[m][k]=1;
        edge1[k][m]=1;
    }
    for(int i=0;i<n;i++){
        int temp=edge[i][1];
        int pos1=0;
        for(int j=0;j<n;j++){
            if(arr[j]==temp){
                pos1=j;
            }
        }
        int pos2=0;
        int temp=edge[i][0];
        for(int j=0;j<n;j++){
            if(arr[j]==temp){
                pos2=j;
            }
        }
        if(edge[i][0]<edge[i][1]){
            if(pos1>0){
                if(edge[arr[pos1]][arr[pos1-1]]==1){
                    if(arr[pos1-1]>edge[i][0]){
                        
                    }
                }
            }
            else{

            }
        }
    }
    return 0;
}