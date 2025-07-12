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
    int weight;
    Node *left;
    Node *right;
    string code;
    int cinpos;
    char data;
    Node(int w,char d){
        weight = w;
        code = "";
        left = nullptr;
        right = nullptr;
        cinpos = 0;
        data = d;
    }
};
bool cmp1(Node* a,Node* b){
        return a->cinpos < b->cinpos;
}
class HuffmanTree{
public:
    Node *root;
    HuffmanTree(){
        root = nullptr;
    }
    void buildHuffmanTree(int arr[],int n,char data[]){
        auto cmp = [](Node* a, Node* b) {
            if(a->weight > b->weight){
                return true;
            }
            else if(a->weight < b->weight){
                return false;
            }
            else{
                return a->cinpos > b->cinpos;
            }
        };
        priority_queue<Node*, vector<Node*>, decltype(cmp)> q(cmp);
        Node* Nodearray[100];
        for(int i=1;i<=n;i++){
            Node *node = new Node(arr[i],data[i]);
            node->cinpos = i;
            Nodearray[i] = node;
        }
        sort(Nodearray+1,Nodearray+n+1,cmp);
        for(int i=1;i<=n;i++){
            q.push(Nodearray[i]);
        }
        Node* stone[100];
        int pos=1;
        while(q.size()>1){
            Node *left = q.top();
            q.pop();
            Node *right = q.top();
            q.pop();
            Node *parent = new Node(left->weight+right->weight,' ');
            parent->left = left;
            parent->right = right;
            q.push(parent);
        }
        root = q.top();
        sethuffmanCode(root,"");
        //sort(Nodearray+1,Nodearray+n+1,cmp1);
        //for(int i=1;i<=n;i++){
            //cout<<Nodearray[i]->weight<<"-"<<Nodearray[i]->code<<endl;
        //}
    }
    void sethuffmanCode(Node *node,string code){
        if(node==nullptr){
            return;
        }
        if(node->left==nullptr&&node->right==nullptr){
            node->code = code;
        }
        sethuffmanCode(node->left,code+"0");
        sethuffmanCode(node->right,code+"1");
    }
    int Decode(const string codestr,char txtstr[]){
        int pos=0;
        Node *p = root;
        int textpos = 0;
        while (pos<codestr.size())
        {
            if(codestr[pos]=='0'){
                p = p->left;
            }
            else if(codestr[pos]=='1'){
                p = p->right;
            }
            else{
                return -1;
            }
            if(p->left==nullptr&&p->right==nullptr){
                txtstr[textpos] = p->data;
                textpos++;
                p = root;
            }
            else if(pos==codestr.size()){
                return -1;
            }
            pos++;
            if(pos==codestr.size()&&p->left!=nullptr&&p->right!=nullptr){
                if(p==root){
                    return 1;
                }
                return -1;
            }
        }
        return 1;
    }
};
int main(){
    #ifndef ONLINE_JUDGE
	freopen("in.in", "r", stdin);
	freopen("out.out", "w", stdout);
    #endif
    int n;
    cin>>n;
    int arr[100];
    for(int i=1;i<=n;i++){
        cin>>arr[i];
    }
    char data[100];
    for(int i=1;i<=n;i++){
        cin>>data[i];
    }
    int t;
    cin>>t;
    HuffmanTree huffmantree;
    huffmantree.buildHuffmanTree(arr,n,data);
    while(t--){
        string codestr;
        cin>>codestr;
        char txtstr[100];
        int ret = huffmantree.Decode(codestr,txtstr);
        if(ret==-1){
            cout<<"error"<<endl;
        }
        else{
            cout<<txtstr<<endl;
        }
    }
    return 0;
}