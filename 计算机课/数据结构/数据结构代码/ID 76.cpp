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
#define ll long long
bool compareDates(string& date1,string& date2) {
    int year1, month1, day1;
    int year2, month2, day2;
    sscanf(date1.c_str(), "%d/%d/%d", &year1, &month1, &day1);
    sscanf(date2.c_str(), "%d/%d/%d", &year2, &month2, &day2);
    if (year1 != year2) return year1 < year2;
    if (month1 != month2) return month1 < month2;
    return day1 < day2;
}
struct certificate
{
    string Date;
    string type;
    int firstmoney;
    int secondmoney;
};

int main(){
    #ifndef ONLINE_JUDGE
	freopen("in.in", "r", stdin);
	freopen("out.out", "w", stdout);
    #endif
    int n;
    cin>>n;
    int m;
    cin>>m;
    vector<certificate> v1,v2;
    int n1=0,n2=0;
    for(int i=0;i<n;i++){
        certificate c;
        cin>>c.Date>>c.type>>c.firstmoney>>c.secondmoney;
        if(c.type=="open"){
            v1.push_back(c);
            n1++;
        }
        else{
            v2.push_back(c);
            n2++;
        }
    }
    sort(v1.begin(),v1.end(),[](certificate& a,certificate& b){
        return compareDates(a.Date,b.Date);
    });
    sort(v2.begin(),v2.end(),[](certificate& a,certificate& b){
        return compareDates(a.Date,b.Date);
    });
    for(int i=m-1;i<n1;i++){
        int num1=0,num2=0;
        for(int j=0;j<m;j++){
           num1+=v1[i-j].firstmoney;
           num2+=v1[i-j].secondmoney;
        }
        num1/=m;
        num2/=m;
        cout<<v1[i].Date<<" "<<v1[i].type<<" "<<num1<<" "<<num2<<endl;
    }
    for(int i=m-1;i<n2;i++){
        int num1=0,num2=0;
        for(int j=0;j<m;j++){
           num1+=v2[i-j].firstmoney;
           num2+=v2[i-j].secondmoney;
        }
        num1/=m;
        num2/=m;
        cout<<v2[i].Date<<" "<<v2[i].type<<" "<<num1<<" "<<num2<<endl;
    }
    return 0;
}