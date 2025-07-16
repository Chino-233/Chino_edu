//输入两个字符串。若均为数字串，输出两个数字的和；否则，输出串链接。
#include <iostream>
#include <string>
#include <sstream>
#include <regex>
using namespace std;

//判读是否数字串
bool alldigit(string s)
{
    for (auto x : s)
        if (x < '0' || x > '9')
            return false; //非数字串
    return true;          //数字串
}

int main()
{
    string str1, str2;
	//stringstream ss;
    int num1, num2;
    while (cin >> str1 >> str2) //循环输入两个串
    {
        if (alldigit(str1) && alldigit(str2)) //均为数字串
		{
			stringstream ss;			  //串转数字，方法一
			ss << str1 << " " << str2;    
            ss >> num1 >> num2; 
			//ss.clear();                  //如果stringstream ss在20句定义，这句要打开。

			/*num1 = stoi(str1);        //串转数字，方法二
            num2 = stoi(str2);*/

            //sscanf(str1.c_str(),"%d",&num1);  //串转数字，方法三
            //sscanf(str2.c_str(),"%d",&num2);
            cout << num1 + num2 << endl;
        }
        else //非数字串
            cout << str1 + str2 << endl;
    }
    system("pause");
    return 0;
}