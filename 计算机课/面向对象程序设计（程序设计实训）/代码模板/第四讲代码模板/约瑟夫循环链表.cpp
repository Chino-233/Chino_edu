//约瑟夫环，循环链表
#include <iostream>
using namespace std;

//结点类
class cnode
{
    int data;
    cnode *next;
public:
    cnode(int e = 0,cnode *nextval = nullptr);
    int getdata();
    cnode *getnext();
    void setdata(int e);
    void setnext(cnode *next);   
};

//约瑟夫求解类
class joseph
{
    cnode *head;
public:
    joseph(int n);
    void number(int s,int k);
    cnode *find(int s);
    void display();
};

cnode::cnode(int e, cnode *nextval):data(e),next(nextval)
{
}
int cnode::getdata()
{
    return data;
}
void cnode::setdata(int e)
{
    data = e;
}
cnode *cnode::getnext()
{
    return next;
}
void cnode::setnext(cnode *next)
{
    this->next = next;
}

joseph::joseph(int n)
{
    head = nullptr;
    cnode *tail;
    for(int i=1; i<=n; i++)
    {
        cnode *s = new cnode(i);
        if(!head)
            head = s;
        else
            tail->setnext(s);
        tail = s;
        s->setnext(head);
    }
}

void joseph::display()
{
    cnode *p = head;

    while(p)
    {
        cout << p->getdata() << " ";
        if(p->getnext() == head)
            break;
        p = p->getnext();
    } 
    cout << endl;   
}

cnode *joseph::find(int s)
{
    cnode *p = head;

    //走到最后一个结点, 计数第0个结点
    int k = 0;
    while(p->getnext() != head)
        p = p->getnext();
    
    while(p && k<s)
    {
        p = p->getnext();
        k++;
    }
    return p;
}

void joseph::number(int s,int k)
{
    cnode *p = find(s-1);   //找到第s-1个结点
    
    //开始报数，每次报数删除p之后结点
    while(p->getnext() != p)   //p->getnext()==p，只有1个结点
    {
        for(int i=1; i<k; i++)
            p = p->getnext();
        cnode *q = p->getnext();
        cout << q->getdata() << " ";
        p->setnext(q->getnext());
        delete q;
    }
    cout << p->getdata() << endl;
}

int main()
{
    int t, n, s, k;

    while(cin >> n >> s >> k)
    {
        joseph jose(n);
        jose.display();
        jose.number(s,k);
    }
    return 0;
}


