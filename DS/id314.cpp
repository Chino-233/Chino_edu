#include <iostream>
using namespace std;
void dirrectsort(int arr[], int n){
    int con = 0;
    for(int endindex = 1;endindex<n;endindex++)
    {
        int temp=arr[endindex];
        //cout<<"temp: "<<temp<<endl;
        int con=endindex-1;
        for(;con>=0;con--){
            if(temp<arr[con]){
                arr[con+1]=arr[con];
            }else{
                break;
            }
        }
        arr[con+1]=temp;
        for(int i=0;i<n;i++){
            cout<<arr[i];
            if(i!=n-1){
                cout<<" ";
            }
            else{
                cout<<endl;
            }
        }
        //cout<<endl;
    }
}
int main(){
    #ifndef ONLINE_JUDGE
	freopen("in.in", "r", stdin);
	freopen("out.out", "w", stdout);
    #endif
    int n;
    int arr[100];
    cin>>n;
    for(int i = 0; i < 100; i++){
        cin>>arr[i];
    }
    dirrectsort(arr, n);
    return 0;
}