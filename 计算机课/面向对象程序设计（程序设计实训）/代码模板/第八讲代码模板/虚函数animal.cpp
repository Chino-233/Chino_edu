#include <iostream>
using namespace std;

class Animal 
{
public:
	virtual void speak()
	{
		cout << "animal" << endl;
	}
};

class Tiger:public Animal 
{
public:
	virtual void speak()
	{
		cout << "AOOO" << endl;
	}
};

class Dog:public Animal  
{
public:
	virtual void speak() 
	{
		cout << "WangWang" << endl;
	}
};

class Cat:public Animal
{
public:
	void speak(int l)
	{
		cout << "MiaoMiao" << endl;
	}
};

int main()
{
	#ifndef ONLINE_JUDGE
		freopen("in.txt", "r", stdin);
		freopen("out.txt", "w", stdout);
	#endif
	//Animal an;   //error
	//Animal *pan;  //ok
	int t, type;
	
	cin >> t;
	while(t--)
	{
		cin >> type;
		if(type == 1)
		{
			Tiger t;
			//an = t;
			//pan = &t;
			//pan->speak();
			Animal &ran = t;
			ran.speak();
	
		} 
		else if(type == 2)
		{
			Dog d;
			//pan = &d;
			//pan->speak();
			//an = d;
			Animal &ran = d;
			ran.speak();
	
		}
		else if(type == 3)
		{
			Cat c;
			//pan = &c;
			//pan->speak();
			//an = c;
			Animal &ran = c;
			ran.speak();
			c.speak(10);
	
		}
		//an.speak();
		//pan->speak();
	}

}

