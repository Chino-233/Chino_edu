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
//#define int long long
using namespace std;
const int N = 1e5 + 10;
typedef double db;
class CCompplex{
private:
	int real;
	int imag;
	int* data;
	double dou;
public:
	CCompplex(){};
	CCompplex(int r,int i):real(r),imag(i){
		data=new int[11];
		for(int i=0;i<11;i++){
			data[i]=114514;
		}
	};
	friend CCompplex operator+(CCompplex &lhs,CCompplex &rhs);
	friend CCompplex operator-(CCompplex &lhs,CCompplex &rhs);
	friend CCompplex operator*(CCompplex &lhs,CCompplex &rhs);
	friend istream &operator>>(istream &in,CCompplex &rhs);
	friend ostream &operator<<(ostream &output_iterator,CCompplex &rhs);
	int &operator [](int i){
		return data[i];
	}
	operator int(){
		return real;
	}
	void* operator new(size_t d,int a,int b,int c){};
	void operator delete(void*,int a){};
	operator double(){
		return dou;
	}
	void print(){
		cout<<"Real="<<real<<" Image="<<imag<<endl;
	}
};
CCompplex operator+(CCompplex &lhs,CCompplex &rhs){
	return CCompplex(lhs.real+rhs.real,lhs.imag+rhs.imag);
}
CCompplex operator-(CCompplex &lhs,CCompplex &rhs){
	return CCompplex(lhs.real-rhs.real,lhs.imag-rhs.imag);
}
CCompplex operator*(CCompplex &lhs,CCompplex &rhs){
	return CCompplex(lhs.real*rhs.real-lhs.imag*rhs.imag,rhs.real*lhs.imag+lhs.real*rhs.imag);
}
istream &operator>>(istream &in,CCompplex &rhs){
    in>>rhs.real>>rhs.imag;
    return in;
}
ostream &operator<<(ostream &out,CCompplex &rhs){
    out<<rhs.real<<rhs.imag;
    return out;
}
class str{
private:
	char *p;
public:
	str();
	str(char *s);
	friend bool operator>(str &,str &);
	friend bool operator<(str &,str &);
	friend bool operator==(str &,str &);
	void show(){
		cout<<p<<endl;
	}
};
str::str(){};
str::str(char *s){
	p=new char[1145];
	strcpy(p,s);
}
bool operator>(str &lhs,str &rhs){
	if(strcmp(lhs.p,rhs.p)>0){
		return 1;
	}
	else{
		return 0;
	}
}
bool operator<(str &lhs,str &rhs){
	if(strcmp(lhs.p,rhs.p)<0){
		return 1;
	}
	else{
		return 0;
	}
}
bool operator==(str &lhs,str &rhs){
	if(strcmp(lhs.p,rhs.p)==0){
		return 1;
	}
	else{
		return 0;
	}
}
class CClock{
private:
	int hour;
	int min;
	int sec;
public:
	CClock(){};
	CClock(int h,int m,int s):hour(h),min(m),sec(s){};
	void operator--(){
	sec--;
	if(sec<0){
		sec+=60;
		min--;
	}
	if(min<0){
		min+=60;
		hour--;
	}
	if(hour<0){
		hour+=12;
	}
	}
	void operator++(){
	sec++;
	if(sec>=60){
		sec-=60;
		min++;
	}
	if(min>=60){
		min-=60;
		hour++;
	}
	if(hour>=12){
		hour-=12;
	}
	}
	void print(){
	cout<<hour<<":"<<min<<":"<<sec<<endl;
	}
};
class CXGraph{
private:
	int n;
public:
	CXGraph(int nval):n(nval){};
	CXGraph& operator++(){
		if(n+2<=21){
			n+=2;
		}
		return *this;
	}
	CXGraph& operator--(){
		if(n-2>=1){
			n-=2;
		}
		return *this;
	}
	CXGraph operator++(int){
		CXGraph old(n);
		if(n+2<=21){
			n+=2;
		}
		return old;
	}
	CXGraph operator--(int){
		CXGraph old(n);
		if(n-2>=1){
			n-=2;
		}
		return old;
	}
	friend ostream &operator<<(ostream &out,const CXGraph &rhs);
};
ostream &operator<<(ostream &out,const CXGraph &rhs){
		int temp=rhs.n;
		while (temp!=-1)
		{
			for(int i=0;i<(rhs.n-temp)/2;i++){
				out<<" ";
			}
			for(int i=0;i<temp;i++){
				out<<"X";
			}
			out<<endl;
			temp-=2;
		}
		temp+=4;
		while (temp!=rhs.n+2)
		{
			for(int i=0;i<(rhs.n-temp)/2;i++){
				out<<" ";
			}
			for(int i=0;i<temp;i++){
				out<<"X";
			}
			out<<endl;
			temp+=2;
		}
		return out;
}
signed main() {
	#ifndef ONLINE_JUDGE
    freopen("in.in", "r", stdin);
    freopen("out.out", "w", stdout);
    #endif
	int t,n;
	string command;
	cin>>n;
	CXGraph xGraph(n);
	cin>>t;
	CCompplex c(1,2);
	int a=c[0];
	while (t--)
	{
		cin>>command;
		if(command=="show++"){
			cout<<xGraph++<<endl;
		}
		else if(command=="++show"){
			cout<<++xGraph<<endl;
		}
		else if(command=="show--"){
			cout<<xGraph--<<endl;
		}
		else if(command=="--show"){
			cout<<--xGraph<<endl;
		}
		else if(command=="show"){
			cout<<xGraph<<endl;
		}
	}
	return 0;
}
