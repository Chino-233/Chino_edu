#include <iostream>
using namespace std;

class cnode
{
    int data;   //数据
    cnode *next; //next指针，下一个结点
public:
    cnode(int e = 0, cnode *p = nullptr):data(e),next(p){}
    int getdata()
    {
        return data;
    }
    cnode *getnext()
    {
        return next;
    }
    void setnext(cnode *p)
    {
        next = p;
    }    
};

class clist
{
    cnode *head;
public:
    clist():head(nullptr){}

    //insert in head
    /*void createlist(int *num,int n)
    {
        for(int i=0; i<n; i++)
        {
            cnode *s = new cnode(num[i],head);
            head = s;
        }
    }*/
    //insert in tail
    void createlist(int *num,int n)
    {
        cnode *tail;
        for(int i=0; i<n; i++)
        {
            cnode *s = new cnode(num[i]);
            if(!head)
                head = s;
            else
                tail->setnext(s);
            tail = s;
        }
    }

    //this , *this
    void print()
    {
        cnode *p = head;
        while(p)
        {
            cout << p->getdata() << endl;
            p = p->getnext();
        }
    }
};

int main()
{
    clist l;
    int num[] = {10,20,30};
    l.createlist(num,3);
    l.print();
    system("pause");
}
