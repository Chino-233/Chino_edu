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
void makehash(int arr[],int hash[],int hashstone[],int n,int m){
    for(int i=0;i<n;i++){
        int temp=arr[i]%11;
        if(hash[temp]>=1){
            while (hash[temp]>=1)
            {
                temp=(temp+1)%m;
            }
            hash[temp]++;
            hashstone[temp]=arr[i];
        }
        else{
            hash[temp]++;
            hashstone[temp]=arr[i];
        }
    }
    for(int i=0;i<m;i++){
        if(hash[i]==1){
            cout<<hashstone[i];
            if(i!=m-1){
                cout<<" ";
            }
        }
        else{
            cout<<"NULL"<<" ";
        }
    }
    cout<<endl;
}
void makehashbin(int arr[],int hash[],int hashstone[],int n,int m){
    for(int i=0;i<n;i++){
        int temp=arr[i]%11;
        int pownum=1;
        int findcut=0;
        if(hash[temp]==0){
            hash[temp]=1;
            hashstone[temp]=arr[i];
            continue;
        }
        int temp1=temp;
        while (hash[temp]==1)
        {
            temp=(temp1+(int)pow(pownum,2))%m;
            findcut++;
            if(hash[temp]==0){
                hash[temp]=1;
                hashstone[temp]=arr[i];
                break;
            }
            else if(findcut>=m){
                break;
            }
            temp=(temp1-(int)pow(pownum,2)+m*m)%m;
            findcut++;
            if(hash[temp]==0){
                hash[temp]=1;
                hashstone[temp]=arr[i];
                break;
            }
            else if(findcut>=m){
                break;
            }
            pownum++;
        }        
    }
    
    for(int i=0;i<m;i++){
        if(hash[i]==1){
            cout<<hashstone[i];
            if(i!=m-1){
                cout<<" ";
            }
        }
        else{
            cout<<"NULL"<<" ";
        }
    }
    cout<<endl;
}
void findhash(int hash[],int hashstone[],int m,int x){
    int temp=x%11;
    int findcut=0;
    if(hash[temp]==0){
        cout<<"0 1"<<endl;
        return;
    }
    int temp1=temp;
    if(hash[temp]==1){
        if(hashstone[temp]==x){
            cout<<"1 1 "<<temp+1<<endl;
            return;
        }
        findcut++;
        while (hash[temp]==1 && hashstone[temp]!=x)
        {
            temp=(temp+1)%m;
            findcut++;
            if(temp==temp1){
                break;
            }
        }
        if(temp==temp1||hash[temp]==0){
            cout<<"0 "<<findcut<<endl;
            return;
        }
        else{
            cout<<"1 "<<findcut<<" "<<temp+1<<endl;
            return;
        }
        
    }
}
void findhashbin(int hash[],int hashstone[],int m,int x){
    int temp=x%11;
    int findcut=0;
    if(hash[temp]==0){
        cout<<"0 1"<<endl;
        return;
    }
    int temp1=temp;
    if(hash[temp]==1){
        if(hashstone[temp]==x){
            cout<<"1 1 "<<temp+1<<endl;
            return;
        }
        findcut++;
        int pownum=1;
        int temp1=temp;
        while (hashstone[temp]!=x)
        {
            temp=(temp1+(int)pow(pownum,2)+m*m)%m;
            findcut++;
            if(hash[temp]==0||findcut>=m){
                cout<<"0 "<<findcut<<endl;
                break;
            }
            else if(hashstone[temp]==x){
                cout<<"1 "<<findcut<<" "<<temp+1<<endl;
                return;
            }
            temp=(temp1-(int)pow(pownum,2)+m*m)%m;
            findcut++;
            if(hash[temp]==0||findcut>=m){
                cout<<"0 "<<findcut<<endl;
                break;
            }
            else if(hashstone[temp]==x){
                cout<<"1 "<<findcut<<" "<<temp+1<<endl;
                return;
            }
            pownum++;
        }        
    }
}
int main(){
    #ifndef ONLINE_JUDGE
	freopen("in.in", "r", stdin);
	freopen("out.out", "w", stdout);
    #endif
    int t;
    cin>>t;
    while(t--){
        int m,n;
        cin>>m>>n;
        int arr[100];
        for(int i=0;i<n;i++){
            cin>>arr[i];
        }
        int hash[100];
        int hashstone[100];
        memset(hash,0,sizeof(hash));
        makehashbin(arr,hash,hashstone,n,m);
        int k;
        cin>>k;
        for(int i=0;i<k;i++){
            int x;
            cin>>x;
            //findhash(hash,hashstone,m,x);
            findhashbin(hash,hashstone,m,x);
        }
    }
    return 0;
}