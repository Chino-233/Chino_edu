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
/*题目描述

组队列是队列结构中一种常见的队列结构，在很多地方有着广泛应用。组队列是是指队列内的元素分组聚集在一起。组队列包含两种命令：

1、 ENQUEUE，表示当有新的元素进入队列，首先会检索是否有同一组的元素已经存在，如果有，则新元素排在同组的最后，如果没有则插入队列末尾。

2、 DEQUEUE，表示队列头元素出队

3、 STOP，停止操作

 


输入

第1行输入一个t(t<=10)，表示1个队列中有多少个组

第2行输入一个第1组的元素个数和数值

第3行输入一个第2组的元素个数和数值

以此类推输入完t组以定义同组元素之后，开始输入多个操作命令(<200)，对空的组队列进行操作，例如输入ENQUEUE 100，表示把元素100插入队列


输出

DEQUEUE出队的元素*/
int main(){
    #ifndef ONLINE_JUDGE
	freopen("in.in", "r", stdin);
	freopen("out.out", "w", stdout);
    #endif
    //int clock=0;
    int flag=0;
    int t;
    cin >> t;
    vector<vector<int>> v;
    for(int i = 0; i < t; i++){
        int n;
        cin >> n;
        vector<int> tmp;
        for(int j = 0; j < n; j++){
            int x;
            cin >> x;
            tmp.push_back(x);
        }
        v.push_back(tmp);
    }
    vector<queue<int>> vq;
    int vqpos[10];
    for(int i = 0; i < 10; i++){
        vqpos[i] = -1;
    }
    string s;
    while(cin >> s){
        if(s == "STOP") break;
        if(s == "ENQUEUE"){
            int x;
            cin >> x;
            int pos = -1;
            for(int i = 0; i < v.size(); i++){
                if(find(v[i].begin(), v[i].end(), x) != v[i].end()){
                    pos = i;
                    break;
                }
            }
            if(vq.empty()){
                vqpos[pos] = 0;
                queue<int> q;
                q.push(x);
                vq.push_back(q);
            }
            else{
                if(vqpos[pos] == -1){
                    vqpos[pos] = vq.size();
                    queue<int> q;
                    q.push(x);
                    vq.push_back(q);
                }
                else{
                    vq[vqpos[pos]].push(x);
                }
            }

        }
        else{
            //cout<<"第"<<++clock<<"次出队元素为：";
            //if(clock==7){
            //    cout<<"";
            //}
            for(int i = 0; i < vq.size(); i++){
                if(!vq[i].empty()){
                    if(flag){
                        cout<<" ";
                    }
                    else{
                        flag = 1;
                    }
                    cout << vq[i].front();
                    vq[i].pop();
                    while(vq[0].empty()){
                        for(int j=0;j<vq.size()-1;j++){
                            vq[j] = vq[j+1];
                        }
                        vq.pop_back();
                        for(int j=0;j<10;j++){
                            if(vqpos[j] != -1){
                                vqpos[j]--;
                            }
                        }
                        if(vq.empty()){
                            break;
                        }
                    }
                    break;
                }
            }
        }
    }
    cout<<endl;

    return 0;
}