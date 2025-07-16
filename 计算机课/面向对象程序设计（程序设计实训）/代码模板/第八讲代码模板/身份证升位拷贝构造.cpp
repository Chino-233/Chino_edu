#include <iostream> 
#include <string>
#include <cstring>
#include <sstream>
#include <iomanip>
using namespace std;

class CDate
{
private:
    int year, month, day;
public:
    CDate(int, int, int);
    bool check(); // 检验日期是否合法
    bool isLeap();
    void print();
    friend string to_string(const CDate &b)
    {
        stringstream ss;
        ss << b.year << setw(2) << setfill('0') << b.month << setw(2) << setfill('0') << b.day;
        return ss.str();
    }
};

class CStudentID
{
private:
    char *p_id, *p_name; // 身份证号码，姓名
    CDate birthday; // 出生日期
    int registered; // 登记否
public:
    CStudentID(char *p_idval, char *p_nameval, CDate &day); // 构造函数，若:日期非法;日期与id日期不符;id有非法字符;id不是15位或18位;id是18位但检验码不对,则输出"illegal id"并置registered=0。否则输出对象的属性并置registered=1.
    CStudentID(const CStudentID &r); // 拷贝构造函数，若r.p_id为15位则升到18位(加年份的19和校验位)
    bool Isdigit(string id)
    {
        for(int i=0; i<id.size(); i++)
            if(!isdigit(id[i]))
                return 0;
        return 1;
    }
    char getlast();
    bool checked() { return registered; }
    
    ~CStudentID();
};

CDate::CDate(int y, int m, int d):year(y),month(m),day(d){}
bool CDate::check()
{
    int days[]{0,31,28,31,30,31,30,31,31,30,31,30,31};
    if(isLeap()) days[2]++;
    return month<=12 && month >=1 && day >= 1 && day <= days[month];
}
bool CDate::isLeap()
{
    return !(year%400) || !(year%4) && year%100;
}

void CDate::print() 
{
    cout << year << "年" << month << "月" << day << "日";
}


CStudentID::CStudentID(char *p_idval, char *p_nameval, CDate &day):birthday(day)
// 构造函数，若:日期非法;日期与id日期不符;id有非法字符;id不是15位或18位;id是18位但检验码不对,则输出"illegal id"并置registered=0。否则输出对象的属性并置registered=1.
{
    p_id = new char[strlen(p_idval)+1];
    strcpy(p_id,p_idval);

    p_name = new char[strlen(p_nameval)+1];
    strcpy(p_name, p_nameval);

    if(strlen(p_id)!=15 && strlen(p_id)!=18 && !day.check())
    {
        cout << p_name << " illegal id" << endl;
        registered = 0;
        return;
    }
    string pid = string(p_id);
    if(strlen(p_id) == 15)
        pid.insert(6,"19");

    if(pid.substr(6,8) != to_string(birthday) || !Isdigit(pid.substr(0,17)))
    {
        cout << p_name << " illegal id" << endl;
        registered = 0;
        return;
    }
    if(pid.size()==18)
    {
        int weight[]{7,9,10,5,8,4,2,1,6,3,7,9,10,5,8,4,2};
        char verfity[] ="10X98765432";
        int sum = 0;
        for(int i=0; i<17; i++)
            sum += (pid[i]-'0')*weight[i];
        if(pid[17] != verfity[sum%11])
        {
            cout << p_name << " illegal id" << endl;
            registered = 0;
        }   
    }
    else
    {
        registered = 1;   
        cout << p_name << " ";
        birthday.print();
        cout << " " << p_id << endl;
    }
}    

CStudentID::CStudentID(const CStudentID &r):birthday(r.birthday) // 拷贝构造函数，若r.p_id为15位则升到18位(加年份的19和校验位)
{
    p_id = new char[20]{0};
    p_name = new char[strlen(r.p_name)+1];
    strcpy(p_id, r.p_id);
    strcpy(p_name, r.p_name);

    if(strlen(r.p_id) == 15)
    {
        p_id[6] = 0;
        strcat(p_id,"19");
        strcpy(p_id+8 , r.p_id+6);
        int weight[]{7,9,10,5,8,4,2,1,6,3,7,9,10,5,8,4,2};
        char verfity[] ="10X98765432";
        int sum = 0;
        for(int i=0; i<17; i++)
            sum += (p_id[i]-'0')*weight[i];
        p_id[17] += verfity[sum%11];
        cout << p_name << " ";
        birthday.print();
        cout << " " << p_id << endl;
    }
}

CStudentID::~CStudentID()
{
    delete []p_id;
    delete []p_name;
}

int main()
{
    int t, year, month, day;
    char name[20], id[20];

    cin >> t;
    while(t--)
    {
        cin >> year >> month >> day;
        CDate birthday(year,month,day);
        cin >> name >> id;
        CStudentID s(id, name, birthday);

        if(s.checked())
            CStudentID snew = s;
    }
    system("pause");
}



