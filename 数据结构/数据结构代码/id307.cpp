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
struct contry{
    int id;
    int gold;
    int brand;
    int people;
    double gold_per_people;
    double brand_per_people;
};
bool cop_gold(contry a, contry b){
    return a.gold > b.gold;
}
bool cop_brand(contry a, contry b){
    return a.brand > b.brand;
}
bool cop_gold_per_people(contry a, contry b){
    return a.gold_per_people > b.gold_per_people;
}
bool cop_brand_per_people(contry a, contry b){
    return a.brand_per_people > b.brand_per_people;
}
int main(){
    #ifndef ONLINE_JUDGE
	freopen("in.in", "r", stdin);
	freopen("out.out", "w", stdout);
    #endif
    int n, m;
    cin >> n >> m;
    contry savearr[300];
    for(int i=0;i<n;i++){
        int id, gold, brand, people;
        cin>> gold >> brand >> people;
        contry node;
        node.id = i;
        node.gold = gold;
        node.brand = brand;
        node.people = people;
        node.gold_per_people = (double)(gold) / (double)(people);
        node.brand_per_people = (double)(brand) / (double)(people);
        savearr[i] = node;
    }
    contry max_gold[300];
    contry max_brand[300];
    contry max_gold_per_people[300];
    contry max_brand_per_people[300];
    contry temp[300];
    for(int i=0;i<n;i++){
        temp[i] = savearr[i];   
    }
    sort(temp, temp+n, cop_gold);
    for(int i=0;i<n;i++){
        max_gold[i] = temp[i];
        temp[i]=savearr[i];
    }
    sort(temp, temp+n, cop_brand);
    for(int i=0;i<n;i++){
        max_brand[i] = temp[i];
        temp[i]=savearr[i];
    }
    sort(temp, temp+n, cop_gold_per_people);
    for(int i=0;i<n;i++){
        max_gold_per_people[i] = temp[i];
        temp[i]=savearr[i];
    }
    sort(temp, temp+n, cop_brand_per_people);
    for(int i=0;i<n;i++){
        max_brand_per_people[i] = temp[i];
    }
    for(int i=0;i<n;i++){
        //cout<<max_gold[i].id<<" "<<max_brand[i].id<<" "<<max_gold_per_people[i].id<<" "<<max_brand_per_people[i].id<<endl;
    }
    for(int i=0;i<m;i++){
        int id;
        cin >> id;
        for(int j=0;j<n;j++){
            if(max_gold[j].id == id){
                cout<<j+1<<":"<<1<<" ";
                break;
            }
            else if(max_brand[j].id == id){
                cout<<j+1<<":"<<2<<" ";
                break;
            }
            else if(max_gold_per_people[j].id == id){
                cout<<j+1<<":"<<3<<" ";
                break;
            }
            else if(max_brand_per_people[j].id == id){
                cout<<j+1<<":"<<4<<" ";
                break;
            }
        }
    }
    return 0;
}