#include<iostream>
using namespace std;
int main(){
    #ifndef ONLINE_JUDGE
    freopen("in.in", "r", stdin);
    freopen("out.out", "w", stdout);
    #endif
    int t;
    cin>>t;
    while(t--){
        int n;
        cin>>n;
        int edge[100][100];
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                cin>>edge[i][j];
            }
        }
        int vis[100]={0};
        for(int k=0;k<n;k++){
            for(int i=0;i<n;i++){
                int cut=0;
                for(int j=0;j<n;j++){
                    if(edge[j][i]==1||vis[i]==1){
                        cut++;
                    }
                }
                if(cut==0){
                    cout<<i<<" ";
                    //cout<<endl;
                    vis[i]=1;
                    for(int j=0;j<n;j++){
                        if(edge[i][j]==1){
                            edge[i][j]=0;
                        }
                    }
                    /*
                    for(int j=0;j<n;j++){
                        for(int k=0;k<n;k++){
                            cout<<edge[j][k]<<" ";
                        }
                        cout<<endl;
                    }
                    */
                    break;
                }
        }
        }
        cout<<endl;
    }
}