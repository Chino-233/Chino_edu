//多重继承，name二义性
#include <iostream>
#include <string>
using namespace std;

class CPeople
{
protected:
	string name;
public:
	CPeople()
	{
		cout << "CPeople cons default " << endl;
	}
	CPeople(string nameval):name(nameval)
	{
		cout << "CPeople cons para " << endl;
	}
	void print()
	{
		cout << name << endl;
	}
	~CPeople()
	{
		cout << "CPeople des " << endl;
	}
};

class CStudent:virtual public CPeople  //虚基类
{
protected:
	string no;
public:
	CStudent()
	{
		cout << "CStudent cons default " << endl;
	}
	CStudent(string nameval,string noval):CPeople(nameval),no(noval)
	{
		cout << "CStudent cons para " << endl;
	}
	void print()
	{
		cout << name << " " << no << endl;
	}
	~CStudent()
	{
		cout << "CStudent des " << endl;
	}
};

class CTeacher:virtual public CPeople   //虚基类
{
protected:
	string title;
public:
	CTeacher()
	{
		cout << "CTeacher cons default " << endl;
	
	} 
	CTeacher(string nameval,string t):CPeople(nameval),title(t)
	{
		cout << "CTeacher cons para " << endl;
	}
	void print()
	{
		cout << name << " " << title << endl;
	}
	~CTeacher()
	{
		cout << "CTeacher des" << endl;
	}
};

class CGradOnWork:public CStudent, public CTeacher
{
	string research;
public:
	CGradOnWork()
	{
		cout << "CGradOnWork cons default" << endl;
	}
    //先调虚基类的构造函数
	CGradOnWork(string name,string no,string title,string r):CPeople(name),CTeacher(name,title),CStudent(name,no),research(r)
	{
		cout << "CGradOnWork cons para" << endl;
	}
	void print()
	{
		cout << name << " " << no << " " << title << " " << research << endl;
	}
	~CGradOnWork()
	{
		cout << "CGradOnWork des " << endl;
	}
};

int main()
{
	CGradOnWork grad("liming","2023001","lecture","Ai");
	//CGradOnWork grad;
	grad.print();
	system("pause");
}



