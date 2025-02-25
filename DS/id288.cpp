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
class Node{
public:
    string name;
    Node* leftchild;
    Node* rightchild;
    Node(string name){
        this->name = name;
        leftchild = rightchild = nullptr;
    }
    Node(char c){
        name = c;
        leftchild = rightchild = nullptr;
    }
};
class Tree{
public:
    Node* root;
    Tree(){
        root = nullptr;
    }
    Node* buildTree(string arr, int index, int n) {
    if (index >= n || arr[index] == '0') return nullptr;
    Node* root = new Node(arr[index]);
    root->leftchild = buildTree(arr, 2 * index + 1, n);
    root->rightchild = buildTree(arr, 2 * index + 2, n);
    return root;
    }
    int getheight(Node* root){
        if(root == nullptr) return 0;
        return max(getheight(root->leftchild), getheight(root->rightchild)) + 1;
    }
    void getBlanceFactor(Node* root){
        if(root == nullptr) return;
        getBlanceFactor(root->leftchild);
        getBlanceFactor(root->rightchild);
        int left = getheight(root->leftchild);
        int right = getheight(root->rightchild);
        cout<<root->name<<" "<<left - right<<endl;
    }
    
};

int main(){
    #ifndef ONLINE_JUDGE
	freopen("in.in", "r", stdin);
	freopen("out.out", "w", stdout);
    #endif
    int t;
    cin>>t;
    while(t--){
        Tree tree;
        int n;
        cin>>n;
        string arr;
        cin>>arr;
        tree.root = tree.buildTree(arr, 0, n);
        tree.getBlanceFactor(tree.root);
    }
    return 0;
}