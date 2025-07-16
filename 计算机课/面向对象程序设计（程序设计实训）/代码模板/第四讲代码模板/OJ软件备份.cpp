#include <iostream>
#include <string>
using namespace std;

class CDate
{
private:
	int year, month, day;

public:
	CDate(int y, int m, int d)
	{
		year = y;
		month = m;
		day = d;
	}
	bool isLeapYear() { return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0; }
	int getYear() { return year; }
	int getMonth() { return month; }
	int getDay() { return day; }
	int getDayofYear() // 计算日期从当年1月1日算起的天数
	{
		int i, sum = day;
		int days[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
		
		if (isLeapYear())
			days[2] = 29;
		for (i = 0; i < month; i++)
			sum += days[i];
		return sum;
	}
	bool before()
	{
		if(year != 2015)
			return year < 2015;
		else if(month != 4)
			return month < 4;
		else
			return day < 7;
	}
};

class software
{
	string name;
	char   type;      //O,T,B
	CDate  deadline;
	char   media;    //D,H,U
public:
	software(string nval,char tval,int y,int m, int d, char mval):name(nval),
		type(tval),deadline(y,m,d),media(mval){}

	software(const software &rhs):deadline(rhs.deadline)
	{
		type = 'B';
		media = 'H';
		name = rhs.name;
	}

	void print()
	{
		cout << "name:" << name << endl;
		cout << "type:";
		if(type == 'O')
			cout << "original" << endl;
		else if(type == 'T')
			cout << "trial" << endl;
		else 
			cout << "backup" << endl;
		cout << "media:";
		if(media == 'D')
			cout << "optical disk" << endl;
		else if(media == 'H')
			cout << "hard disk" << endl;
		else
			cout << "USB disk" << endl;
		if(deadline.getDay()==0 && deadline.getMonth() == 0 && deadline.getYear() == 0)
			cout << "this software has unlimited use" << endl;
		else if(deadline.before())
			cout << "this software has expired" << endl;
		else
		{
			int sum = deadline.getDayofYear() -CDate(2015,4,7).getDayofYear();
			for(int y=2015; y<deadline.getYear(); y++)
				sum += !(y % 4) && y % 100 || !(y % 400)? 366 : 365;
			cout << "this software is going to be expired in " << sum <<" days" << endl;
		}
		cout << endl;
	}
};

int main()
{
	int t, year, month, day;
	string name;
	char type,media;

	cin >> t;
	while(t--)
	{
		cin >> name >> type >> media >> year >> month >>  day;
		software soft(name,type,year,month,day,media);
		soft.print();
		software softbak(soft);
		softbak.print();
	}
	system("pause");
}