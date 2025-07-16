// 带头结点的单链表
#include <iostream>
using namespace std;

class cnode;
class clist
{
    cnode *head;
    friend void display(const clist &l);

public:
    clist();
    cnode *find(int i);
    int insert(int i, int e);
    int del(int i);
    // this , *this
    void print();
    int getlength();
    ~clist();
    void createlist(int num[], int n);
};

class cnode
{
    int data;    // 数据
    cnode *next; // next指针，下一个结点
    static int total;
    friend void display(const clist &l);
    friend int clist::insert(int i, int e);
    friend class clist;

public:
    cnode(int e = 0, cnode *p = nullptr);
    int getdata();
    cnode *getnext();
    void setnext(cnode *p);
    static int gettotal();
    ~cnode();
};


int cnode::gettotal()
{
    return total;
}
    
cnode::cnode(int e, cnode *p) : data(e), next(p) 
{
    total++;
}
int cnode::getdata()
{
    return data;
}
cnode *cnode::getnext()
{
    return next;
}
void cnode::setnext(cnode *p)
{
    next = p;
}
cnode::~cnode()
{
    total--;
}

clist::clist()
{
    head = new cnode;
}

cnode *clist::find(int i)
{
    if (i < 0)
        return nullptr;
    int k = 0;
    cnode *p = head;
    while (p && k < i)
    {
        k++;
        p = p->next;
    }
    return p; // k=i,nullptr
}
int clist::insert(int i, int e)
{
    cnode *p = find(i - 1);
    if (p)
    {
        cnode *s = new cnode(e, p->next);
        p->next = s;
        return 1; // 插入成功
    }
    return 0; // 插入失败
}

int clist::del(int i)
{
    cnode *p = find(i - 1);

    if (p && p->next)
    {
        cnode *q = p->next;
        p->next = q->next;
        delete q;
        return 1; // 删除成功
    }
    return 0; // 删除失败
}

// this , *this
void clist::print()
{
    cnode *p = head->next;
    while (p)
    {
        cout << p->data << " ";
        p = p->next;
    }
    cout << endl;
}
int clist::getlength()
{
    cnode *p = head;
    int k = 0;
    while (p->getnext())
    {
        k++;
        p = p->getnext();
    }
    return k;
}
clist::~clist()
{
    cnode *p = head;
    while (p)
    {
        cnode *q = p;
        // delete q;
        p = p->getnext();
        delete q;
    }
    head = nullptr;
}
// 尾插创建单链表
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
void clist::createlist(int num[], int n)
{
    for (int i = 0; i < n; i++) // O(n)
    {
        cnode *s = new cnode(num[i], head->getnext());
        head->setnext(s);
    }
}
// 输出l单链表
void display(const clist &l)
{
    cnode *p = l.head->next;
    while (p)
    {
        cout << p->data << " ";
        p = p->next;
    }
    cout << endl;
}
int cnode::total = -1;

int main()
{
    clist l;
    cnode a;
    int num[] = {10, 20, 30};
    l.createlist(num, 3);
    l.print();
    cout << cnode::gettotal() << endl;
    cout << a.gettotal() << endl;

    l.del(1);
    l.print();
    cout << cnode::gettotal() << endl;

    // cout << l.getlength() << endl;
    system("pause");
}
