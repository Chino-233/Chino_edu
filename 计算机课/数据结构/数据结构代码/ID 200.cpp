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

/*目描述

给定一棵二叉树的先序遍历序列和中序遍历序列，要求计算该二叉树的高度。

 


输入

输入首先给出正整数N（≤50），为树中结点总数。下面两行先后给出先序和中序遍历序列，均是长度为N的不包含重复英文字母（区别大小写）的字符串。

 


输出

输出为一个整数，即该二叉树的高度。*/
    TreeNode* CreateTree(string s1,string s2,int s1left,int s1right,int s2left,int s2right) {
        if(s1left>s1right){//如果左边界大于右边界 
	        return NULL;
        }
    TreeNode* root=new TreeNode;//分配一个新的结点 
	    root->data =s1[s1left];//先序遍历的第一个就是二叉树的根结点 ，将根结点存入
	int k;
	for(k=s2left;k<=s2right;k++)//在中序遍历中找到根节点所在的位置 
	{
		if(s2[k]==s1[s1left])
		break;
	}
	int numLeft=k-s2left;//左子树的结点个数
	 
	root->left =CreateTree(s1,s2,s1left+1,s1left+numLeft,s2left,k-1);//左子树的先序区间为[preleft+1,preleft+numLeft] ,中序区间为[inleft,k-1] 
	root->right =CreateTree(s1,s2,s1left+numLeft+1,s1right,k+1,s2right);//右子树的先序区间为[preleft+numLeft+1，prerigh
    return root;
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
    //while (t--) {
        string s1,s2;
        cin >> s1 >> s2;
        int i = 0;
        Tree tree;
        TreeNode *root = new TreeNode();
        root=tree.CreateTree(s1,s2,0,t-1,0,t-1);
        tree.setparant(root);
        i=0;
        tree.getheight(root, i, 1);
        cout<<i;
    //}
    return 0;
}
