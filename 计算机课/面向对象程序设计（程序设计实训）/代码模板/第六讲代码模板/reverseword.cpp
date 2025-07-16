//input one line, find out words
#include <iostream>
#include <string>
#include <sstream>
using namespace std;


int main()
{
    string line,words[1000];
    stringstream ss;         //串流对象
    int i;

    while(getline(cin, line))   //input one line
    {
        ss.str(line);          //设置流对象值
        
        i = 0;
        while(ss>>words[i]) i++;    //input words from stringstream
        
        for(--i; i>=0; i--)       //output
            cout<<words[i]<<" ";
        cout<<endl;
    }
    system("pause");
    return 0;
}

