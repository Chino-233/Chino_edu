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
class BiTreeNode
{
public:
    char data;
    BiTreeNode *left;
    BiTreeNode *RightChild;
    BiTreeNode():left(NULL),RightChild(NULL){}
    ~BiTreeNode(){}
};
 
class BiTree
{
public:
    BiTreeNode *Root;
    int pos;
    string strTree;
    BiTreeNode *CreateBiTree();
    void PreOrder(BiTreeNode *t);
    void InOrder(BiTreeNode *t);
    void PostOrder(BiTreeNode *t);
    void level_Order(BiTreeNode *t);
public:
    BiTree(){};
    ~BiTree(){};
    void CreateTree(string TreeArray);
    void PreOrder();
    void InOrder();
    void PostOrder();
    void exchange(BiTreeNode *t);
};
void BiTree::exchange(BiTreeNode *t)
{
    if(t!=NULL)
    {
        exchange(t->left);
        exchange(t->RightChild);
        BiTreeNode *temp;
        temp = t->left;
        t->left = t->RightChild;
        t->RightChild = temp;
    }
}
void BiTree::CreateTree(string TreeArray)
{+
    pos=0;
    strTree.assign(TreeArray);
    Root =CreateBiTree();
}
BiTreeNode *BiTree::CreateBiTree()
{
    BiTreeNode *T;
    char ch;
    ch= strTree[pos++];
    if(ch == '#')
        T=NULL;
    else
    {
        T=new BiTreeNode();
        T->data=ch;
        T->left = CreateBiTree();
        T->RightChild = CreateBiTree();
    }
    return T;
}
void BiTree::PreOrder()
{
    if(strTree[0]=='#')
        cout<<"NULL";
    else
        PreOrder(Root);
    cout<<endl;
}
void BiTree::PreOrder(BiTreeNode *t)
{
    if(t)
    {
        cout<<t->data<<' ';
        PreOrder(t->left);
        PreOrder(t->RightChild);
    }
}
void BiTree::InOrder()
{
    if(strTree[0]=='#')
        cout<<"NULL";
    else
        InOrder(Root);
    cout<<endl;
}
void BiTree::InOrder(BiTreeNode *t)
{
    if(t)
    {
        InOrder(t->left);
        cout<<t->data<<' ';
        InOrder(t->RightChild);
    }
}
void BiTree::PostOrder()
{
    if(strTree[0]=='#')
        cout<<"NULL";
    else
        PostOrder(Root);
    cout<<endl;
}
void BiTree::PostOrder(BiTreeNode *t)
{
    if(t)
    {
        PostOrder(t->left);
        PostOrder(t->RightChild);
        cout<<t->data<<' ';
    }
}
void BiTree::level_Order(BiTreeNode *t)
{
    if(strTree[0]=='#')
        cout<<"NULL";
    else
    {
        queue<BiTreeNode*> qt;
        BiTreeNode *p=t;
        qt.push(p);
        while(1)
        {
            p=qt.front();
            qt.pop();
            cout<<p->data<<' ';
            if(p->left)
                qt.push(p->left);
            if(p->RightChild)
                qt.push(p->RightChild);
            if(qt.empty())
                break;
        }
    }
    cout<<endl;
}
int main()
{
    int t;
    cin>>t;
    string s;
    while(t--)
    {
        cin>>s;
        BiTree t;
        t.CreateTree(s);
        t.exchange(t.Root);
        t.PreOrder();
        t.InOrder();
        t.PostOrder();
        t.level_Order(t.Root);
    }
}
 
 
 