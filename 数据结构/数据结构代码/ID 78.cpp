#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <cstring>
#include <algorithm>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <iomanip>
#include <cmath>
#include <map>
using namespace std;
const int N = 1e5 + 10;
typedef double db;
#define ll long long

class Node {
public:
    int data;
    int index;
    Node* next;
    Node(int val = 0):data(val),next(nullptr) {}
    Node(int val,int indexval):data(val),index(indexval),next(nullptr) {}
};

class LinkedList {
protected:
    Node* head;
    int size;
public:
    LinkedList() {
        head = new Node(); // 头结点
    }

    LinkedList(int n) {
        head = new Node(); // 头结点
        Node* current = head;
        for (int i = 0; i < n; i++) {
            int val;
            cin >> val;
            Node* newNode = new Node(val);
            current->next = newNode;
            current = newNode;
        }
        //printList();
    }
    void endinsert(int value,int index){
        Node* newNode = new Node(value,index);
        Node* current = head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newNode;
        //printList();
    }
    void insert(int position,int value) {
        if (position <= 0) {
            cout << "error" << endl;
            return;
        }
        Node* newNode = new Node(value);
        Node* current = head;
        int index = 0;

        while (current != nullptr && index < position - 1) {
            current = current->next;
            index++;
        }

        if (current == nullptr) {
            cout << "error" << endl;
            delete newNode;
            return;
        }

        newNode->next = current->next;
        current->next = newNode;
        //printList();
    }

    void remove(int position) {
        if (position <= 0) {
            cout << "error" << endl;
            return;
        }
        Node* current = head;
        int index = 0;

        while (current->next != nullptr && index < position - 1) {
            current = current->next;
            index++;
        }

        if (current->next == nullptr) {
            cout << "error" << endl;
            return;
        }

        Node* temp = current->next;
        current->next = temp->next;
        delete temp;
        printList();
    }

    void find(int position) {
        if (position <= 0) {
            cout << "error" << endl;
            return;
        }
        Node* current = head->next;
        int index = 1;

        while (current != nullptr && index < position) {
            current = current->next;
            index++;
        }

        if (current == nullptr) {
            cout << "error" << endl;
            return;
        }

        cout << current->data << endl;
    }

    void swapNode(int posa, int posb) {
        if (posa <= 0 || posb <= 0 || posa == posb) {
            cout << "error" << endl;
            return;
        }

        Node* preva = nullptr, *prevb = nullptr, *nodea = head, *nodeb = head;
        int nowposa = 0, nowposb = 0;

        while (nodea != nullptr && nowposa < posa) {
            preva = nodea;
            nodea = nodea->next;
            nowposa++;
        }

        while (nodeb != nullptr && nowposb < posb) {
            prevb = nodeb;
            nodeb = nodeb->next;
            nowposb++;
        }

        if (nodea == nullptr || nodeb == nullptr) {
            cout << "error" << endl;
            return;
        }

        if (preva != nullptr) {
            preva->next = nodeb;
        }
        if (prevb != nullptr) {
            prevb->next = nodea;
        }

        Node* temp = nodea->next;
        nodea->next = nodeb->next;
        nodeb->next = temp;

        if (posa == 1) {
            head->next = nodeb;
        }
        if (posb == 1) {
            head->next = nodea;
        }

        printList();
    }

    void printList() {
        Node* current = head->next;
        while (current != nullptr) {
            cout << current->data << " ";
            current = current->next;
        }
        cout << endl;
    }
    void printpolynomial(){
        Node* current = head;
        while (current != nullptr) {
            if(current->data<0){
                cout <<'('<< current->data <<')';
            }
            else if(current->data==0){
                current = current->next;
                continue;
            }
            else{
                cout << current->data;
            }
            if(current->index<0){
                cout<<"x^"<<'('<<current->index<<')';
            }
            else if(current->index==0){
                current = current->next;
                cout<<" + ";
                continue;
            }
            else{
                cout<<"x^"<<current->index;
            }
            if(current->next!=nullptr){
                Node* temp=current;
                while (temp->next!=nullptr)
                {
                    if(temp->next->data!=0){
                        cout<<" + ";
                        break;
                    }
                    temp=temp->next;
                }
                
                
            }
            //cout<<" + ";
            current = current->next;
        }
        cout << endl;
    }
    int LL_marge(LinkedList *La,LinkedList *Lb) {
        Node *pa = La->head->next;
        Node *pb = Lb->head->next;
        Node *pc = La->head;
        while (pa && pb) {
            if (pa->data <= pb->data) {
                pc->next = pa;
                pc = pa;
                pa = pa->next;
            } else {
                pc->next = pb;
                pc = pb;
                pb = pb->next;
            }
        }
        pc->next = pa ? pa : pb;
        //delete Lb;
        return 0;
    }
    void setpolynomial(){
        int n;
        cin>>n;
        int index,val;
        cin>>val>>index;
        head->data=val;
        head->index=index;
        
        for(int i=0;i<n-1;i++){
            //int index;
            //int val;
            cin>>val;
            cin>>index;
            endinsert(val,index);
        }
    }
    void polynomialadd(LinkedList *list){
        Node* temp1=head;
        Node* prev=nullptr;
        Node* temp2=list->head;
        while(temp2!=nullptr){
            if(temp1==nullptr&&temp2!=nullptr){
                    while (temp2!=nullptr)
                    {
                        prev->next=temp2;
                        prev=prev->next;
                        temp2=temp2->next;
                    }
                    break;
                }
            if(temp1->index==temp2->index){
                temp1->data+=temp2->data;
                prev=temp1;
                temp1=temp1->next;
                temp2=temp2->next;
            }
            else if(temp1->index<temp2->index){
                if(temp1->next!=nullptr){
                    prev=temp1;
                    temp1=temp1->next;
                }
                if(temp1->next==nullptr&&temp2!=nullptr){
                    while (temp2!=nullptr)
                    {
                        temp1->next=temp2;
                        temp1=temp1->next;
                        temp2=temp2->next;
                    }
                    break;
                    
                }
                //temp2=temp2->next;
            }
            else if(temp1->index>temp2->index){
                Node* newNode = new Node(temp2->data,temp2->index);
                newNode->next=temp1;
                if(prev==nullptr){
                    head=newNode;
                    prev=head;
                }
                else{
                    prev->next=newNode;
                    newNode->next=temp1;
                }
                temp2=temp2->next;
                if(temp2==nullptr){
                    break;
                }
            }
            else{
                break;
            }
        }

    }
};

int main(){
    #ifndef ONLINE_JUDGE
    freopen("in.in", "r", stdin);
    freopen("out.out", "w", stdout);
    #endif
    int t;
    cin>>t;
    while (t--)
    {
    LinkedList pol1;
    pol1.setpolynomial();
    pol1.printpolynomial();
    LinkedList pol2;
    pol2.setpolynomial();
    pol2.printpolynomial();
    pol1.polynomialadd(&pol2);
    pol1.printpolynomial();
    }
    return 0;
}