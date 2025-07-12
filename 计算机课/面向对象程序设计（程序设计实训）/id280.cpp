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
class JourneyCard{
protected:
	int Journeyid;
	int Journeyscore; 
public:
	JourneyCard(int idval):Journeyid(idval){
		Journeyscore=0;
	}
};
class CreditCard{
protected:
	int id;
	string name;
	int limit;
	float cost;
	int Creditscore;
public:
	CreditCard(int idval,string nval,int lival):id(idval),name(nval),limit(lival){
		cost=0;
		Creditscore=0;
	}
	void Crpays(double m){
		if(cost+m>limit){

		}
		else{
			cost+=m;
			Creditscore+=m;
		}
	}
	void Crbackpay(double m){
			cost-=m;
			Creditscore-=m;
	}
};
class JourneyCreditCard :public JourneyCard,public CreditCard{
public:
	JourneyCreditCard(int idval,int Joidval,string nval,int lival):CreditCard(idval,nval,lival),JourneyCard(Joidval){
		cost=0;
		Creditscore=0;
		Journeyscore=0;
	}
	void pays(double m){
		if(cost+m<=limit){
			cost+=m;
			Creditscore+=m;
			Journeyscore+=m;
		}	
	}
	void backpay(double m){
		cost-=m;
		Creditscore-=m;
		Journeyscore-=m;
	}
	void change(int m){
		Journeyscore+=(m/2);
		Creditscore-=m;
	}
	void print(){
		cout<<Journeyid<<" "<<Journeyscore<<endl;
		cout<<id<<" "<<name<<" "<<fixed<<setprecision(1)<<cost<<" "<<Creditscore<<endl;
	}
};
int main()
{
	#ifndef ONLINE_JUDGE
    freopen("in.in", "r", stdin);
    freopen("out.out", "w", stdout);
    #endif
	int id,Creditid,limit;
	string name;
	int t;
	cin>>id>>Creditid>>name>>limit>>t;
	JourneyCreditCard jc(Creditid,id,name,limit);
	while (t--)
	{
		string type;
		cin>>type;
		if(type=="o"){
			double m;
			cin>>m;
			jc.pays(m);
		}
		else if(type=="c"){
			double m;
			cin>>m;
			jc.Crpays(m);
		}
		else if(type=="q"){
			double m;
			cin>>m;
			jc.Crbackpay(m);
		}
		else if(type=="t"){
			int m;
			cin>>m;
			jc.change(m);
		}
	}
	jc.print();
	
}

