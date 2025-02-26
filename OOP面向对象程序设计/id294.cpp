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
using namespace std;
class CRatge{
private:
	int size;
public:
	void print(){
		for(int i=0;i<size;i++){
			for(int j=0;j<size;j++){
				cout<<"*";
			}
			cout<<endl;
		}
		cout<<endl;
	}
	CRatge(){};
	CRatge(int sval):size(sval){};
	friend CRatge operator ++(CRatge &rhs);
	friend CRatge operator --(CRatge &rhs);
	friend CRatge operator --(CRatge &rhs,int);
	friend CRatge operator ++(CRatge &rhs,int);
};
CRatge operator ++(CRatge &rhs){
	rhs.size++;
	rhs.print();
	return rhs;
}
CRatge operator ++(CRatge &rhs,int){
	rhs.print();
	rhs.size++;
	return rhs;
}CRatge operator --(CRatge &rhs){
	rhs.size--;
	rhs.print();
	return rhs;
}
CRatge operator --(CRatge &rhs,int){
	rhs.print();
	rhs.size--;
	return rhs;
}
int main()
{
	#ifndef ONLINE_JUDGE
    freopen("in.in", "r", stdin);
    freopen("out.out", "w", stdout);
    #endif
	int t;
	cin>>t;
	while (t--)
	{
		int size;
		cin>>size;
		int N;
		cin>>N;
		//int n;
		//cin>>n;
		CRatge cr(size);
		for(int i=0;i<N;i++){
			string type;
			cin>>type;
			if(type=="ADD"){
				++cr;
			}
			else if(type=="add"){
				cr++;
			}
			else if(type=="MINUS"){
				--cr;
			}
			else if(type=="minus"){
				cr--;
			}
		}
		cout<<"----------"<<endl;
	}
}


