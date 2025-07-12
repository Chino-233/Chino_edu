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
class BinNode{
public:
    BinNode *parent;
    BinNode *rightchild;
    BinNode *leftchild;
    int data;
    BinNode(int w){
        data=w;
        rightchild=nullptr;
        leftchild=nullptr;
        parent=nullptr;
    }
};
class Tree{
public:
    BinNode *root=nullptr;
    Tree(){
        root=nullptr;
    }
    void insert(int w){
        BinNode *node=new BinNode(w);
        if(root==nullptr){   
            root=node;
        }
        else{
            BinNode *temp=root;
            while (temp!=nullptr)
            {
            if(w<temp->data){
                if(temp->leftchild==nullptr){
                    temp->leftchild=node;
                    temp->leftchild->parent=temp;
                    break;
                }
                else{
                    temp=temp->leftchild;
                }
            }
            else{
                if(temp->rightchild==nullptr){
                    temp->rightchild=node;
                    temp->rightchild->parent=temp;
                    break;
                }
                else{
                    temp=temp->rightchild;
                }
            }
            }
        }
    }
    BinNode* find(int data){
        BinNode *temp=this->root;
        while (temp!=nullptr)
        {
            if(data<temp->data){
                temp=temp->leftchild;
            }
            else if(data>temp->data){
                temp=temp->rightchild;
            }
            else{
                return temp;
            }
        }
        return nullptr;
    }
    void detele(int data){
        BinNode *temp=find(data);
        if(temp!=nullptr){
            if(temp->leftchild==nullptr&&temp->rightchild==nullptr){
                if(temp->parent->leftchild==temp){
                    temp->parent->leftchild=nullptr;
                }
                else if(temp->parent->rightchild==temp){
                    temp->parent->rightchild=nullptr;
                }
                else{
                    cout<<0<<endl;
                }
            }
            else if(temp->leftchild!=nullptr&&temp->rightchild==nullptr){
                if(temp->parent->leftchild==temp){
                    temp->parent->leftchild=temp->leftchild;
                }
                else if(temp->parent->rightchild==temp){
                    temp->parent->rightchild=temp->leftchild;
                }
                else{
                    cout<<0<<endl;
                }
            }
            else if(temp->leftchild==nullptr&&temp->rightchild!=nullptr){
                if(temp->parent->leftchild==temp){
                    temp->parent->leftchild=temp->rightchild;
                }
                else if(temp->parent->rightchild==temp){
                    temp->parent->rightchild=temp->rightchild;
                }
                else{
                    cout<<0<<endl;
                }
            }
            else{
                BinNode *temp1=temp;
                temp1=temp->leftchild;
                while (temp1->rightchild!=nullptr)
                {
                    temp1=temp1->rightchild;
                }
                if(temp->parent->leftchild==temp){
                    temp->parent->leftchild=temp1;
                    detele(temp1->data);
                    temp1->leftchild=temp->leftchild;
                    temp1->rightchild=temp->rightchild;
                }
                else if(temp->parent->rightchild==temp){
                    temp->parent->rightchild=temp1;
                    detele(temp1->data);
                    temp1->leftchild=temp->leftchild;
                    temp1->rightchild=temp->rightchild;
                }
                else{
                    cout<<0<<endl;
                }
            }
        }
        else{
            return;
        }
    }
    void PreOrder(BinNode *root){
        if(root==nullptr){
            return;
        }
        cout<<root->data<<" ";
        PreOrder(root->leftchild);
        PreOrder(root->rightchild);
    }
};
int main(){
    #ifndef ONLINE_JUDGE
	freopen("in.in", "r", stdin);
	freopen("out.out", "w", stdout);
    #endif
    int n;
    cin>>n;
    Tree tree;
    for(int i=0;i<n;i++){
        int w;
        cin>>w;
        tree.insert(w);
        //tree.PreOrder(tree.root);
    }
    tree.PreOrder(tree.root);
    cout<<endl;
    int m;
    cin>>m;
    for(int i=0;i<m;i++){
        int w;
        cin>>w;
        tree.insert(w);
        tree.PreOrder(tree.root);
        cout<<endl;
    }
    return 0;
}