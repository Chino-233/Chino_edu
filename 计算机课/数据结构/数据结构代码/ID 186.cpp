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
/**/
class TreeNode {
public:
    char data;
    TreeNode *left;
    TreeNode *right;
    TreeNode *parent;
    int id;
    TreeNode(char c) : data(c), left(nullptr), right(nullptr), parent(nullptr) {}
    TreeNode() : data(0),left(nullptr), right(nullptr), parent(nullptr) {}
    void SetData(char c) {
        data = c;
    }
    void SetLeft(TreeNode *l) {
        left = l;
    }
    void SetRight(TreeNode *r) {
        right = r;
    }
};

class Tree{
public:
    TreeNode *root;
    Tree() : root(nullptr) {}
    void CreateTree(TreeNode *&root, string s,int &pos) {
        if (pos >= s.length()) {
            return;
        }
        if (s[pos] == '#') {
            root = nullptr;
        }
        else{
            root=new TreeNode();
            root->SetData(s[pos]);
            TreeNode* left = new TreeNode();
            TreeNode* right = new TreeNode();
            root->SetLeft(left);
            root->SetRight(right);
            CreateTree(root->left, s,++pos);
            CreateTree(root->right, s,++pos);
        }
    }
    void PreOrder(TreeNode *root) {
        if (root == nullptr) {
            return;
        }
        cout << root->data;
        PreOrder(root->left);
        PreOrder(root->right);
    }
    void InOrder(TreeNode *root) {
        if (root == nullptr) {
            return;
        }
        InOrder(root->left);
        cout << root->data;
        InOrder(root->right);
    }
    void PostOrder(TreeNode *root) {
        if (root == nullptr) {
            return;
        }
        PostOrder(root->left);
        PostOrder(root->right);
        cout << root->data;
    }
    void printLeaf(TreeNode *root) {
        if (root == nullptr) {
            return;
        }
        if (root->left == nullptr && root->right == nullptr) {
            cout<<root->data;
            cout<<" ";
        }
        printLeaf(root->left);
        printLeaf(root->right);
    }
    void printleafparent(TreeNode *root){
        if (root == nullptr) {
            return;
        }
        if (root->left == nullptr && root->right == nullptr) {
            //cout << root->data;
            //cout << " ";
            cout << root->parent->data<<" ";
            //cout << endl;
        }
        printleafparent(root->left);
        printleafparent(root->right);
    }
    void setparant(TreeNode *root){
        if (root == nullptr) {
            return;
        }
        if (root->left != nullptr) {
            root->left->parent = root;
            setparant(root->left);
        }
        if (root->right != nullptr) {
            root->right->parent = root;
            setparant(root->right);
        }
    }
    void rankOrder(TreeNode *root) {
        queue<TreeNode*> q;
        q.push(root);
        while (!q.empty()) {
            TreeNode *node = q.front();
            q.pop();
            cout << node->data;
            if (node->left != nullptr) {
                q.push(node->left);
            }
            if (node->right != nullptr) {
                q.push(node->right);
            }
        }
    }
    void getheight(TreeNode *root, int &height, int level) {
        if (root == nullptr) {
            return;
        }
        if (level > height) {
            height = level;
        }
        getheight(root->left, height, level + 1);
        getheight(root->right, height, level + 1);
    }
    void printdoubleparent(TreeNode *root,int &pos,int arr[],int parentpos[]) {
        if (root == nullptr) {
            return;
        }
        queue<TreeNode*> q;
        q.push(root);
        while (!q.empty())
        {
            q.front()->id=pos;
            arr[pos]=q.front()->data;
            if(q.front()->parent!=nullptr){
                parentpos[pos]=q.front()->parent->id;
            }
            else{
                parentpos[pos]=-1;
            }
            pos++;
            if(q.front()->left!=nullptr){
                q.push(q.front()->left);
            }
            if(q.front()->right!=nullptr){
                q.push(q.front()->right);
            }
            q.pop();
        }
        
    }
    TreeNode* GetRoot() {
        return root;
    }
};

int main() {
    #ifndef ONLINE_JUDGE
	freopen("in.in", "r", stdin);
	freopen("out.out", "w", stdout);
    #endif
    int t;
    cin >> t;
    while (t--) {
        string s;
        cin >> s;
        int len = s.length();
        int i = 0;
        Tree tree;
        TreeNode *root = new TreeNode();
        tree.CreateTree(root, s, i);
        tree.setparant(root);
        int arr[100]={0};
        int pos=0;
        int parentpos[100]={0};
        tree.printdoubleparent(root,pos,arr,parentpos);
        for(int i=0;i<pos;i++){
            cout<<(char)arr[i];
            if(i!=pos-1){
                cout<<" ";
            }
            else{
                cout<<endl;
            }
        }
        for(int i=0;i<pos;i++){
            cout<<parentpos[i];
            if(i!=pos-1){
                cout<<" ";
            }
            else{
                cout<<endl;
            }
        }
    }
    return 0;
}
