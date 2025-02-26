#include <iostream>
using namespace std;
int main(){
    #ifndef ONLINE_JUDGE
	freopen("in.in", "r", stdin);
	freopen("out.out", "w", stdout);
    #endif
    int t;
    cin>>t;
    while(t--){
        char type;
        cin>>type;
        int n;
        cin>>n;
        char a[n][n+1];
        for(int i=0;i<n;i++){
            for(int j=0;j<n+1;j++){
                a[i][j]='\0';
            }
        }
        getchar();
        for(int i=0;i<n;i++){
            int j=0;
            while (1)
            {
                char temp=getchar();
                if(temp==' '||temp=='\n'){
                    a[i][j]='\0';
                    break;
                }
                else{
                    a[i][j]=temp;
                    j++;
                }
            }
            
        }
        int edge[n][n];
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                edge[i][j]=0;
            }
        }
        int k;
        cin>>k;
        if(type=='D'){
            for(int i=0;i<k;i++){
            char s1[100]={0};
            char s2[100]={0};
            cin>>s1>>s2;
            for(int j=0;j<n;j++){
                int flag1=0,flag2=0;
                for(int l=0;l<n;l++){
                    if(s1[l]==a[j][l]){
                        flag1=1;
                    }
                    else{
                        flag1=0;
                        break;
                    }
                }
                if(flag1){
                    for(int l=0;l<n;l++){
                        for(int m=0;m<n;m++){
                            if(s2[m]==a[l][m]){
                                flag2=1;
                            }
                            else{
                                flag2=0;
                                break;
                            }
                        }
                        if(flag2){
                            edge[j][l]=1;
                            //edge[l][j]=1;
                            }
                        }
                    }
                }
            }
            int count=0;
            int degree[n][2];//0:outdegree 1:indegree
            for(int i=0;i<n;i++){
                for(int j=0;j<2;j++){
                    degree[i][j]=0;
                }
            }
            for(int i=0;i<n;i++){
                for(int j=0;j<n;j++){
                    if(edge[i][j]==1){
                        degree[i][0]++;
                    }
                    cout<<edge[i][j];
                    if(j==n-1){
                        cout<<endl;
                    }
                    else{
                        cout<<" ";
                    }
                }
            }
            for(int i=0;i<n;i++){
                for(int j=0;j<n;j++){
                    if(edge[j][i]==1){
                        degree[i][1]++;
                    }
                }
            }
            for(int i=0;i<n;i++){
                if(degree[i][0]+degree[i][1]==0){
                    cout<<a[i]<<endl;
                }
                else{
                cout<<a[i]<<": "<<degree[i][0]<<" "<<degree[i][1]<<" "<<degree[i][0]+degree[i][1]<<endl;
                }
            }
        }
        else if(type=='U'){
            for(int i=0;i<k;i++){
            char s1[100]={0};
            char s2[100]={0};
            cin>>s1>>s2;
            for(int j=0;j<n;j++){
                int flag1=0,flag2=0;
                for(int l=0;l<n;l++){
                    if(s1[l]==a[j][l]){
                        flag1=1;
                    }
                    else{
                        flag1=0;
                        break;
                    }
                }
                if(flag1){
                    for(int l=0;l<n;l++){
                        for(int m=0;m<n;m++){
                            if(s2[m]==a[l][m]){
                                flag2=1;
                            }
                            else{
                                flag2=0;
                                break;
                            }
                        }
                        if(flag2){
                            edge[j][l]=1;
                            edge[l][j]=1;
                            }
                        }
                    }
                }
            }
            int count=0;
            int degree[n][2];//0:outdegree 1:indegree
            for(int i=0;i<n;i++){
                for(int j=0;j<2;j++){
                    degree[i][j]=0;
                }
            }
            for(int i=0;i<n;i++){
                for(int j=0;j<n;j++){
                    if(edge[i][j]==1){
                        degree[i][0]++;
                    }
                    cout<<edge[i][j];
                    if(j==n-1){
                        cout<<endl;
                    }
                    else{
                        cout<<" ";
                    }
                }
            }
            for(int i=0;i<n;i++){
                for(int j=0;j<n;j++){
                    if(edge[j][i]==1){
                        degree[i][1]++;
                    }
                }
            }
            for(int i=0;i<n;i++){
                if(degree[i][0]+degree[i][1]==0){
                    cout<<a[i]<<endl;
                }
                else{
                    cout<<a[i]<<": "<<degree[i][0]<<endl;
                }
            }
        }
    }
    return 0;
}