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
class myString {
private:
    string mainstr;
    int size;
public:
    myString();
    ~myString();
    void SetVal(string sp);
};

myString::myString() {
    size = 0;
    mainstr = "";
}

myString::~myString() {
    size = 0;
    mainstr = "";
}

void myString::SetVal(string sp) {
    mainstr = "";
    mainstr.assign(sp);
    size = mainstr.length();
}

void GetNext(string p, int next[]) {
    int i = 0, j = -1;
    next[0] = -1;
    while (i < p.length() - 1) {
        if (j == -1 || p[i] == p[j]) {
            i++;
            j++;
            next[i] = j;
        } else {
            j = next[j];
        }
    }
}

int KMPFind(string mainstr, string p, int pos, int next[]) {
    int i = pos, j = 0;
    while ((i < mainstr.length()) && (p.length() - j > 0)) {
        if (j == -1 || mainstr[i] == p[j]) {
            i++;
            j++;
        } else {
            j = next[j];
        }
    }
    if (j == p.length()) {
        return i - j;
    } else {
        return -1;
    }
}

int main() {
    #ifndef ONLINE_JUDGE
	freopen("in.in", "r", stdin);
	freopen("out.out", "w", stdout);
    #endif
    int t;
    cin >> t;
    while (t--) {
        string mainstr;
        cin >> mainstr;
        myString ms;
        ms.SetVal(mainstr);
        int n = mainstr.length();
        int maxLen = -1;
        for (int i = 0; i < n; i++) {
            int lengthtemp=mainstr.length();
            for (int j=1;j<lengthtemp-i+1;j++) {
                string pattern = mainstr.substr(i,j);
                //cout<<pattern<<endl;
                int *next = new int[pattern.length()];
                GetNext(pattern, next);
                int pos = KMPFind(mainstr, pattern, 0, next);
                if(pos!=-1){
                    pos=KMPFind(mainstr,pattern,pos+1,next);
                }
                else{
                    pos=-1;
                }
                if (pos != -1) {
                    int temp=pattern.length();
                    //cout<<"i:"<<i<<"j:"<<j<<"temp:"<<temp<<endl;
                    maxLen = max(maxLen, temp);
                }
                delete[] next;
            }
        }
        cout << maxLen << endl;
    }
    return 0;
}
