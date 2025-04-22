#include <bits/stdc++.h>
using namespace std;
//  const int N = 1e6 + 10;
const int N = 10;
#define ll long long

int main()
{
#ifndef ONLINE_JUDGE
  freopen("in.in", "r", stdin);
  freopen("out.out", "w", stdout);
#endif
  long c[4]={0};
  long now[4]={0};
  for(int i=0;i<3;i++){
    cin>>c[i];
    cin>>now[i];
  }
  for(int flag=0;flag<100;){
    if(now[0]<c[1]-now[1]){
      now[1]+=now[0];
      now[0]=0;
    }
    else{
      now[0]-=(c[1]-now[1]);
      now[1]=c[1];
    }
    flag++;
    if(flag==100){
      break;
    }
    if(now[1]<c[2]-now[2]){
      now[2]+=now[1];
      now[1]=0;
    }
    else{
      now[1]-=(c[2]-now[2]);
      now[2]=c[2];
    }
    flag++;
    if(flag==100){
      break;
    }
    if(now[2]<c[0]-now[0]){
      now[0]+=now[2];
      now[2]=0;
    }
    else{
      now[2]-=(c[0]-now[0]);
      now[0]=c[0];
    }
    flag++;
    if(flag==100){
      break;
    }
  }
  for(int i=0;i<3;i++){
    cout<<now[i]<<endl;
  }
  return 0;
}