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
#include<cmath>
using namespace std;
int main(){
    #ifndef ONLINE_JUDGE
	freopen("in.in","r",stdin);
	freopen("out.out","w",stdout);
	#endif
	int n;
	cin>>n;
	int arr[1000];
	int sum[400];
	int f[40][40];
	sum[0]=0;
	memset(f,0x3f,sizeof(f));//这个是最大值的初始化，记住！原因是下面要求最小值，不然只会输出0；
	for(int i=1;i<=n;i++){
		cin>>arr[i];
		sum[i]=sum[i-1]+arr[i];
		f[i][i]=0;
	}
	for(int len=2;len<=n;len++){//记得len要初始化最多为2
		for(int i=1;i<=n+1-len;i++){//肯定得+1，不然得爆i
			int j=len+i-1;
			for(int k=i;k<j;k++){//j固定后逐个遍历里面的k；
				f[i][j]=min(f[i][j],f[i][k]+f[k+1][j]+sum[j]-sum[i-1]);//区间DP的状态转移公式；
			}
		}
	}
	cout<<f[1][n];

}
