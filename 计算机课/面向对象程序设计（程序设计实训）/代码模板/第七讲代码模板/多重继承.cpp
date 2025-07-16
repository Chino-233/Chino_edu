//多重继承，name二义性
#include <iostream>
#include <string>
using namespace std;

class CStudent 
{
protected:
	string name, no;
public:
	CStudent()
	{
		cout << "CStudent cons default " << endl;
	}
	CStudent(string nameval,string noval):name(nameval),no(noval)
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

class CTeacher 
{
protected:
	string name, title;
public:
	CTeacher()
	{
		cout << "CTeacher cons default " << endl;
	
	} 
	CTeacher(string nameval,string t):name(nameval),title(t)
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
	CGradOnWork(string name,string no,string title,string r):CTeacher(name,title),CStudent(name,no),research(r)
	{
		cout << "CGradOnWork cons para" << endl;
	}
	void print()
	{
		cout << CStudent::name << " " << no << " " << title << " " << research << endl;
	}
	~CGradOnWork()
	{
		cout << "CGradOnWork des " << endl;
	}
};

int main()
{
	CGradOnWork grad("liming","2023001","lecture","Ai");
	grad.print();
	system("pause");
}



