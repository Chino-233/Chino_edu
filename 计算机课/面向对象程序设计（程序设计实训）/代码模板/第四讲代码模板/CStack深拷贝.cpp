#include <iostream>
#include <stack>
using namespace std;

#define MAXSIZE 5

class CStack
{
    int *data;
    int itop;
public:
    CStack():itop(-1)
    {
        data = new int[MAXSIZE];
    }
    
    //this <- rhs
    CStack(const CStack &rhs)   
    {
        //cout << data << " " << rhs.data << endl;
        //data = new int[MAXSIZE];
        //memcpy(data, rhs.data, sizeof(int)*MAXSIZE);
        data = rhs.data;
        itop = rhs.itop;
    }

    bool empty()
    {
        return itop == -1;
    }
    int size()
    {
        return itop+1;
    }
    int top()
    {
        return data[itop];
    }
    void push(int e)
    {
        data[++itop] = e;
    }
    void pop()
    {
        itop--;
    }
    bool full()
    {
        return itop == MAXSIZE-1;
    }
    ~CStack()
    {
        cout << this << " " << data << endl;
        //delete []data;
    }
};

int main()
{
    CStack s;
    cout << &s << endl;
    //stack<int> s;
    int e;

    while(!s.full())   //5
    {
        cin >> e;
        s.push(e);
    }

    CStack s1 = s;  //拷贝构造
    cout << &s1 << endl;
    while(!s1.empty())
    {
        cout << s1.top() << " ";
        s1.pop();
    }
    system("pause");
    
}