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
class BinTreeNode{
    public:
        int data;
        BinTreeNode *left;
        BinTreeNode *right;
        BinTreeNode(int data){
            this->data = data;
            this->left = NULL;
            this->right = NULL;
        }
};
class Bintree{
    public:
        BinTreeNode *root;
        Bintree(){
            root = NULL;
        }
        void create(int arr[], int n){
            for(int i = 0; i < n; i++){
                insert(arr[i]);
            }
        }
        void insert(int data){
            BinTreeNode *node = new BinTreeNode(data);
            if(root == NULL){
                root = node;
            }else{
                BinTreeNode *temp = root;
                while(temp != NULL){
                    if(data < temp->data){
                        if(temp->left == NULL){
                            temp->left = node;
                            break;
                        }else{
                            temp = temp->left;
                        }
                    }else{
                        if(temp->right == NULL){
                            temp->right = node;
                            break;
                        }else{
                            temp = temp->right;
                        }
                    }
                }
            }
        }
        void preOrder(BinTreeNode *node){
            if(node != NULL){
                cout << node->data << " ";
                preOrder(node->left);
                preOrder(node->right);
            }
        }
        void minOrder(BinTreeNode *node){
            if(node != NULL){
                minOrder(node->left);
                cout << node->data << " ";
                minOrder(node->right);
            }
        }
        void find(int data){
            BinTreeNode *temp = root;
            int high = 1;
            while(temp != NULL){
                if(temp->data == data){
                    cout<<high<<endl;
                    return;
                }else if(temp->data > data){
                    temp = temp->left;
                    high++;
                }else{
                    temp = temp->right;
                    high++;
                }
            }
            cout << -1 << endl;
        }
        void del(int data){
            BinTreeNode *temp = root;
            BinTreeNode *parent = NULL;
            while(temp != NULL){
                if(temp->data == data){
                    if(temp->left == NULL && temp->right == NULL){
                        if(parent == NULL){
                            root = NULL;
                        }else if(parent->left == temp){
                            parent->left = NULL;
                        }
                    }
                }
            }
        }
};
int main(){
    #ifndef ONLINE_JUDGE
	freopen("in.in", "r", stdin);
	freopen("out.out", "w", stdout);
    #endif
    int n;
    cin >> n;
    int arr[n]={0};
    Bintree tree;
    for(int i = 0; i < n; i++){
        cin >> arr[i];
    }
    tree.create(arr, n);
    //tree.preOrder(tree.root);
    tree.minOrder(tree.root);
    cout<<endl;
    int m;
    cin >> m;
    for(int i = 0; i < m; i++){
        int m;
        cin >> m;
        //tree.insert(m);
        //tree.preOrder(tree.root);
        //cout<<endl;
        tree.find(m);
    }
    return 0;
}