#include <iostream>
using namespace std;


class CDate
{
	int year, month, day;
public:
	CDate()
	{
		cout << " CDate  cons" << endl;
	}
	~CDate()
	{
		cout << " CDate  des" << endl;
	}
	CDate(int y, int m, int d) : year(y), month(m), day(d) 
	{
		cout << " CDate cons with para" << endl;
	}
	int getyear()
	{
		return year;
	}
	int getmonth()
	{
		return month;
	}
	int getday()
	{
		return day;
	}
};

class CPerson
{
protected:
	string name, sex;
public:
	CPerson()
	{
		cout << " CPerson cons" << endl;
	}
	~CPerson()
	{
		cout << " CPerson des" << endl;
	}
	CPerson(string n, string s) : name(n), sex(s) 
	{
		cout << " CPerson cons with para" << endl;
	}
	string getname()
	{
		return name;
	}
	string getsex()
	{
		return sex;
	}
	void display()
	{
		cout << name << " " << sex << endl;
	}
};

class CStudent: public CPerson
{
	string no;
	CDate endate;
public:
	CStudent(string name,string sex,string noval,int y,int m,int d):CPerson(name,sex),no(noval),endate(y,m,d)
	{
		cout << " CStudent cons with para" << endl;
	}
	CStudent()
	{
		cout << " CStudent cons" << endl;
	}
	~CStudent()
	{
		cout << " CStudent des" << endl;
	}
	void display(int l)
	{
        CPerson::display();
		cout << name << " " << sex << " " << no << " " << endate.getyear() << "/"
		     << endate.getmonth() << endl;
	}
};

int main()
{
	CStudent s("lisi","female","2023",2023,9,1);
	s.display();

	system("pause");
}

