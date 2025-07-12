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
    int data;
    BinNode* left;
    BinNode* right;
    int height;
    BinNode(int data){
        this->data = data;
        this->left = nullptr;
        this->right = nullptr;
        this->height = 0;
    }
    BinNode(){
        this->data = 0;
        this->left = nullptr;
        this->right = nullptr;
        this->height = 0;
    }
};
class BinTree{
public:
    BinNode* root;
    BinTree(){
        this->root = nullptr;
    }
    void insert(int data){
        BinNode* node = new BinNode(data);
        if(this->root == nullptr){
            this->root = node;
            return;
        }
        BinNode* pos = this->root;
        while (pos)
        {
            if(data<pos->data){
                if(pos->left == nullptr){
                    pos->left = node;
                    node->height = pos->height + 1;
                    return;
                }
                else{
                    pos = pos->left;
                }
            }
            else{
                if(pos->right == nullptr){
                    pos->right = node;
                    node->height = pos->height + 1;
                    return;
                }
                else{
                    pos = pos->right;
                }
            }
        }
        
    }
    void findLCA(int a, int b){
        BinNode* pos = this->root;
        while (pos)
        {
            if(a<pos->data && b<pos->data){
                pos = pos->left;
            }
            else if(a>pos->data && b>pos->data){
                pos = pos->right;
            }
            else{
                if(b==pos->data){
                    cout<<b<<" is an ancestor of "<<a<<"."<<endl;
                    return;
                }
                else{
                    cout<<"LCA of "<<a<<" and "<<b<<" is "<<pos->data<<"."<<endl;
                }
                return;
            }
        }
    }
    int findnode(int data){
        BinNode* pos = this->root;
        while (pos)
        {
            if(pos->data == data){
                return pos->height;
            }
            else if(data<pos->data){
                pos = pos->left;
            }
            else{
                pos = pos->right;
            }
        }
        return -1;
    }
};
int main(){
    #ifndef ONLINE_JUDGE
	freopen("in.in", "r", stdin);
	freopen("out.out", "w", stdout);
    #endif
    int m,n;
    cin>>m>>n;
    BinTree tree;
    for(int i=0;i<n;i++){
        int data;
        cin>>data;
        tree.insert(data);
    }
    while (m--)
    {
    int a,b;
    cin>>a>>b;
    int h1 = tree.findnode(a);
    int h2 = tree.findnode(b);
    if(h1==-1){
        if(h2==-1){
            cout<<"ERROR: "<<a<<" and "<<b<<" are not found."<<endl;
        }
        else{
            cout<<"ERROR: "<<a<<" is not found."<<endl;
        }
    }
    else if(h2==-1){
        cout<<"ERROR: "<<b<<" is not found."<<endl;
    }
    else{
        if(h1>h2){
            tree.findLCA(a,b);
        }
        else{
            tree.findLCA(b,a);
        }
    }
    }
    return 0;
}