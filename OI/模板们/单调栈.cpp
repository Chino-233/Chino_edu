#include<iostream>
#include<string>
#include<algorithm>
#include<vector>
#include<stack>
#include<queue>
#include<cstring>
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
using namespace std;
struct round1
{
    long long d;
    long long c;
};
int main(){
    #ifndef ONLINE_JUDGE
	freopen("in.in","r",stdin);
	freopen("out.out","w",stdout);
	#endif
    int arr[100][100];
    //arr[i][j]=min(arr[i][j-1],arr[i+pow(2,j-1)][j-1]);
    int N,Q;
    cin>>N>>Q;
    round1 fo[100];
    for(int i=1;i<=N;i++){
        cin>>fo[i].d>>fo[i].c;
    }
    int arr1[100];
    int arr2[100];
    q.push(fo[1].d);
	int pos=0;
	arr2[0]=1;
	for(int i=2;i<=N;i++){
	for(int l=0;l<=N;l++){
	if(arr[i]>q.top()&&!q.empty()){
			q.pop();
			arr1[arr2[pos]]=i;
			pos--;
			if(pos==-1){
			pos++;
			arr2[pos]=i;
			q.push(fo[i].d);
			break;
			}
	}	
	else{
		pos++;
		arr2[pos]=i;
		q.push(fo[i].d);
		break;
	}
	}
	}
    long long f[1000][20];
    for(int i=0;i<N;i++){
        int x;
        for(int j=0;j<pow(2,j);j++){
            
        }
    }
}
