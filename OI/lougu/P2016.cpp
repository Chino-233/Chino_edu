#include <bits/stdc++.h>
using namespace std;
//  const int N = 1e6 + 10;
  const int N = 10;
#define ll long long
class Node{
public:
    ll id;
    ll parent=0; // 父节点
    ll child[N];
    ll childnum;
    ll dislook=0; // 距离
    ll used=0;
};
bool cmp(Node po1,Node po2){
    return po1.dislook<po2.dislook;
}
int main(){
    #ifndef ONLINE_JUDGE
	freopen("in.in", "r", stdin);
	freopen("out.out", "w", stdout);
    #endif
    ll n;
    cin>>n;
    Node node[N];
    map<ll,Node> mp;
    for(ll i=0;i<n;i++){
        cin>>node[i].id;
        ll m;
        cin>>m;
        node[i].childnum=m;
        for(ll j=0;j<m;j++){
            ll k;
            cin>>k;
            node[k].parent=i;
            node[i].child[j]=k;
            node[k].dislook++;
            node[i].dislook++;
        }        
        
    }
    ll allnum=0;
    ll maxdislook=1;
    while (maxdislook)
    {
        maxdislook=0;
        //sort(node,node+n,cmp);
        ll max=0;
        for(ll i=0;i<n;i++){
            if(node[i].dislook>maxdislook){
                max=i;
                maxdislook=node[i].dislook;
            }
        }
        if(maxdislook==0){
            break;
        }
        allnum++;
        //cout<<allnum<<":"<<max<<endl;
        node[max].dislook=0;
        for(ll i=0;i<node[max].childnum;i++){
            if(!node[node[max].child[i]].used){
                node[node[max].child[i]].used=1;
                node[node[max].child[i]].dislook--;
                node[max].dislook--;
            }
        }
        if(!node[max].used){
            node[max].used=1;
            node[node[max].parent].dislook--;
        }
    }
    cout<<allnum;
    return 0;
}