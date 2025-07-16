#include <iostream>
#include <sstream>
#include <string>
using namespace std;

/*
一问一世界, 杨澜, 56.7,上海文艺出版社
寂寞猎人,宫部美雪,27.2,百花洲文艺出版社
*/
int main(){
	string s, txt[100];

	/*
	while(getline(cin, s, ','))
		cout << s << endl;
	*/

	
	while(getline(cin,s))
	{
		stringstream ss;
		ss << s;

		int i = 0;
		while(getline(ss, txt[i],','))
			i++;
		
		cout << txt[0] << " " << txt[1] << " " << txt[2] << " " << txt[3] << endl;
		

	}
	system("pause");
	return 0;
}