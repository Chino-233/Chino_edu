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
        printList();
    }

    void insert(int position, int value) {
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
        printList();
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
};

int main(){
    #ifndef ONLINE_JUDGE
    freopen("in.in", "r", stdin);
    freopen("out.out", "w", stdout);
    #endif
    
    int n;
    cin >> n;
    LinkedList list(n);

    int pos1, pos2;
    cin >> pos1 >> pos2;
    list.swapNode(pos1, pos2);

    cin >> pos1 >> pos2;
    list.swapNode(pos1, pos2);

    return 0;
}