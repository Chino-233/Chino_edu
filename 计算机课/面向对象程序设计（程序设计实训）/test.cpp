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
const int hmax=26;
class Info { //联系人，用一个链表结点表示
private:
	string name; //姓名
	int phoneNo; //电话
public:
	Info* next;  //指向下一个结点
	Info(string tn= "no name", int pno=0)
	{
		name = tn;
		phoneNo = pno;
		next = NULL;
	}
	void Print()
	{cout<<name<<"--"<<phoneNo<<endl;}
	Info* getnext(){
		return next;
	}
	string getname(){
		return name;
	}
	int getnumber(){
		return phoneNo;
	}
	void setnumber(int number){
		phoneNo=number;
	}
	//属性的get和set方法....自行定义
};
class PhoneBook {//组链表方式实现通讯录
//....自行增加一些操作
//提示：把插入和查找先写成内部函数，再被运算符重载调用，会更方便
public:
	Info Table[hmax];//链表头结点数组，对应26个大写字母
	int insert(string nval,int phonenum){
		int num=nval[0]-'A';
		if(num<0||num>26){
			return 0;
		}
		if(find(nval)==1){
			findchange(nval,phonenum);
		}
		else{
			Info *p=new Info(nval,phonenum);
			p->next=Table[num].next;
			Table[num].next=p;
		}
		return 1;
	}
	int find(string nval){
		int num=nval[0]-'A';
		Info *p=&Table[num];
		while (p!=nullptr)
		{
			if(p->getname()==nval){
				//p->Print();
				return 1;
			}
			p=p->getnext();
		}
		return 0;
	}
	Info* findpos(string nval){
		int num=nval[0]-'A';
		Info *p=&Table[num];
		while (p!=nullptr)
		{
			if(p->getname()==nval){
				//p->Print();
				return p;
			}
			p=p->getnext();
		}
		return NULL;
	}
	int findchange(string nval,int phonenum){
		int num=nval[0]-'A';
		Info *p=&Table[num];
		while (p!=nullptr)
		{
			if(p->getname()==nval){
				p->setnumber(phonenum);
				return 1;
			}
			p=p->getnext();
		}
		return 0;
	}
	void Input(){
		int n;
		cin>>n;
		string nval;
		int phonenum;
		for(int i=0;i<n;i++){
			cin>>nval>>phonenum;
			insert(nval,phonenum);
		}
		
	}
	Info* operator()(string nval){
		return findpos(nval);
	} 
	friend PhoneBook operator+(PhoneBook &lhs,PhoneBook &rhs);
	void operator+=(Info p){
		if(p.getname()=="no name"){
			return;
		}
		if(find(p.getname())){
			findchange(p.getname(),p.getnumber());
		}
		else{
			insert(p.getname(),p.getnumber());
		}
	}
	void Print(){
		for(int i=0;i<26;i++){
		Info *p=&Table[i];
		int flag=0;
		while (p!=nullptr)
		{
			if(p->getname()=="no name"){
				p=p->getnext();;
				continue;
			}
			if(flag==0){
				char a='A';
				a+=i;
				cout<<a<<"--";
				flag=1;
			}
			cout<<p->getname()<<"."<<p->getnumber()<<"--";
			p=p->getnext();
		}
		if(flag==1){
			cout<<endl;
		}
		}	
	}
//以下定义五个操作：输入Input、打印Print、插入、合并、查找
//具体操作看前面说明
};
PhoneBook operator+(PhoneBook &lhs,PhoneBook &rhs){
	for(int i=0;i<26;i++){
		Info *p=&rhs.Table[i];
		while (p!=nullptr)
		{
			if(p->getname()=="no name"){
				p=p->getnext();
				continue;
			}
			if(lhs.find(p->getname())){
				lhs.findchange(p->getname(),p->getnumber());
			}
			else{
				lhs.insert(p->getname(),p->getnumber());
			}
			p=p->getnext();
		}
	}	
	return lhs;
}
//...PhoneBook类成员函数，类外实现，自行编写
//----主函数----
int main()
{	
	#ifndef ONLINE_JUDGE
	freopen("in.in", "r", stdin);
	freopen("out.out", "w", stdout);
	#endif	
	string tname;
	int i, tno;
	Info *p;
	PhoneBook pb;
	pb.Input(); //接收输入数据，初始化第一个通讯录
	//两次姓名查找
	for (i=0; i<2; i++)
	{
		cin>>tname;
		p = pb(tname); //调用()运算符，实现查找
		if (p)p->Print(); //查找成功，输出联系人信息
		else cout<<"查找失败"<<endl; //查找失败，输出提示信息
	}
	//一次插入
	cin>>tname>>tno;
	Info temp(tname, tno);
	pb += temp; //调用+=运算符，实现插入新联系人
	//通讯录合并
	PhoneBook pc;
	pc.Input(); //初始化第二个通讯录
	pb = pb+pc; //调用+运算符，实现合并
	pb.Print(); //输出所有操作后的通讯录
	return 0;
	}
/*******************************************/

