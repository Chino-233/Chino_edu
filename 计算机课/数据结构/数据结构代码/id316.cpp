#include <iostream>
using namespace std;
void quicklysort(int arr[],int n,int l,int r){
    if(l>=r){
        return;
    }
    int i=l;
    int j=r;
    int key=arr[l];
    while(i<j){
        while(i<j&&arr[j]>=key){
            j--;
        }
        arr[i]=arr[j];
        while(i<j&&arr[i]<=key){
            i++;
        }
        arr[j]=arr[i];
    }
    arr[i]=key;
    for(int i=0;i<n;i++){
        cout<<arr[i];
        if(i!=n-1){
            cout<<" ";
        }
        else{
            cout<<endl<<key;
            cout<<endl;
        }
    }
    quicklysort(arr,n,l,i-1);
    quicklysort(arr,n,i+1,r);
    
}
void quick(int arr[],int high,int low,int n){
    if(high<=low){
        return;
    }
    int core=arr[low];
    int hightemp=high;
    int lowtemp=low;
    while (high>low)
    {
        while (high>low&&arr[high]>=core)
        {
            high--;
        }
        arr[low]=arr[high];
        while (low<high&&arr[low]<=core)
        {
            low++;
        }
        arr[high]=arr[low];
    }
    arr[high]=core;
    for(int i=0;i<n;i++){
        cout<<arr[i];
        if(i!=n-1){
            cout<<" ";
        }
        else{
            //cout<<endl<<core;
            cout<<endl;
        }
    }
    quick(arr,high-1,lowtemp,n);
    quick(arr,hightemp,high+1,n);
    
}
void qucik(int arr[],int low,int high,int n){
    if(high<=low){
        return;
    }
    int l=low;
    int h=high;
    int core=arr[low];
    while (l<h)
    {
        while (l<h&&arr[h]>=core)
        {
            h--;
        }
        arr[l]=arr[h];
        while (l<h&&arr[l]<=core)
        {
            l++;
        }
        arr[h]=arr[l];
    }
    arr[h]=core;
    for(int i=0;i<n;i++){
        cout<<arr[i];
        if(i!=n-1){
            cout<<" ";
        }
        else{
            cout<<endl;
        }
    }
    qucik(arr,low,l-1,n);
    qucik(arr,l+1,high,n);
}
int main(){
    #ifndef ONLINE_JUDGE
	freopen("in.in", "r", stdin);
	freopen("out.out", "w", stdout);
    #endif
    int t;
    cin>>t;
    while (t--)
    {
        int n;
        cin>>n;
        int arr[100]={0};
        for(int i = 0; i < n; i++){
            cin>>arr[i];
        }
        //quick(arr,n-1,0,n);
        qucik(arr,0,n-1,n);
        //quicklysort(arr,n,0,n-1);
        cout<<endl;
    }
    return 0;
}