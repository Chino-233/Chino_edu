#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <cstring>
#include <algorithm>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <iomanip>
#include <cmath>
#include <map>
#include <stack>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <bitset>
#include <list>
#include <deque>
#include <forward_list>
#include <array>
#include <tuple>
#include <utility>
#include <memory>
#include <functional>
#include <iterator>
#include <numeric>
using namespace std;
const int N = 1e5 + 10;
typedef double db;
#define ll long long
char Prior[7][7] = {'>','>','<','<','<','>','>','>','>','<','<','<','>','>','>','>','>','>','<','>','>','>','>','>','>','<','>','>','<','<','<','<','<','=',' ','>','>','>','>',' ','>','>','<','<','<','<','<',' ','='};

float Operate(float a, unsigned char theta, float b)
{
    if('*'==theta)
        return a*b;
    else if('/'==theta)
        return a/b;
    else if('+'==theta)
        return a+b;
    else if('-'==theta)
        return a-b;
    else
        return -1;
}

char OPSET[7]={'+','-','*','/','(',')','#'};

int In(char Test, char *TestOp)
{
    int i;
    for(i=0;i<7;i++)
    {
        if(Test==TestOp[i])
            return 1;
    }
    return 0;
}

char precede(char Aop, char Bop)
{
    int i,j;
    for(i=0;i<7;i++)
    {
        if(OPSET[i]==Aop)
        {
            for(j=0;j<7;j++)
            {
                if(OPSET[j]==Bop)
                    return Prior[i][j];
            }
        }
    }
    return -1;
}


float EvaluateExpression(string MyExp)
{
    stack<char> OPTR;
    stack<double> OPND;
    char Tempnum[20];
    double num,a,b;
    char theta,Dr[2];
    char c;
    int i=0;
    OPTR.push('#');
    c = MyExp[0];
    strcpy(Tempnum,"\0");
    while(c!='#' || OPTR.top()!='#')
    {
        if(!In(c,OPSET))
        {
            Dr[0]=c;
            Dr[1]='\0';
            strcat(Tempnum,Dr);
            c=MyExp[++i];
            if(In(c,OPSET))
            {
                num=(float)atof(Tempnum);
                OPND.push(num);
                strcpy(Tempnum,"\0");
            }
        }
        else
        {
            switch(precede(OPTR.top(),c))
            {
                case '<':
                    OPTR.push(c);
                    c=MyExp[++i];
                    break;
                case '=':
                    OPTR.pop();
                    c=MyExp[++i];
                    break;
                case '>':
                    theta=OPTR.top();
                    OPTR.pop();
                    b=OPND.top();
                    OPND.pop();
                    a=OPND.top();
                    OPND.pop();
                    OPND.push(Operate(a,theta,b));
                    break;
            }
        }
    }
    return OPND.top();
}

int main()
{
    #ifndef ONLINE_JUDGE
	freopen("in.in", "r", stdin);
	freopen("out.out", "w", stdout);
    #endif
    string Exp;
    int t;
    double result;
    cin>>t;
    while(t--)
    {
        cin>>Exp;
        result=EvaluateExpression(Exp);
        cout<<fixed<<setprecision(4)<<result<<endl;
    }
    return 0;
}

