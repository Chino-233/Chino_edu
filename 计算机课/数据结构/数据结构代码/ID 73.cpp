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
    Node* next;
    Node(int val = 0) : data(val), next(nullptr) {}
};

class LinkedList {
private:
    Node* head;
    int size;
public:
    LinkedList() {
        head = new Node(); 
        head->next = head; 
        size = 0;
    }

    LinkedList(int n) {
        head = new Node();
        head->next = head;
        size = 0;
        head->data=1;
        Node* current = head;
        size++;
        for (int i = 0; i < n-1; i++) {
            int val=i+2;
            //cin >> val;
            Node* newNode = new Node(val);
            current->next = newNode;
            current = newNode;
            size++;
        }
        current->next = head; 
    }

    void insert(int position, int value) {
        if (position <= 0 || position > size + 1) {
            cout << "error" << endl;
            return;
        }
        Node* newNode = new Node(value);
        Node* current = head;
        int index = 0;

        while (current->next != head && index < position - 1) {
            current = current->next;
            index++;
        }

        newNode->next = current->next;
        current->next = newNode;
        size++;
        printList();
    }

    void remove(int position) {
        if (position <= 0 || position > size) {
            cout << "error" << endl;
            return;
        }
        Node* current = head;
        int index = 0;

        while (current->next != head && index < position - 1) {
            current = current->next;
            index++;
        }

        Node* temp = current->next;
        current->next = temp->next;
        delete temp;
        size--;
        printList();
    }

    void find(int position) {
        if (position <= 0 || position > size) {
            cout << "error" << endl;
            return;
        }
        Node* current = head->next;
        int index = 1;

        while (current != head && index < position) {
            current = current->next;
            index++;
        }

        if (current == head) {
            cout << "error" << endl;
            return;
        }

        cout << current->data << endl;
    }

    void swapNode(int posa, int posb) {
        if (posa <= 0 || posb <= 0 || posa > size || posb > size || posa == posb) {
            cout << "error" << endl;
            return;
        }

        Node* preva = nullptr, *prevb = nullptr, *nodea = head, *nodeb = head;
        int nowposa = 0, nowposb = 0;

        while (nodea->next != head && nowposa < posa) {
            preva = nodea;
            nodea = nodea->next;
            nowposa++;
        }

        while (nodeb->next != head && nowposb < posb) {
            prevb = nodeb;
            nodeb = nodeb->next;
            nowposb++;
        }

        if (nodea == head || nodeb == head) {
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
        while (current != head) {
            cout << current->data << " ";
            current = current->next;
        }
        cout << endl;
    }

    int LL_marge(LinkedList *La, LinkedList *Lb) {
        Node *pa = La->head->next;
        Node *pb = Lb->head->next;
        Node *pc = La->head;
        while (pa != La->head && pb != Lb->head) {
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
        pc->next = (pa != La->head) ? pa : pb;
        Node* tail = pc;
        while (tail->next != La->head && tail->next != Lb->head) {
            tail = tail->next;
        }
        tail->next = La->head;
        La->size += Lb->size;
        return 0;
    }
    void joseph(int k,int s) {
        Node *temp= head;
        for(int i=0;i<s-1;i++){
           temp=temp->next;
        }
        Node *prve=nullptr;
        //Node *del=temp->next;
        //cout<<temp->next->data<<" ";
        //temp->next=del->next;
        while(size>1){
            for(int i=0;i<k-1;i++){
                prve=temp;
                temp=temp->next;    
            }
            Node *del=prve->next;
            cout<<temp->data<<" ";
            temp=temp->next;
            prve->next=temp;
            //delete del;
            size--;
        }
        head=temp;
        cout<<head->data<<" "<<endl;
    }
};

int main(){
    #ifndef ONLINE_JUDGE
    freopen("in.in", "r", stdin);
    freopen("out.out", "w", stdout);
    #endif
    int t;
    cin>>t;
    while(t--){
        int n,k,s;
        cin >> n;
        LinkedList list(n);
        cin >> k >> s;
        list.joseph(k,s);
    }
}