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
    while ((i<mainstr.length())&&(p.length()-j>0)) {
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
        string mainstr, pattern,replace;
        cin >> mainstr >> pattern>>replace;
        cout<<mainstr<<endl;
        myString ms;
        ms.SetVal(mainstr);
        int L = pattern.length();
        int *next = new int[L];
        GetNext(pattern, next);
        int pos = KMPFind(mainstr, pattern, 0, next);
        if (pos != -1) {
            mainstr.replace(pos, L, replace);
        }
        cout<<mainstr<<endl;
        delete[] next;
    }

    return 0;
}
