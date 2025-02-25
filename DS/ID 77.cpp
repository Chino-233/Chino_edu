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
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <list>
#include <deque>
#include <bitset>
#include <functional>
#include <numeric>
using namespace std;
const int N = 1e5 + 10;
typedef double db;
#define ll long long
class Dormitory_Room{
protected:
    int roomnum;
    string name;
public:
    Dormitory_Room(string name,int roomnum){
        this->name=name;
        this->roomnum=roomnum;
    }
    void setname(string name){
        this->name=name;
    }
    void setroomnum(int roomnum){
        this->roomnum=roomnum;
    }
    string getname(){
        return name;
    }
    int getroomnum(){
        return roomnum;
    }
    void print(){
        cout<<roomnum;
    }
    void printalredayused(){
        cout<<name<<"("<<roomnum<<")";
    }
};
int main(){
    #ifndef ONLINE_JUDGE
	freopen("in.in", "r", stdin);
	freopen("out.out", "w", stdout);
    #endif
    list<Dormitory_Room> alreadyused;
    list<Dormitory_Room> dontused;
    int n;
    cin>>n;
    int pos=101;
    for(int i=0;i<n;i++){
        string name;
        int roomnum;
        cin>>name>>roomnum;
        while (roomnum!=pos)
        {
            Dormitory_Room temp("",pos);
            dontused.push_back(temp);
            pos++;
        }
        pos++;
        Dormitory_Room temp(name,roomnum);
        alreadyused.push_back(temp);
    }
    if(pos<=120){
        for(int i=pos;i<=120;i++){
            Dormitory_Room temp("",i);
            dontused.push_back(temp);
        }
    }
    int m;
    cin>>m;
    while (m--)
    {
        string type;
        cin>>type;
        if(type=="assign"){
            string name;
            cin>>name;
            Dormitory_Room temp=dontused.front();
            temp.setname(name);
            int pos=0;
            if(temp.getroomnum()<alreadyused.front().getroomnum()){
                alreadyused.push_front(temp);
            }
            else if(temp.getroomnum()>alreadyused.back().getroomnum()){
                alreadyused.push_back(temp);
            }
            else{
                for(auto it=alreadyused.begin();it!=alreadyused.end();it++){
                    if(it->getroomnum()>temp.getroomnum()){
                        alreadyused.insert(it,temp);
                        break;
                    }
                }
            }
            dontused.pop_front();
        }
        else if(type=="return"){
            int roomnum;
            cin>>roomnum;
            for(auto it=alreadyused.begin();it!=alreadyused.end();it++){
                if(it->getroomnum()==roomnum){
                    Dormitory_Room temp(it->getname(),roomnum);
                    dontused.push_back(temp);
                    alreadyused.erase(it);
                    break;
                }
            }
        }
        else if(type=="display_free"){
            for(auto it=dontused.begin();it!=dontused.end();it++){
                (*it).print();
                if(it!=--dontused.end()) cout<<"-";
            }
            cout<<endl;
        }
        else if(type=="display_used"){
            for(auto it=alreadyused.begin();it!=alreadyused.end();it++){
                (*it).printalredayused();
                if(it!=--alreadyused.end()) cout<<"-";
            }
            cout<<endl;
        }
    }
    
    return 0;
}