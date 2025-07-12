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
/*二叉树可以采用数组的方法进行存储，把数组中的数据依次自上而下,自左至右存储到二叉树结点中，一般二叉树与完全二叉树对比，比完全二叉树缺少的结点就在数组中用0来表示。，如下图所示



从上图可以看出，右边的是一颗普通的二叉树，当它与左边的完全二叉树对比，发现它比完全二叉树少了第5号结点，所以在数组中用0表示，同样它还少了完全二叉树中的第10、11号结点，所以在数组中也用0表示。

结点存储的数据均为非负整数


输入

第一行输入一个整数t，表示有t个二叉树

第二行起，每行输入一个数组，先输入数组长度，再输入数组内数据，每个数据之间用空格隔开，输入的数据都是非负整数

连续输入t行*/
class Tree{
public:
    int arr[1000];
    void CreateTree(int *arr, int n){
        for(int i=0;i<1000;i++){
            arr[i]=0;
        }
        for (int i = 1; i < n+1; i++) {
            cin >> arr[i];
        }
    }
    void PreOrder(int &i){
        if (arr[i] == 0) {
            return;
        }
        cout << arr[i] << " ";
        int temp = 2 * i;
        PreOrder(temp);
        temp++;
        PreOrder(temp);
    }
};
int main(){
    #ifndef ONLINE_JUDGE
	freopen("in.in", "r", stdin);
	freopen("out.out", "w", stdout);
    #endif
    Tree tree;
    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        tree.CreateTree(tree.arr, n);
        int i=1;
        tree.PreOrder(i);
        cout << endl;
    }
    return 0;
}