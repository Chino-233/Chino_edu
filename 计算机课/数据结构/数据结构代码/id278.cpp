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
int judgeright(int arr[],int l,int r){
    for(int i=l+1;i<=r;i++){
        if(arr[i]>=arr[l]){
            for(int j=i;j<r;j++){
                if(arr[j]<arr[l]){
                    return -1;
                }
            }
            if(judgeright(arr,l+1,i-1)==-1){
                return -1;
            }
            if(judgeright(arr,i+1,r)==-1){
                return -1;
            }
        }
        if(i==r){
            if(judgeright(arr,l+1,r)==-1){
                return -1;
            }
        }
    }
    return 1;
}
int judgeminnor(int arr[],int l,int r){
    for(int i=l;i<r;i++){
        if(arr[i]<arr[r]){
            for(int j=i;j<r;j++){
                if(arr[j]>=arr[l]){
                    return -1;
                }
            }
            if(judgeminnor(arr,l+1,i-1)==-1){
                return -1;
            }
            if(judgeright(arr,i+1,r)==-1){
                return -1;
            }
        }
        if(i==r){
            if(judgeminnor(arr,l+1,r-1)==-1){
                return -1;
            }
        }
    }
    return 1;
}
class Node{
    public:
    int data;
    Node *left;
    Node *right;
    Node(int data){
        this->data = data;
        this->left = NULL;
        this->right = NULL;
    }
};
class Tree{
    public:
    Node *root;
    Tree(){
        root = NULL;
    }
    void insert(int data){
        if(root==NULL){
            root = new Node(data);
            return;
        }
        Node *temp = root;
        while(temp!=NULL){
            if(data<temp->data){
                if(temp->left==NULL){
                    temp->left = new Node(data);
                    return;
                }
                temp = temp->left;
            }
            else{
                if(temp->right==NULL){
                    temp->right = new Node(data);
                    return;
                }
                temp = temp->right;
            }
        }
    }
    void laterorder(Node *root){
        if(root==NULL){
            return;
        }
        laterorder(root->left);
        laterorder(root->right);
        cout << root->data << " ";
    }
};
int main(){
    #ifndef ONLINE_JUDGE
	freopen("in.in", "r", stdin);
	freopen("out.out", "w", stdout);
    #endif
    int n;
    cin >> n;
    int arr[100];
    for(int i=0;i<n;i++){
        cin >> arr[i];
    }
    if(judgeright(arr,0,n-1)==1){
        cout << "YES" << endl;
        Tree tree;
        for(int i=0;i<n;i++){
            tree.insert(arr[i]);
        }
        tree.laterorder(tree.root);
    }
    else{
        if(judgeminnor(arr,0,n-1)==1){
            cout << "YES" << endl;
            Tree tree;
            for(int i=0;i<n;i++){
                tree.insert(arr[i]);
            }
            tree.laterorder(tree.root);
        }
        else{
            cout << "NO" << endl;
        }
    }
    return 0;
}