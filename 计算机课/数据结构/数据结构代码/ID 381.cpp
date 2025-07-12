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

/*给出一棵树的双亲表示法结果，用一个二维数组表示，位置下标从0开始，如果双亲位置为-1则表示该结点为根结点

编写程序，输出该树的先根遍历结果。


输入

第一个输入t，表示有t棵树

接着每棵树输入3行：

第1行输入n，表示树有n个结点

第2行输入n个英文字母，表示每个树结点的数值

第3行输入n个整数，表示每个结点的双亲在数组的下标

以此类推输入下一棵树


输出

共输出t行，每行输出一棵树的先根遍历结果*/
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
    void setNewNode(TreeNode *&root, int arr[][100],TreeNode* stonenode[],int i){
        if(stonenode[i]!=nullptr){
            return;
        }
        if(arr[1][i]==-1){
            if(root!=nullptr){
                return;
            }
            root=new TreeNode();
            root->SetData(arr[0][i]);
            stonenode[i]=root;
        }
        else{
            if(stonenode[arr[1][i]]!=nullptr){
                TreeNode* temp=new TreeNode();
                temp->SetData(arr[0][i]);
                temp->parent=stonenode[arr[1][i]];
                stonenode[arr[1][i]]->leaf.push_back(temp);
                stonenode[i]=temp;
                return;
            }
            TreeNode* temp=new TreeNode();
            temp->SetData(arr[0][i]);
            if(stonenode[arr[1][i]]==nullptr){
                setNewNode(root,arr,stonenode,arr[1][i]);
                temp->parent=stonenode[arr[1][i]];
                stonenode[arr[1][i]]->leaf.push_back(temp);
                stonenode[i]=temp;
                return;
            }
            else{
                temp->parent=stonenode[arr[1][i]];
                stonenode[arr[1][i]]->leaf.push_back(temp);
                stonenode[i]=temp;
                return;
            }
        }
        return;
    }
    void CreateTree(TreeNode *&root, int arr[][100],int arrsize,TreeNode* stonenode[]) {
        for(int i=0;i<arrsize;i++){
            setNewNode(root,arr,stonenode,i);
        }
    }
    void PreOrder(TreeNode *root) {
        if (root == nullptr) {
            return;
        }
        cout << (char)root->data;
        for(int i=0;i<root->leaf.size();i++){
            PreOrder(root->leaf[i]);
        }
    }
    TreeNode* findNode(TreeNode *root, char c) {
        if (root == nullptr) {
            return nullptr;
        }
        if (root->data == c) {
            return root;
        }
        for (int i = 0; i < root->leaf.size(); i++) {
            TreeNode *temp = findNode(root->leaf[i], c);
            if (temp != nullptr) {
                return temp;
            }
        }
        return nullptr;
    }
    void printChliren(TreeNode *root,int arr[][100],int arrsize){
        for(int i=0;i<arrsize;i++){
            TreeNode* temp=findNode(root,(char)arr[0][i]);
            cout<<(char)temp->data<<" ";
            for(int j=0;j<temp->leaf.size();j++){
                char c=(char)temp->leaf[j]->data;
                for(int k=0;k<arrsize;k++){
                    if(arr[0][k]==c){
                        cout<<k<<" ";
                    }
                }
            }
            if(temp->leaf.size()==0){
                cout<<"-1";
            }
            cout<<endl;
        }
    }
    
};
int main(){
    #ifndef ONLINE_JUDGE
	freopen("in.in", "r", stdin);
	freopen("out.out", "w", stdout);
    #endif
    //while (t--)
    //{
        int n;
        cin>>n;
        int arr[2][100];
        for(int i=0;i<100;i++){
            arr[0][i]=0;
            arr[1][i]=0;
        }
        for(int i=0;i<n;i++){
            char temp;
            cin>>temp;
            int a=(int)temp;
            arr[0][i]=a;
        }
        for(int i=0;i<n;i++){
            cin>>arr[1][i];
        }
        TreeNode* stonenode[100];
        for(int i=0;i<100;i++){
            stonenode[i]=nullptr;
        }
        Tree tree;
        tree.CreateTree(tree.root,arr,n,stonenode);
        tree.printChliren(tree.root,arr,n);
    //}
    return 0;
}