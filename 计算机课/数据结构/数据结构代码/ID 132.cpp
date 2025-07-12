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
const int N = 1e5 + 10;
typedef double db;
#define int long long
class Customer {
private:
    int id;
    int arrivalTime;
    int processTime;
    bool isVIP;
public:
    Customer(int arrivalTimeval, int processTimeval, bool isVIPval,int idval)
    :arrivalTime(arrivalTimeval),
    processTime(processTimeval),isVIP(isVIPval),id(idval) {}
    int getArrivalTime() const {
        return arrivalTime;
    }
    int getProcessTime() const {
        return processTime;
    }
    bool getIsVIP() const {
        return isVIP;
    }
    int getId() const {
        return id;
    }
    static bool cmp(const Customer &a, const Customer &b) {
        return a.getArrivalTime() < b.getArrivalTime();
    }
};

class Window {
private:
    int endTime;
    int hadservicecomustmer;
public:
    Window():endTime(0),hadservicecomustmer(0) {}
    Window(int endTimeval, int hadservicecomustmerval):endTime(endTimeval),hadservicecomustmer(hadservicecomustmerval){}
    int getEndTime(){
        return endTime;
    }
    int getHadServiceCustomer(){
        return hadservicecomustmer;
    }
    void setEndTime(int endTime) {
        this->endTime = endTime;
    }
    void setHadServiceCustomer(int hadservicecomustmer) {
        this->hadservicecomustmer = hadservicecomustmer;
    }
    static bool cmp(Window a,Window b) {
        return a.getEndTime() < b.getEndTime();
    }
};

signed main() {
    #ifndef ONLINE_JUDGE
    freopen("in.in", "r", stdin);
    freopen("out.out", "w", stdout);
    #endif
    int n;
    cin >> n;
    vector<Customer> customers;
    for (int i = 0; i < n; i++) {
        int T, P, V;
        cin >> T >> P >> V;
        if(P>=60){
            P=60;
        }
        customers.push_back(Customer(T, P, V , i));
    }
    int maxTime=customers.back().getArrivalTime()+1;
    list<Customer> Waitcustomers;
    vector<Window> windows;
    int k,vipk;
    cin>>k>>vipk;
    for(int i=0;i<k;i++){
        Window temp(0,0);
        windows.push_back(temp);
    }
    int sumwaittme=0,maxwaittime=0,lastwaittime=0;
    for(int m=0;m<1145141919810;m++) {
        if(customers.empty()&&Waitcustomers.empty()) {
            for(int j=0;j<k;j++){
                if(windows[j].getEndTime()>lastwaittime){
                    lastwaittime=windows[j].getEndTime();
                }
            }
            break;
        }
        while(!customers.empty()&&customers.front().getArrivalTime() == m) {
            Waitcustomers.push_back(customers.front());
            customers.erase(customers.begin());
        }
        if(Waitcustomers.empty()) {
            continue;
        }
        if(windows[vipk].getEndTime()<=m){
            list<Customer>::iterator it=Waitcustomers.begin();
            for(int j=0;j<Waitcustomers.size();j++){
                if(it->getIsVIP()){
                    windows[vipk].setEndTime(m+it->getProcessTime());
                    windows[vipk].setHadServiceCustomer(windows[vipk].getHadServiceCustomer()+1);
                    sumwaittme+=m-it->getArrivalTime();
                    if(m-it->getArrivalTime()>maxwaittime){
                        maxwaittime=m-it->getArrivalTime();
                    }
                    Customer temp=*it;
                    /*
                    cout<<"窗口"<<vipk<<"在"<<m<<"时刻开始服务"<<"第"<<
                    windows[vipk].getHadServiceCustomer()<<"位客户,"<<"其属性为 T:"<<temp.getArrivalTime()<<
                    " P:"<<temp.getProcessTime()<<" V:"<<temp.getIsVIP()<<" 结束时间是"<<
                    windows[vipk].getEndTime()<<endl;
                    */
                    Waitcustomers.erase(it);
                    break;
                }
                it++;
            }
        }
        for(int j=0;j<k;j++){
            if(Waitcustomers.empty()){
                    break;
            }
            if(windows[j].getEndTime()<=m){
                list<Customer>::iterator it=Waitcustomers.begin();
                windows[j].setEndTime(m+it->getProcessTime());
                windows[j].setHadServiceCustomer(windows[j].getHadServiceCustomer()+1);
                sumwaittme+=m-it->getArrivalTime();
                if(m-it->getArrivalTime()>maxwaittime){
                    maxwaittime=m-it->getArrivalTime();
                }
                Customer temp=*it;
                /*
                cout<<"窗口"<<j<<"在"<<m<<"时刻开始服务"<<"第"<<
                windows[j].getHadServiceCustomer()<<"位客户,"<<"其属性为 T:"<<temp.getArrivalTime()<<
                " P:"<<temp.getProcessTime()<<" V:"<<temp.getIsVIP()<<" 结束时间是"<<
                windows[j].getEndTime()<<endl;
                */
                Waitcustomers.erase(it);
            }
        }
        
    }
    cout<<round(sumwaittme*10.0/n)/10<<" "<<maxwaittime<<" "<<lastwaittime<<endl;
    for(int i=0;i<k;i++){
        cout<<windows[i].getHadServiceCustomer();
        if(i!=k){
            cout<<" ";
        }
    }
    return 0;
}