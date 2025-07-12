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
int n;
int b;
class BiNode{
public:
    char data;
    BiNode* left;
    BiNode* right;
    BiNode() {
        this->data = 0;
        this->left = nullptr;
        this->right = nullptr;
    }
};
class Node{
public:
    char data;
    Node* children[100];
    Node(){
        for(int i = 0;i < 26;i++){
            this->children[i] = nullptr;
        }
    }
};
class Tree{
public:
    Node* root;
    string s;
    int pos;
    Tree(string s1){
        s.assign(s1);
        pos = 0;
        this->root = Createtree();
    }
    Node* Createtree(){
        if(s[pos] == '0'){
            return nullptr;
        }
        Node* temp = new Node();
        temp->data = s[pos];
        for(int i = 0;i<b;i++){
            pos++;
            temp->children[i] = Createtree();
        }
        return temp;

    }
    Tree(){
        this->root = new Node();
    }
    BiNode* toBiTree(Node* root){
        BiNode* temp = new BiNode();
        if(root){
            temp->data = root->data;
            int cut=0;
            while (!root->children[cut] && cut<b)
            {
                cut++;
            }
            if(cut==b){
                temp->left = toBiTree(nullptr);
            }
            else{
                temp->left = toBiTree(root->children[cut]);
            }
            if(temp->left){
                BiNode* temp1 = temp->left;
                for(int i = cut+1;i<b;i++){
                    temp1->right = toBiTree(root->children[i]);
                    if(temp1->right){
                        temp1 = temp1->right;
                    }
                }
            }
            return temp;
        }
        return nullptr;
    }
};
class Bitree{
public:
    BiNode* root;
    Bitree(){
        this->root = nullptr;
    }
    void PreOreder(BiNode* root,string &s){
        if(root->left==nullptr&&root->right==nullptr){
            for(int i = 0;i<s.size();i++){
                cout<<s[i];
                if(i!=s.size()-1) cout<<" ";
            }
            cout<<endl;
            //cout<<root->data<<endl;
            return;
        }
        if(root->left!=nullptr){
            s+="0";
            PreOreder(root->left,s);
            s.pop_back();
        }
        if(root->right!=nullptr){
            s+="1";
            PreOreder(root->right,s);
            s.pop_back();
        }
        return;
    }
};
int main(){
    #ifndef ONLINE_JUDGE
	freopen("in.in", "r", stdin);
	freopen("out.out", "w", stdout);
    #endif
    cin>>n;
    int num = n;
    cin>>b;
    Bitree all;
    while(n--){
        string s;
        char c;
        while (cin>>c)
        {
            s+=c;
            if(getchar()=='\n') break;
        }
        Tree tree(s);
        Bitree bitree;
        bitree.root = tree.toBiTree(tree.root);
        if(n==num-1){
            all.root=bitree.root;
        }
        else{
            BiNode* temp = all.root;
            while(temp->right!=nullptr){
                temp = temp->right;
            }
            temp->right = bitree.root;
        }
    }
    string s1="";
    all.PreOreder(all.root,s1);
    return 0;
}