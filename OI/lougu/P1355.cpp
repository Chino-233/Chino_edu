#include <bits/stdc++.h>
using namespace std;

// const int N = 1e6 + 10;
const int N = 10;
#define ll long long
int diration(int x1,int y1,int x2,int y2,int x4,int y4){
    if((x2 - x1) * (y4 - y1) - (y2 - y1) * (x4 - x1)>0){
        return 1;
    }
    else if((x2 - x1) * (y4 - y1) - (y2 - y1) * (x4 - x1)==0){
        return 0;
    }
    else{
        return -1;
    }
}
bool pointOnSegment(int x1, int y1, int x2, int y2, int x4, int y4) {
    // 首先判断点是否在直线上（叉积为0）
    if((x2 - x1) * (y4 - y1) - (y2 - y1) * (x4 - x1) != 0) {
        return false;
    }
    
    // 再判断点是否在线段上（而不是延长线上）
    return (x4 >= min(x1, x2) && x4 <= max(x1, x2) &&
            y4 >= min(y1, y2) && y4 <= max(y1, y2));
}
int main(){
    #ifndef ONLINE_JUDGE
    freopen("in.in", "r", stdin);
    freopen("out.out", "w", stdout);
    #endif
    int x1=0,y1=0;
    int x2=0,y2=0;
    int x3=0,y3=0;
    int x4=0,y4=0;
    scanf("(%d,%d)\n(%d,%d)\n(%d,%d)\n(%d,%d)", 
        &x1, &y1, &x2, &y2, &x3, &y3, &x4, &y4);
    if(x1==x4&&y1==y4 || x2==x4&&y2==y4 || x3==x4&&y3==y4){
            cout<<4<<endl;
    }
    else{
    if(pointOnSegment(x1,y1,x2,y2,x4,y4) || pointOnSegment(x3,y3,x1,y1,x4,y4) || pointOnSegment(x2,y2,x3,y3,x4,y4)){
        cout<<3<<endl;
    }
    else{
        int z=0;int f=0;
        if(diration(x1,y1,x2,y2,x4,y4)==1){
            z++;
        }
        if(diration(x1,y1,x2,y2,x4,y4)==-1){
            f++;
        }
        if(diration(x3,y3,x1,y1,x4,y4)==1){
            z++;
        }
        if(diration(x3,y3,x1,y1,x4,y4)==-1){
            f++;
        }
        if(diration(x2,y2,x3,y3,x4,y4)==1){
            z++;
        }
        if(diration(x2,y2,x3,y3,x4,y4)==-1){
            f++;
        }
        if(z==3||f==3){
            cout<<1<<endl;
        }
        else if(z>0&&f>0){
            cout<<2<<endl;
        }
    }
    }
    return 0;
}