//带头结点的单链表
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
    clist()
    {
        head = new cnode;
    }

    cnode *find(int i)
    {
        if(i<0)
            return nullptr;
        int k = 0;
        cnode *p = head;
        while(p && k<i)
        {
            k++;
            p = p->getnext();
        }
        return p;  //k=i,nullptr
    }
    int insert(int i, int e)
    {
        cnode *p = find(i-1);
        if(p)
        {
            cnode *s = new cnode(e,p->getnext());
            p->setnext(s);
            return 1;   //插入成功
        }
        return 0;   //插入失败
    }

    int del(int i)
    {
        cnode *p = find(i-1);

        if(p && p->getnext())
        {
            cnode *q = p->getnext();
            p->setnext(q->getnext());
            delete q;
            return 1;    //删除成功
        }
        return 0;    //删除失败
    }

    //this , *this
    void print()
    {
        cnode *p = head->getnext();
        while(p)
        {
            cout << p->getdata() << " ";
            p = p->getnext();
        }
        cout << endl;
    }
    int getlength()
    {
        cnode *p = head;
        int k = 0;
        while(p->getnext())
        {
            k++;
            p = p->getnext();
        }
        return k;
    }
    ~clist()
    {
        cnode *p = head;
        while(p)
        {
            cnode *q = p;
            //delete q;
            p = p->getnext();
            delete q;
        }
        head = nullptr;
    }
    //尾插创建单链表
    /*void createlist(int num[],int n)
    {
        cnode *tail = head;
        for(int i=0; i<n; i++)  //O(n)
        {
            cnode *s = new cnode(num[i]);
            tail->setnext(s);
            tail = s;
        }
    }*/
    void createlist(int num[],int n)
    {
        for(int i=0; i<n; i++)  //O(n)
        {
            cnode *s = new cnode(num[i],head->getnext());
            head->setnext(s);
        }
    }
};

int main()
{
    clist l;
    int num[] = {10,20,30};
    /*for(int i=0; i<3; i++)
        l.insert(i+1,num[i]);*/
    l.createlist(num,3);
    l.print();
    l.del(1);
    l.print();
    if(l.del(10))
        l.print();
    else
        cout << "error!" << endl;

    cout << l.getlength() << endl;
    system("pause");
}
