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
#define ll long long
struct Node{
    int data;
    Node* next;
    Node(int x){
        data = x;
        next = NULL;
    }
    Node(){
        data = 114514;
        next = NULL;
    }
};
void inserthash(vector<Node*>& v, int x){
    Node* temp = new Node(x);
    int index = x % 11;
    if(v[index] == NULL){
        v[index] = temp;
    }
    else{
        temp->next = v[index];
        v[index]= temp;
    }
}
void inserthashend(vector<Node*>& v, int x){
    Node* temp = new Node(x);
    int index = x % 11;
    if(v[index] == NULL){
        v[index] = temp;
    }
    else{
        Node* temp2 = v[index];
        while(temp2->next != NULL){
            temp2 = temp2->next;
        }
        temp2->next = temp;
    }
}
int findhash(vector<Node*>& v, int x){
    int index = x % 11;
    Node* temp = v[index];
    int findcut=0;
    while(temp != NULL){
        findcut++;
        if(temp->data == x){
            cout<<index<<" "<<findcut<<endl;
            return 1;
        }
        temp = temp->next;
    }
    inserthashend(v,x);
    cout<<"error"<<endl;
    return 0;
}
int main(){
    #ifndef ONLINE_JUDGE
	freopen("in.in", "r", stdin);
	freopen("out.out", "w", stdout);
    #endif
    int n;
    cin >> n;
    int arr[1000]={0};
    for(int i = 0; i < n; i++){
        cin >> arr[i];
    }
    vector<Node*> v(20);
    for(int i=0;i<n;i++){
        inserthashend(v,arr[i]);
    }
    int t;
    cin >> t;
    for(int i = 0; i < t; i++){
        int x;
        cin>>x;
        findhash(v,x);
    }
    return 0;
}