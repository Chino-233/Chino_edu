#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <cstring>
#include <algorithm>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <iomanip>
#include <cmath>
#include <map>
#include <stack>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <bitset>
#include <list>
#include <deque>
#include <forward_list>
#include <array>
#include <tuple>
#include <utility>
#include <memory>
#include <functional>
#include <iterator>
#include <numeric>
using namespace std;
class Treenode
{
private:
	int weight;
	int parent,lchild,rchild;
public:
	Treenode():weight(0),parent(0),lchild(0),rchild(0)
	{
	}
	Treenode(int w,int p,int l,int r):weight(w),parent(p),lchild(l),rchild(r)
	{
	}
	friend class HFTree;
};
class HFTree
{
private:
	Treenode *root;
public:
	HFTree()
	{
	}
	HFTree(Treenode *_root):root(_root)
	{
	}
	void CreateHFTree(int *w,int n)
	{
		CreateHFTree(root,w,n);
	}
	int select(Treenode *&HT,int n)
	{
		int s1=0;
		while(HT[s1].parent!=0)
		{
			s1++;
		}
		//在0--n-1选出两个parent=0且最小的两个结点
		for(int j=0;j<n;j++)
		{		
			if(HT[j].weight<HT[s1].weight&&HT[j].parent==0)
			{
				s1=j;
			}
		}
		HT[s1].parent=1;//暂时赋值
		return s1;
	}
	void CreateHFTree(Treenode *&HT,int *w,int n)
	{
		if(n<=1)
			return;
		int m=2*n-1;
		HT=new Treenode[m];
		for(int i=0;i<n;i++)
		{
			HT[i].weight=w[i];//w权重传递给HT
		}
		for(int i=n;i<m;i++)//选出两个parent=0且最小的两个结点
		{
			int s1=select(HT,i),s2=select(HT,i);
//			cout<<s1<<" "<<s2<<endl;
//			for(int j=0;j=i-1;j++)
//			{
//				while(HT[s1].parent!=0)
//				{
//					s1++;
//				}
//				if(HT[j].weight<=HT[s1].weight&&HT[j].parent==0)
//				{
//					s1=j;
//				}
//			}
//			HT[s1].parent=i;//暂时赋值
//			for(int j=0;j=i-1;j++)
//			{
//				int temp,s2=0;
//				while(HT[s2].parent!=0)
//				{
//					s2++;
//				}
//				if(HT[j].weight<=HT[s2].weight&&HT[j].parent==0)
//				{
//					s2=j;
//				}
//			}
//			HT[s2].parent=i;//暂时赋值
//			Select(HT,i-1,s1,s2);
			HT[s1].parent=i;
			HT[s2].parent=i;
			HT[i].lchild=s1;
			HT[i].rchild=s2;
			HT[i].weight=HT[s1].weight+HT[s2].weight;
		}
	}
	void findcode(int n)
	{
		findcode(root,n);
	}
	void findcode(Treenode * &HT,int n)
	{
		int c,f;
		char *cd=new char[n];//存储编码
		for(int i=0;i<n;i++)
		{
			cd[n-1]='\0';
			int start=n-1;
			for(c=i,f=HT[i].parent;f!=0;c=f,f=HT[f].parent)
			{
				if(HT[f].lchild==c)
					cd[--start]='0';
				else
					cd[--start]='1';
			}
			cout<<HT[i].weight<<"-";
			for(int j=start;j<n-1;j++)
			{
				cout<<cd[j];
			}
			cout<<endl;
		}
	}
};
int main()
{
	#ifndef ONLINE_JUDGE
	freopen("in.in", "r", stdin);
	freopen("out.out", "w", stdout);
    #endif
	int n;
	cin>>n;
	int *w=new int [n];
	for(int i=0;i<n;i++)
		cin>>w[i];
	HFTree tree;
	tree.CreateHFTree(w,n);
	tree.findcode(n);
	return 0;
}