#include <iostream>
#include <string>
#include <vector>
using namespace std;

template <class T>
class cvector
{
public:
    typedef T *iterator; // 内嵌类

protected:
    iterator start;          // 当前使用空间头
    iterator finish;         // 当前使用空间尾
    iterator end_of_storage; // 目前可用空间尾

    // 类内实现友元重载输出
    /*
    friend ostream &operator<<(ostream &out, const cvector<T> &rhs)
    {
        for (typename cvector<T>::iterator it = rhs.begin(); it != rhs.end(); it++)
            out << *it << " ";
        return out;
    }
    */

    // 类内声明，类外定义
    template <class T1>
    friend ostream &operator<<(ostream &out, const cvector<T1> &rhs);

public :
    void reserve(int n); // 改变容量
    void emplace_back(T &e);          // 构造，末尾插入元素
    T back();                         // 访问最后一个元素
    T &operator[](int i);             // 访问第i个元素
    iterator begin() const;           // 返回开始迭代器
    iterator end() const;             // 返回结束迭代器
    int size();                       // 返回元素数
    iterator erase(iterator it);      // 删除it指向的元素
};

template <class T>
void cvector<T>::reserve(int n)
{
    start = new T[n];
    finish = start;
    end_of_storage = start + n;
}
template <class T>
void cvector<T>::emplace_back(T &e)
{
    *finish = e;
    finish++;
}

template <class T>
T cvector<T>::back()
{
    return *(finish - 1);
}

template <class T>
T &cvector<T>::operator[](int i)
{
    return *(start + i);
}

template <class T>
typename cvector<T>::iterator cvector<T>::begin() const
{
    return start;
}

template <class T>
typename cvector<T>::iterator cvector<T>::end() const
{
    return finish;
}

template <class T>
int cvector<T>::size()
{
    return finish - start;
}

template <class T>
typename cvector<T>::iterator cvector<T>::erase(iterator it)
{
    for (auto it1 = it; it1 + 1 < end(); it1++)
        *it1 = *(it1 + 1);
    finish--;
    return it;
}

template <class T>
ostream &operator<<(ostream &out, const cvector<T> &rhs)
{
    for (typename cvector<T>::iterator it = rhs.begin(); it != rhs.end(); it++)
        out << *it << " ";
    return out;
}

int main()
{
    cvector<string> v1;

    string s1 = "nhooo", s2 = ".com";
    v1.reserve(10);
    v1.emplace_back(s1);
    v1.emplace_back(s2);
    
    for(auto it = v1.begin(); it!=v1.end(); it++)
        cout << *it << " ";
    cout << endl;

    cout << v1 << endl;
    cout << v1.size() << endl;
    cout << v1[0] << endl;
    
    auto it = v1.erase(v1.begin()+1);
    cout << (it == v1.end()) << endl;

    
    for(auto it = v1.begin(); it!=v1.end(); it++)
        cout << *it << " ";
    
    
    system("pause");
    return 0;
}