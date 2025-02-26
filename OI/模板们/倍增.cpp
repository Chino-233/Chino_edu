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
	/*
	前言：倍增的思想便是2^j=2^(j-1)+2^(j-1)；
	数组当中，
	第一个为当前的数字
	第二为加上当前数字，一共2^j个数字时对应的位置；如果我们要扫描下一部分，
	（也就是算（j+1)时）就需要先加上1，到达下一部分再进行倍增；
	但是这样是有风险的，因为我们从1开始的时候，0会被算进去；
	因此特判0来避免这种情况；
	在这里，用g来描述前进的步数；
	用f来描述前进的步数所对应的具体数字；
	f和g一样的方法，但是具体数字交由题目，比如说此处f1是最小，f2是最大；
	*/
	int m,n;
	cin>>m>>n;
	int arr[100]={0};
	int f1[100][20]={0};
	int f2[100][20]={0};
	int g[100][20];//命名
	for(int i=1;i<=m;i++){
		cin>>arr[i];
	}//输入原始数据
	for(int i=1;i<=m;i++){
		f1[i][0]=arr[i];
		f2[i][0]=arr[i];
	}//由于j=0时只包括自己，因此把相应的值赋给0；
	for(int i=1;i<=m;i++){
		g[i][0]=i;
	}//g的0对应的是自己，也就是当前的位置i
	for(int j=1;j<16;j++){
		for(int i=1;i<m;i++){
			if(g[i][j-1]!=0){//特判0
				g[i][j]=g[g[i][j-1]+1][j-1];//g算法
				f1[i][j]=min(f1[g[i][j-1]+1][j-1],f1[i][j-1]);//f1算法
			}
		}
	}
	for(int j=1;j<16;j++){
		for(int i=1;i<=m;i++){
			if(g[i][j-1]!=0&&g[i][j-1]<=m-1){
				g[i][j]=g[g[i][j-1]+1][j-1];
				f2[i][j]=max(f2[g[i][j-1]+1][j-1],f2[i][j-1]);
			}
		}
	}
	for(int i=0;i<100;i++){
		for(int j=0;j<20;j++){
			if(g[i][j]==0){
				g[i][j]=100000;
			}
		}
	}//将0全部都打上大数字，这样就不会干扰到下面的步数判断
	//（因为i=0在这里存在且g[i][]=0,会被导入)
	for(int i=1;i<=n;i++){
		int low,high;
		int mi=21000000;
		int mx=-1;
		cin>>low>>high;//输入区间
		int low1=low;
		int high1=high;
		for(int j=16;j>=0;j--){
			if(g[low][j]<=high&&g[low][j]>0){//判断j在这个值的时候有没有超界
				if(mi>f1[low][j]){
					mi=f1[low][j];
				}//小区间最小值的最小值就是大区间的最小值
				low=g[low][j]+1;//和前面一样，加一到达下一个需要判断的小区间内；
			}
		}
		for(int j=16;j>=0;j--){
			if(g[low1][j]<=high1&&g[low1][j]>0){
				if(mx<f2[low1][j]){
					mx=f2[low1][j];
				}
				low1=g[low1][j]+1;
			}
		}
		cout<<mx-mi<<endl;//输出
	}

}