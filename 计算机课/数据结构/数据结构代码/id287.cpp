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
class Node{
public:
    char data;
    Node** next;
    Node(char data){
        this->data=data;
        next=new Node*[26];
        for(int i=0;i<26;i++){
            next[i]=NULL;
        }
    }
};
class Tree{
    Node* root;
public:
    Tree(){
        root=new Node('/');
    }
    void insert(string s){
        Node* temp=this->root;
        for(int i=0;i<s.size();i++){
            int index=s[i]-'a';
            if(temp->next[index]==NULL){
                temp->next[index]=new Node(s[i]);
            }
            temp=temp->next[index];
        }
    }
    void treeheightorder(){
        queue<Node*> q;
        q.push(root);
        while(!q.empty()){
            Node* node=q.front();
            q.pop();
            if(node->data!='/'){
                if(node->data=='0'){
                    int a=0;
                }
                cout<<node->data;
            }
            for(int i=0;i<26;i++){
                if(node->next[i]!=NULL){
                    q.push(node->next[i]);
                }
            }
        }
    }
    int find(string s){
        Node* temp=this->root;
        for(int i=0;i<s.size();i++){
            int index=s[i]-'a';
            if(temp->next[index]==NULL){
                return 0;
            }
            temp=temp->next[index];
        }
        return getwordnum(temp);
    }
    int getwordnum(Node *node){
        int num=0;
        for(int i=0;i<26;i++){
            if(node->next[i]!=NULL){
                num+=getwordnum(node->next[i]);
            }
        }
        if(num==0){
            return 1;
        }
        return num;
    }
};
int main(){
    #ifndef ONLINE_JUDGE
	freopen("in.in", "r", stdin);
	freopen("out.out", "w", stdout);
    #endif
    string s;
    int t;
    Tree tree;
    while(cin>>s){
        if(s[0]>='0'&&s[0]<='9'){
            t=stoi(s);
            break;
        }
        tree.insert(s);
    }
    tree.treeheightorder();
    cout<<endl;
    while (t--)
    {
        cin>>s;
        cout<<tree.find(s)<<endl;
    }
    return 0;
}