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
class TreeNode{
public:
    string data;
    string type;
    TreeNode* next;
    TreeNode* left;
    TreeNode* right;
    TreeNode* parent;
    TreeNode(){
        next=nullptr;
        left=nullptr;
        right=nullptr;
        parent=nullptr;
    }
    void setTreeNode(string c){
        data=c;
        if(c=="1"){
            type="I";
        }
        else if(c=="0"){
            type="B";
        }
    }
    
};
void PostOrder(TreeNode* root){
    if(root->left!=nullptr){
        PostOrder(root->left);
    }
    if(root->right!=nullptr){
        PostOrder(root->right);
    }
    cout<<root->type;
    
}
void Chino(){
    int n;
    cin>>n;
    string s;
    cin>>s;
    queue<TreeNode*> q;
    stack<TreeNode*> q1;
    TreeNode* root=new TreeNode();
    root->setTreeNode(s);
    q.push(root);
    while (!q.empty())
    {
        TreeNode* temp=q.front();
        if(temp->data.length()>1){
            string s1=temp->data.substr(0,temp->data.length()/2);
            string s2=temp->data.substr((temp->data.length()/2),temp->data.length()/2);
            TreeNode* left=new TreeNode();
            TreeNode* right=new TreeNode();
            left->setTreeNode(s1);
            right->setTreeNode(s2);
            temp->left=left;
            temp->right=right;
            left->parent=temp;
            right->parent=temp;
            q.push(left);
            q.push(right);
        }
        q1.push(q.front());
        q.pop();
    }
    while (!q1.empty())
    {
        q.push(q1.top());
        q1.pop();
    }
    while (!q.empty())
    {
        if(q.front()->data.length()==1){
            q.pop();
        }
        else{
            if(q.front()->left->type=="I"&&q.front()->right->type=="I"){
                q.front()->type="I";
            }
            else if(q.front()->left->type=="B"&&q.front()->right->type=="B"){
                q.front()->type="B";
            }
            else{
                q.front()->type="F";
            }
            q.pop();
        }
    }
    PostOrder(root);
}
int main(){
    #ifndef ONLINE_JUDGE
	freopen("in.in", "r", stdin);
	freopen("out.out", "w", stdout);
    #endif
    Chino();
    return 0;
}