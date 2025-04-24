#include <bits/stdc++.h>
using namespace std;

// const int N = 1e6 + 10;
const int N = 10;
#define ll long long
struct student{
  int id;
  int score;      
};
bool cmp(student a,student b){
    if(a.score==b.score){
        return a.id<b.id;
    }
    return a.score>b.score;
}
int main(){
    #ifndef ONLINE_JUDGE
    freopen("in.in", "r", stdin);
    freopen("out.out", "w", stdout);
    #endif
    int n,m;
    cin>>n>>m;
    student stu[N];
    for(int i=0;i<n;i++){
        cin>>stu[i].id;
        cin>>stu[i].score;
    }
    sort(stu,stu+n,cmp);
    int line=(m*3)/2;
    int linescore=stu[line-1].score;
    for(int i=line;i<n;i++){
        if(stu[i].score>=linescore){
            line++;
        }
        else{
            break;
        }
    }
    cout<<linescore<<" "<<line<<endl;
    for(int i=0;i<line;i++){
        cout<<stu[i].id<<" ";
        cout<<stu[i].score<<endl;
    }
    return 0;
}