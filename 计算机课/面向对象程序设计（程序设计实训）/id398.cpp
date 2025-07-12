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
class CPeople{
protected:
	string id;
	string name;
public:

};
class CInternetUser:virtual public CPeople{
protected:
	string password;
public:
	void registerUser(string nval,string idval,string paval){
		name=nval;
		id=idval;
		password=paval;
	}
	bool login(string idval,string paval){
		if(idval==id&&paval==password){
			return 1;
		}
		return 0;
	}
};
class CBankCustomer:virtual public CPeople{
protected:
	double balance;
public:
	void openAccount(string nval,string idval){
		name=nval;
		id=idval;
	}
	bool deposit(double m){
		balance+=m;
		return 1;
	}
	bool withdraw(double m){
		if(balance>=m){
			balance-=m;
			return 1;
		}
		else{
			return 0;
		}
	}
};
class CInternetBankCustomer:public CBankCustomer,public CInternetUser{
private:
	double Internetbalance;
	double yestodaybalance;
	double todayInterest;
	double wangtodayInterest;
	double yestodayInterest;
public:
	CInternetBankCustomer(){
		Internetbalance=0;
		yestodaybalance=0;
		todayInterest=0;
	}
	void setInterest(double m){
		wangtodayInterest=m;
	}
	void calculateProfit(){
		todayInterest=(yestodaybalance/10000)*yestodayInterest;
		Internetbalance+=todayInterest;
		yestodaybalance=Internetbalance;
		yestodayInterest=wangtodayInterest;
	}
	bool deposit(double m){
		if(balance>=m){
			balance-=m;
			Internetbalance+=m;
			return 1;
		}
		return 0;
	}
	bool withdraw(double m){
		if(Internetbalance>=m){
			Internetbalance-=m;
			balance+=m;
			return 1;
		}
		else{
			return 0;
		}
	}
	void print(){
	cout<<"Name: "<<name<<" ID: "<<id<<endl;
	cout<<"Bank balance: "<<balance<<endl;
	cout<<"Internet bank balance: "<<Internetbalance<<endl<<endl;
	}
};
/*******************************************/
int main()
{
	#ifndef ONLINE_JUDGE
    freopen("in.in", "r", stdin);
    freopen("out.out", "w", stdout);
    #endif
    int t, no_of_days, i;
    string i_xm, i_id, i_mm, b_xm, b_id, ib_id, ib_mm;
    double money, interest;
    char op_code;

    // 输入测试案例数t
    cin >> t;
    while (t--)
    {
        // 输入互联网用户注册时的用户名,id,登陆密码
        cin >> i_xm >> i_id >> i_mm;

        // 输入银行开户用户名,id
        cin >> b_xm >> b_id;

        // 输入互联网用户登陆时的id,登陆密码
        cin >> ib_id >> ib_mm;

        CInternetBankCustomer ib_user;

        ib_user.registerUser(i_xm, i_id, i_mm);
        ib_user.openAccount(b_xm, b_id);

        if (ib_user.login(ib_id, ib_mm) == 0) // 互联网用户登陆,若id与密码不符;以及银行开户姓名和id与互联网开户姓名和id不同
        {
            cout << "Password or ID incorrect" << endl;
            continue;
        }

        // 输入天数
        cin >> no_of_days;
        for (i = 0; i < no_of_days; i++)
        {
            // 输入操作代码, 金额, 当日万元收益
            cin >> op_code >> money >> interest;
            switch (op_code)
            {
            case 'S': // 从银行向互联网金融帐户存入
            case 's':
                if (ib_user.deposit(money) == 0)
                {
                    cout << "Bank balance not enough" << endl;
                    continue;
                }
                break;
            case 'T': // 从互联网金融转入银行帐户
            case 't':
                if (ib_user.withdraw(money) == 0)
                {
                    cout << "Internet bank balance not enough" << endl;
                    continue;
                }
                break;
            case 'D': // 直接向银行帐户存款
            case 'd':
                ib_user.CBankCustomer::deposit(money);
                break;
            case 'W': // 直接从银行帐户取款
            case 'w':
                if (ib_user.CBankCustomer::withdraw(money) == 0)
                {
                    cout << "Bank balance not enough" << endl;
                    continue;
                }
                break;
            default:
                cout << "Illegal input" << endl;
                continue;
            }
            ib_user.setInterest(interest);
            ib_user.calculateProfit();
            // 输出用户名,id
            // 输出银行余额
            // 输出互联网金融账户余额
            ib_user.print();
        }
    }
}


