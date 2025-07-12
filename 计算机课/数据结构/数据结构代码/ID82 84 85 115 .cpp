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
class Clist{
private:
    int *arr ;
    int size;
    int maxsize=1000;
public:
    void create(int n){
        arr = new int[maxsize];
        for(int i=0;i<n;i++){
            cin>>arr[i];
        }
        size = n;
        print();
    }
    void insert(int pos,int x){
        if(size==maxsize){
            cout<<"error"<<endl;
            return;
        }
        if(pos<0||pos>size){
            cout<<"error"<<endl;
            return;
        }
        for(int i=size;i>pos;i--){
            arr[i]=arr[i-1];
        }
        arr[pos]=x;
        size++;
        print();
    }
    void delete1(int pos){
        if(size==0){
            cout<<"error"<<endl;
            return;
        }
        if(pos<0||pos>=size){
            cout<<"error"<<endl;
            return;
        }
        for(int i=pos;i<size-1;i++){
            arr[i]=arr[i+1];
        }
        size--;
        print();
    }
    void find(int pos){
        if(size==0){
            cout<<"error"<<endl;
            return;
        }
        if(pos<0||pos>=size){
            cout<<"error"<<endl;
            return;
        }
        cout<<arr[pos]<<endl;
    }
    void multiinsert(int pos,int n,int item[]){
        if(size+n>maxsize){
            cout<<"error"<<endl;
            return;
        }
        if(pos<0||pos>size){
            cout<<"error"<<endl;
            return;
        }
        for(int i=size-1;i>=pos;i--){
            arr[i+n]=arr[i];
        }
        for(int i=0;i<n;i++){
            arr[pos+i]=item[i];
        }
        size+=n;
        print();
    }
    void multidel(int pos,int n){
        if(size==0){
            cout<<"error"<<endl;
            return;
        }
        if(pos<0||pos+n>size){
            cout<<"error"<<endl;
            return;
        }
        for(int i=pos;i<size-n;i++){
            arr[i]=arr[i+n];
        }
        size-=n;
        print();
    }
    void print(){
        if(size==0){
            cout<<"error"<<endl;
            return;
        }
        //cout<<size<<" ";
        for(int i=0;i<size;i++){
            if(i==size-1){
                cout<<arr[i]<<" "<<endl;
                break;
            }
            cout<<arr[i]<<" ";
        }
    }
    void sort1(){
        if(size==0){
            cout<<"error"<<endl;
            return;
        }
        sort(arr,arr+size);
        //print();
    }
    void merge(Clist &list){
        if(size+list.size>maxsize){
            cout<<"error"<<endl;
            return;
        }
        for(int i=0;i<list.size;i++){
            arr[size+i]=list.arr[i];
        }
        size+=list.size;
        //print();
    }
    void Cyclic_shift_right(){
        if(size==0){
            cout<<"error"<<endl;
            return;
        }
        int temp = arr[size-1];
        for(int i=size-1;i>0;i--){
            arr[i]=arr[i-1];
        }
        arr[0]=temp;
        //print();
    }
    void Cyclic_shift_left(){
        if(size==0){
            cout<<"error"<<endl;
            return;
        }
        int temp = arr[0];
        for(int i=0;i<size-1;i++){
            arr[i]=arr[i+1];
        }
        arr[size-1]=temp;
        //print();
    }
};
int main(){
    #ifndef ONLINE_JUDGE
	freopen("in.in", "r", stdin);
	freopen("out.out", "w", stdout);
    #endif
    Clist list1;
    int n;
    cin>>n;
    list1.create(n);
    int m;
    cin>>m;
    if(m==0){
        cin>>n;
        for(int i=0;i<n;i++){
            list1.Cyclic_shift_left();
        }
       list1.print();
    }
    else if(m==1){
        cin>>n;
        for(int i=0;i<n;i++){
            list1.Cyclic_shift_right();
        }
       list1.print();
    }
    cin>>m;
    if(m==0){
        cin>>n;
        for(int i=0;i<n;i++){
            list1.Cyclic_shift_left();
        }
       list1.print();
    }
    else if(m==1){
        cin>>n;
        for(int i=0;i<n;i++){
            list1.Cyclic_shift_right();
        }
       list1.print();
    }
    return 0;
}