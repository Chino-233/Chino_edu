#include <bits/stdc++.h>
using namespace std;

const int N = 5010; // 大数需要更大的数组

// 高精度加法函数
string add(string a, string b) {
    string res = "";
    int carry = 0;
    
    // 让a始终是较长的数
    if (a.length() < b.length()) swap(a, b);
    
    // 对b补0使两数长度相同
    b = string(a.length() - b.length(), '0') + b;
    
    // 从低位到高位逐位相加
    for (int i = a.length() - 1; i >= 0; i--) {
        int sum = (a[i] - '0') + (b[i] - '0') + carry;
        carry = sum / 10;
        res = char(sum % 10 + '0') + res;
    }
    
    // 处理最高位的进位
    if (carry) res = '1' + res;
    
    return res;
}

int main(){
    #ifndef ONLINE_JUDGE
    freopen("in.in", "r", stdin);
    freopen("out.out", "w", stdout);
    #endif
    
    int n;
    cin >> n;
    
    string arr[N];
    arr[0] = "1";
    arr[1] = "2";
    
    for(int i = 2; i < n; i++){
        arr[i] = add(arr[i-1], arr[i-2]);
    }
    
    cout << arr[n-1] << endl;
    return 0;
}