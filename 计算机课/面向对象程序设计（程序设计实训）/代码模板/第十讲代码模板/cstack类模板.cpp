#include <iostream>
#include <cstring>
using namespace std;

template <class T>
class cstack
{
    int n;
    T *data;
    int itop;

public:
    cstack(int nval);
    void push(T e);
    T top();
    void pop();
    bool empty();
};

template <class T>
cstack<T>::cstack(int nval) : n(nval), itop(0)
{
    data = new T[n];
}

template <class T>
void cstack<T>::push(T e)
{
    data[itop++] = e;
}

template <class T>
T cstack<T>::top()
{
    return data[itop - 1];
}

template <class T>
void cstack<T>::pop()
{
    itop--;
}

template <class T>
bool cstack<T>::empty()
{
    return itop == 0;
}
int main()
{
    int t;
#ifndef ONLINE_JUDGE
    freopen("in.txt", "r", stdin);
    freopen("out.txt", "w", stdout);
#endif
    cstack<int> s(10);
    cstack<string> ss(10);
    string type;

    cin >> t;
    while (t--)
    {
        cin >> type;
        if (type == "I")
        {
            int e;
            for (int i = 0; i < 3; i++)
            {
                cin >> e;
                s.push(e);
            }
        }
        else if (type == "S")
        {
            string e;
            for (int i = 0; i < 3; i++)
            {
                cin >> e;
                ss.push(e);
            }
        }
    }
    while (!s.empty())
    {
        cout << s.top() << " ";
        s.pop();
    }
    cout << endl;
    while (!ss.empty())
    {
        cout << ss.top() << " ";
        ss.pop();
    }
    cout << endl;
    return 0;
}

/*
3
I 10 20 30
S abc hello c++
I 4 5 6
*/