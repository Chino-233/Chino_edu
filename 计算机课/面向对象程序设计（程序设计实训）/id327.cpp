#include <iostream>
using namespace std;
//根据主函数代码完成以下填空
/********** Write your code here! **********/
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
template<int N>
class MyBitset{
private:
	string data;
public:
    MyBitset(int numval):data(N,'0'){
		int num=numval;
		int positiveflag=0;
		if(numval<0){
			positiveflag=1;
			num=abs(numval);
			num--;
		}
		//data=new char[N]{48};
        for(int i=0;i<100;i++){
			if(i==0&&num<0){
				data[i]='0';
				continue;
			}
        	data[i]=(num%2)+'0';
        	num/=2;
        	if(i>=N||num==0){
				if(positiveflag==1){
					for(int j=0;j<N;j++){
						if(data[j]=='0'){
							data[j]='1';
						}
						else if(data[j]=='1'){
							data[j]='0';
						}
					}
				}
				reverse(data.begin(),data.end());
				//cout<<data<<endl;
            	break;
        	}
		}
    }
    template<int T>
	friend ostream &operator<<(ostream &out,const MyBitset<T> &rhs);
    MyBitset operator<<=(int &rhs){
		for(int i=0;i<N;i++){
			if(i+rhs<N){
				data[i]=data[i+rhs];
			}
			else{
				data[i]='0';
			}
		}
		return *this;
	}
    MyBitset operator>>=(int &rhs){
		for(int i=N-1;i>=0;i--){
			if(i-rhs>=0){
				data[i]=data[i-rhs];
			}
			else{
				data[i]='0';
			}
		}
		return *this;
	}
};
template<int N>
ostream &operator<<(ostream &out,const MyBitset<N> &rhs){
	out<<rhs.data;
	return out;
}
/*******************************************/
//主函数
int main() 
{
	#ifndef ONLINE_JUDGE
    freopen("in.in", "r", stdin);
    freopen("out.out", "w", stdout);
    #endif
	int n, offset;
    long num;
 	string mode;
 	cin>>n;
 	while (n--) {
 		cin>>num>>mode>>offset;
    	MyBitset<16> bit16(num);
 		MyBitset<32> bit32(num);
 		MyBitset<64> bit64(num);
 		auto f_printbits = [&](){ // c++11以上 
 			cout << bit16 << endl
  				<< bit32 << endl
  				<< bit64 << endl
  				<< endl;
 		};
 		f_printbits();
 		if (mode == ">>") {
 			bit16 >>= offset;
 			bit32 >>= offset;
 			bit64 >>= offset;
 		} 
    	else if (mode == "<<") {
 			bit16 <<= offset;
 			bit32 <<= offset;
 			bit64 <<= offset;
 		}
 		f_printbits();
 	} //end while
 	return 0;
}
