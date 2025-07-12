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
class TreeNode {
public:
    char data;
    TreeNode *parent;
    vector<TreeNode*> leaf;
    TreeNode(char c) : data(c), parent(nullptr) {}
    TreeNode() : data(0), parent(nullptr) {}
    void SetData(char c) {
        data = c;
    }
};
class Tree{
public:
    TreeNode *root;
    Tree() : root(nullptr) {}
    void setNewNode(TreeNode *&root, int arr[][100],TreeNode* stonenode[],char data[],int i){
        if(stonenode[i]!=nullptr){
            return;
        }
        root->SetData(data[i]);
        stonenode[i]=root;
        int pos=0;
        while (arr[i][pos]!=-1)
        {
            TreeNode* temp2=new TreeNode();
            temp2->SetData(data[arr[i][pos]]);
            temp2->parent=root;
            root->leaf.push_back(temp2);
            setNewNode(temp2,arr,stonenode,data,arr[i][pos]);
            pos++;
        }       
        return;
    }
    void CreateTree(TreeNode *&root, int arr[][100],int arrsize,TreeNode* stonenode[]) {
        for(int i=0;i<arrsize;i++){
            if(i==1){
                int a=0;
            }
            //setNewNode(root,arr,stonenode,i);
        }
    }
    void PostOrder(TreeNode *root) {
        if (root == nullptr) {
            return;
        }
        for(int i=0;i<root->leaf.size();i++){
            PostOrder(root->leaf[i]);
        }
        cout << (char)root->data;
    }
    void PreOrder(TreeNode *root) {
        if (root == nullptr) {
            return;
        }
        for(int i=0;i<root->leaf.size();i++){
            PreOrder(root->leaf[i]);
        }
        cout << (char)root->data;
    }
};
int main(){
    #ifndef ONLINE_JUDGE
	freopen("in.in", "r", stdin);
	freopen("out.out", "w", stdout);
    #endif
    int n,r;
    cin >> n >> r;
    int arr[100][100];
    char data[100];
    for(int i=0;i<n;i++){
        cin >> data[i];
        int pos=0;
        while (1)
        {
            cin >> arr[i][pos];
            if(arr[i][pos]==-1){
                break;
            }
            pos++;
        }
    }
    TreeNode* stonenode[100];
    for(int i=0;i<100;i++){
        stonenode[i]=nullptr;
    }
    Tree tree;
    tree.root=new TreeNode();
    tree.setNewNode(tree.root,arr,stonenode,data,r);
    tree.PreOrder(tree.root);
    return 0;
}