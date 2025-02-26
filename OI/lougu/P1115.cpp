#include <iostream>
using namespace std;
int main(){
    int n;
    cin>>n;
    int sum=0;
    int max=-2147483647;
    for(int i=0;i<n;i++){
        int x;
        cin>>x;
        sum+=x;
        if(sum>max){
            max=sum;
        }
        if(sum<0){
            sum=0;
        }
    }
    cout<<max<<endl;
}