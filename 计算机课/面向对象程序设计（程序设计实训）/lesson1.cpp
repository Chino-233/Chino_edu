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
template<class T>
class CNode
{
public:
	T data;
	CNode<T> *next;
	CNode(T dval,CNode *p=nullptr):data(dval),next(p){};
	CNode()=default;
	CNode<T> *getnext(){
		return next;
	}
	T getdata(){
		return data;
	}
	void setnext(CNode<T> *p){
		next=p;
	}
	void setdata(T a){
		data=a;
	}
	//void CNode(){};
};
template<class T>
class CList
{
private:
	CNode<T> *head;
public:
	CList(int N){
		T temp;
		head=new CNode<T>;
		cin>>temp;
		head->setdata(temp);
		CNode<T>* tail=head;
		for(int i=0;i<N-1;i++){
			cin>>temp;
			CNode<T> *s=new CNode<T>(temp);
			tail->setnext(s);
			tail=s;
		}
	};
	CList(){
		head=new CNode<T>;
	};
	void append(T a){
		CNode<T>* pos=head;
		while (pos->getnext()!=nullptr)
		{
			pos=pos->getnext();
		}
		CNode<T> *temp=new CNode<T>(a,pos->getnext());
		pos->setnext(temp);
	}; // 加到链表最后
	int insert(T a, int n){
		CNode<T>* pos=find(n);
		if(pos){
			CNode<T> *temp=new CNode<T>(a,pos->getnext());
			pos->setnext(temp);
			if(n==0){
				head=temp;
			}
			return 1;
		}
		return 0;
	}; 
	int remove(int n){
		CNode<T>* pos=find(n-1);
		if(pos&&pos->getnext()){
			CNode<T> *temp=pos->getnext();
			pos->setnext(temp->getnext());
			if(n==1){
				head=temp->getnext();
			}
			delete temp;
			return 1;
		}
		return 0;
	};
	CNode<T>* find(int n){
		if(n==0){
			CNode<T>* temp=new CNode<T>;
			temp->setnext(head);
			return temp;
		}
		if(n<0){
			return nullptr;
		}
		CNode<T>* pos=head;
		int k=0;
		while (pos&&k<n-1)
		{
			k++;
			pos=pos->getnext();
		}
		return pos;
	}; 
	void set(T a, int n){
		CNode<T>* pos=find(n);
		if(pos){
			pos->setdata(a);
		}
	}; 
	void print(){
		CNode<T>* pos=head;
		while (pos!=nullptr)
		{
			 cout << pos->data << (pos->next ? " " : "");
			pos=pos->getnext();
		}
		cout<<endl;
	};
	~CList(){
		CNode<T> *p=head;
		while (p)
		{
			CNode<T> *temp=p;
			p=p->getnext();
			delete temp;
		}
		head=nullptr;
	};
};
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
		string type;
		cin>>type;
		if(type=="I"){
			int N;
			cin>>N;
			CList<int> cl(N);
			int pos,num;
			cin>>pos>>num;
			cl.insert(num,pos);
			cin>>pos;
			if(cl.find(pos)){
				cout<<cl.find(pos)->getdata()<<endl;
			}
			else{
				cout<<"error"<<endl;
			}
			cin>>pos;
			cl.remove(pos);
			cin>>pos>>num;
			cl.set(num,pos);
			cl.print();
		}
		if(type=="D"){
			int N;
			cin>>N;
			CList<double> cl(N);
			int pos;
			double num;
			cin>>pos>>num;
			cl.insert(num,pos);
			cin>>pos;
			if(cl.find(pos)){
				cout<<cl.find(pos)->getdata()<<endl;
			}
			else{
				cout<<"error"<<endl;
			}
			cin>>pos;
			cl.remove(pos);
			cin>>pos>>num;
			cl.set(num,pos);
			cl.print();
		}
		if(type=="S"){
			int N;
			cin>>N;
			CList<string> cl(N);
			int pos;
			string num;
			cin>>pos>>num;
			cl.insert(num,pos);
			cin>>pos;
			if(cl.find(pos)){
				cout<<cl.find(pos)->getdata()<<endl;
			}
			else{
				cout<<"error"<<endl;
			}
			cin>>pos;
			cl.remove(pos);
			cin>>pos>>num;
			cl.set(num,pos);
			cl.print();
		}
		
	}
	
}
