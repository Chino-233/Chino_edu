#include <iostream>
using namespace std;

class A
{
public:
    A() { cout << "A's default constructor called." << endl; }
    ~A() { cout << "A's destructor called." << endl; };
    void print() const { cout << "I am A" << endl; };
};
class B : public A
{
public:
    B() { cout << "B's default constructor called." << endl; }
    ~B() { cout << "B's destructor called." << endl; };
    void print() const { cout << "I am B" << endl; };
    void printOne() const { cout << "I am B too." << endl; }
};

int main()
{
    A a, *pa = &a;
    B b, *pb = &b;
    a = b;  //ok
    a.print();  //
    //a.printOne(); // red, error

    pa = pb;   //ok
    pa->print(); //ok
    ((B *)pa)->printOne(); 
    pb->printOne(); // error
    return 0;
}
