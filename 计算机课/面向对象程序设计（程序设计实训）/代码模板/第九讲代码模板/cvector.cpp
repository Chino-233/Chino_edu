#include <iostream>
#include <numeric>
using namespace std;
class cvector
{
    int n;   
    int *data;
public:
    cvector() = default;    //缺省构造函数
    //带参构造函数
    cvector(int nval,int *dval):n(nval),data(new int[n])
    {
        memcpy(data, dval, sizeof(int)*n);
    }
    //成员函数重载+，返回对象未调拷贝构造，因g++编译做了优化
    //返回值为对象时，不再产生临时对象，不调用拷贝构造。
    cvector operator+(const cvector &rhs) const
    {
        int *res = new int[n];
        for(int i=0; i<n; i++)
        {
            res[i] = data[i] + rhs.data[i];
        }
        return cvector(n, res);
    }
    //成员函数重载-
    cvector operator-(const cvector &rhs) const
    {
        int *res = new int[n];
        for(int i=0; i<n; i++)
        {
            res[i] = data[i] - rhs.data[i];
        }
        return cvector(n, res);
    }
    //重载下标，返回引用，可访问，修改元素值
    int &operator[](int i)
    {
        return data[i];
    }
    //重载类型转换，得到各元素和
    operator int()
    {
        /*int sum = 0;
        for(size_t i=0; i<n; i++)
            sum += data[i];
        return sum;*/
        return accumulate(data, data+n, 0);
    }
    //重载关系运算符>
    bool operator>(cvector &rhs)
    {
        size_t n1 = 0, n2 = 0;
        for(size_t i=0; i<n; i++)
        {
            n1 += (data[i] > rhs.data[i]);
            n2 += (data[i] < rhs.data[i]);
        }
        return n1 > n2;
    }
    //重载输入
    friend istream &operator>>(istream &in, cvector &rhs)
    {
        in >> rhs.n;
        rhs.data = new int[rhs.n];
        for(size_t i=0; i<rhs.n; i++)
            in >> rhs.data[i];
        return in;
    }
    //重载输出
    friend ostream &operator<<(ostream &out, const cvector &rhs)
    {
        out <<  rhs.n << " ";
        for(size_t i=0; i<rhs.n; i++)
            out << rhs.data[i] << " ";
        return out;
    }
    //重载函数调用运算符（），每个元素加num
    cvector &operator()(int num)
    {
        for(size_t i=0; i<n; i++)
            data[i] += num;
        return *this;
    }
    //重载逗号运算符,,返回第二个元素
    cvector &operator,(cvector &rhs)
    {
        return rhs;
    }
    //数据成员有指针，重载=，深赋值。测试整型(a=10)=20,理解为何返回引用
    cvector &operator=(const cvector &rhs)
    {
        if(this == &rhs)
            return *this;
        if(data)
        {
            delete []data;
        }
        n = rhs.n;
        data = new int[n];
        memcpy(data, rhs.data, n*sizeof(int));
        return *this;
    }

    //析构
    ~cvector()
    {
        cout << data << endl;
        delete []data;
    }
};


//主函数
int main()
{
    #ifndef ONLINE_JUDGE
        freopen("in.txt","r",stdin);
        freopen("out.txt","w",stdout);
    #endif
    

    cvector v1,v2,v3;
    cin >> v1 >> v2 >> v3;   //输入三个向量
    cout << v1 << endl << v2 << endl << v3 << endl; //输出三个向量

    //计算v1+v2, v1-v2，输出
    cout << v1 + v2 << endl << v1 - v2 << endl;

    //比较v1>v2
    cout << (v1 > v2) << endl;

    //统计v1的元素和
    cout << int(v1) << endl;

    //函数调用测试，v1各元素加10
    cout << v1(10) << endl;

    //逗号运算符测试
    cout << (v1, v2, v3) << endl;

    //赋值，下标
    v1 = v2;
    v1[0] = 100;
    cout << v1 << endl;

    return 0;

}