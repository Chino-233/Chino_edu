//补充CBigInteger类和头文件
/********** Write your code here! **********/
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
const int N = 1e5 + 10;
typedef double db;
class CBigInteger{
private:
	string num;
public:
	CBigInteger(string n){
		//num=new char[1000];
		num=n;
	}
	CBigInteger(){
		//num=new char[1000];
	}
	friend CBigInteger operator+(CBigInteger &lhs,CBigInteger &rhs);
	friend CBigInteger operator-(CBigInteger &lhs,CBigInteger &rhs);
	friend CBigInteger operator*(CBigInteger &lhs,CBigInteger &rhs);
	friend ostream &operator<<(ostream &out,const CBigInteger &rhs);
	friend istream &operator>>(istream &in,CBigInteger &rhs);
	~CBigInteger(){
        //delete num;
	}
};
CBigInteger operator+(CBigInteger &lhs,CBigInteger &rhs){
	int pos=0,tag=0;
    int flag=0;
    string renum;
    if(lhs.num[0]=='-'&&rhs.num[0]!='-'){
        lhs.num.erase(lhs.num.begin(),lhs.num.begin()+1);
       return rhs-lhs;
    }
    else if(rhs.num[0]=='-'&&lhs.num[0]!='-'){
        rhs.num.erase(rhs.num.begin(),rhs.num.begin()+1);
        return rhs-lhs;
    }
    else if(rhs.num[0]=='-'&&lhs.num[0]=='-'){
        rhs.num.erase(rhs.num.begin(),rhs.num.begin()+1);
        lhs.num.erase(lhs.num.begin(),lhs.num.begin()+1);
        flag=1;
    }
    for(int i=(lhs.num.size()-1),j=(rhs.num.size()-1);i>=0||j>=0;i--,j--){
        int num1= i>=0 ? lhs.num[i]-'0' :0;
        int num2= j>=0 ? rhs.num[j]-'0' :0;
        renum+=(num1+num2+tag)%10+'0';
        tag=(num1+num2+tag)/10;
    }
    if(tag){
        renum+=tag+'0';
    }
    string temp=renum;
    for(int i=0;i<renum.size()-1;i++){
        if(temp[temp.size()-1]=='0'){
            temp.erase(temp.end()-1,temp.end());
        }
    }
    renum=temp;
    if(flag){
        renum+='-';
    }
    reverse(renum.begin(),renum.end());
    return CBigInteger(renum);
}
CBigInteger operator-(CBigInteger &lhs,CBigInteger &rhs){
	int pos=0,tag=0;
    int flag=0;
    string renum;
    if(lhs.num[0]=='-'&&rhs.num[0]!='-'){
        lhs.num.erase(lhs.num.begin(),lhs.num.begin()+1);
        rhs.num.insert(rhs.num.begin(),'-');
       return lhs+rhs;
    }
    else if(rhs.num[0]=='-'&&lhs.num[0]!='-'){
        rhs.num.erase(rhs.num.begin(),rhs.num.begin()+1);
        return rhs+lhs;
    }
    else if(rhs.num[0]=='-'&&lhs.num[0]=='-'){
        rhs.num.erase(rhs.num.begin(),rhs.num.begin()+1);
        lhs.num.erase(lhs.num.begin(),lhs.num.begin()+1);
        return rhs-lhs;
    }
    if(lhs.num<rhs.num){
        swap(lhs.num,rhs.num);
        flag=1;
    }
    for(int i=(lhs.num.size()-1),j=(rhs.num.size()-1);i>=0||j>=0;i--,j--){
        int num1= i>=0 ? lhs.num[i]-'0' :0;
        int num2= j>=0 ? rhs.num[j]-'0' :0;
        int sum=num1-num2-tag;
        if(sum<0){
            sum+=10;
            tag--;
        }
        renum+=(sum)%10+'0';
        tag=(sum)/10;
    }
    if(tag){
        renum+=tag+'0';
    }
    string temp=renum;
    for(int i=0;i<renum.size()-1;i++){
        if(temp[temp.size()-1]=='0'){
            temp.erase(temp.end()-1,temp.end());
        }
    }
    renum=temp;
    if(flag){
        renum+='-';
    }
    reverse(renum.begin(),renum.end());
    return CBigInteger(renum);
}
CBigInteger operator*(CBigInteger &lhs,CBigInteger &rhs){
	int pos=0,tag=0;
    int nagetive=0;
    if(lhs.num[0]=='-'){
        nagetive+=1;
        lhs.num.erase(lhs.num.begin(),lhs.num.begin()+1);
    }
    if(rhs.num[0]=='-'){
        nagetive+=1;
        nagetive%=2;
        rhs.num.erase(rhs.num.begin(),rhs.num.begin()+1);
    }
    string renum;
    int savenums[1145]={0};
    for(int i=(lhs.num.size()-1);i>=0;i--){
        for(int j=(rhs.num.size()-1);j>=0;j--){
            int num1= i>=0 ? lhs.num[i]-'0' :0;
            int num2= j>=0 ? rhs.num[j]-'0' :0;
            savenums[i+j]+=num1*num2;
        }
    }
    for(int i=(lhs.num.size()+rhs.num.size()-2);i>=0;i--){
        renum+=(savenums[i]+tag)%10+'0';
        tag=(savenums[i]+tag)/10;
    }
    if(tag){
        renum+=tag+'0';
    }
    string temp=renum;
    for(int i=0;i<renum.size()-1;i++){
        if(temp[temp.size()-1]=='0'){
            temp.erase(temp.end()-1,temp.end());
        }
    }
    renum=temp;
    if(nagetive){
        renum+='-';
    }
    reverse(renum.begin(),renum.end());
    return CBigInteger(renum);
}
ostream &operator<<(ostream &out,const CBigInteger &rhs){
    string temp=rhs.num;
    for(int i=0;i<rhs.num.size()-1;i++){
        if(temp[0]=='0'){
            temp.erase(temp.begin(),temp.begin()+1);
        }
        else if(temp[0]=='-'&&temp[1]=='0'&&temp.size()>2){
            temp.erase(temp.begin()+1,temp.begin()+2);
        }
    }
    if(temp[0]=='-'&&temp[1]=='0'){
        temp.erase(temp.begin(),temp.begin()+1);
    }
    if(temp[0]=='-'){
        out<<'(';
    }
    out<<temp;
    if(temp[0]=='-'){
        out<<')';
    }
    return out;
}
istream &operator>>(istream &in,CBigInteger &rhs){
    rhs.num=new char[1000];
    in>>rhs.num;
    return in;
}
/*******************************************/
int main()
{
	#ifndef ONLINE_JUDGE
    freopen("in.in", "r", stdin);
    freopen("out.out", "w", stdout);
    #endif
    int t;
    char op;
    CBigInteger bigNum1;
    CBigInteger bigNum2;
    
    cin >> t;
    while(t--)
    {
        cin >> bigNum1 >> op >> bigNum2;
        cout << bigNum1 << " " << op << " " << bigNum2 << " = ";
        if(op == '+')
            cout << bigNum1 + bigNum2 << endl;
        else if(op == '-')
            cout << bigNum1 - bigNum2 << endl;
        else if(op == '*')
            cout << bigNum1 * bigNum2 << endl;
            
    }
    return 0;
}
