#include <bits/stdc++.h>
using namespace std;
const int N = 1e5 + 10;
typedef double db;
#define int long long

struct Guest
{
    int T, P, VIP;
};
struct winstu
{
    bool stu;
    int ti;
    int num;
};

int waittime = 0;
int longtime = 0;
int fintime = 0;
int currenttime = 0;

bool choose(queue<Guest> &q, vector<winstu> &win, Guest g, int kvip)
{
    // if (!win[kvip].stu)
    // {
    //     if (g.VIP)
    //     {
    //         win[kvip].stu = 1;
    //         win[kvip].ti = g.T + g.P;
    //         waittime += currenttime - g.T;
    //         q.pop();
    //     }
    // // }
    // else
    {
        for (int i = 0; i < win.size(); i++)
        {
            if (!win[i].stu)
            {
                win[i].stu = 1;
                win[i].ti = currenttime + g.P;
                win[i].num++;
                waittime += currenttime - g.T;
                if (currenttime - g.T > longtime)
                {
                    longtime = currenttime - g.T;
                }
                q.pop();
                return 1;
            }
        }
        return 0;
    }
}

void QAQ()
{
    int n;
    cin >> n;
    // vector<Guest> arr;
    queue<Guest> q;
    for (int i = 0; i < n; i++)
    {
        int t, p, vip;
        cin >> t >> p >> vip;
        if (p > 60)
            p = 60;
        q.push({t, p, vip});
    }
    int k, kvip;
    cin >> k >> kvip;
    vector<winstu> win(k, {0, 0, 0});
    while (!q.empty())
    {
        // if (currenttime == 62)
        // {
        //     cout << q.size() << endl;
        // }
        for (int i = 0; i < k; i++) // 先更新窗口
        {
            if (win[i].ti == currenttime)
            {
                win[i].stu = 0;
                win[i].ti = 0;
            }
        }
        bool flag = 0;
        while (!flag) // 多次循环，直到没有新顾客进入
        {
            flag = 1;
            if (q.empty())
            {
                break;
            }

            if (!win[kvip].stu) // 判断vip窗口是否为空
            {
                queue<Guest> temp;
                while (!q.empty())
                {
                    Guest it = q.front();
                    if (it.VIP && it.T <= currenttime && !win[kvip].stu)
                    {
                        win[kvip].stu = 1;
                        win[kvip].ti = currenttime + it.P;
                        win[kvip].num++;
                        waittime += currenttime - it.T;
                        if (currenttime - it.T > longtime)
                        {
                            longtime = currenttime - it.T;
                        }
                    }
                    else
                    {
                        temp.push(it);
                    }
                    q.pop();
                }
                q = temp;
            }

            if (q.empty())
                break;

            Guest g = q.front();
            if (g.T <= currenttime)
            {
                if (choose(q, win, g, kvip))
                {
                    flag = 0;
                }
            }
            else
            {
                break;
            }
        }
        currenttime++;
    }

    for (int i = 0; i < k; i++)
    {
        if (win[i].ti > fintime)
        {
            fintime = win[i].ti;
        }
    }
    float avg = 1.0 * waittime / n;
    cout << avg << " " << longtime << " " << fintime << endl;
    for (auto it : win)
    {
        cout << it.num << " ";
    }
}

signed main()
{
#ifndef ONLINE_JUDGE
    freopen("in.in", "r", stdin);
    freopen("out.out", "w", stdout);
#endif
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    int t = 1;
    //    cin>>t;
    while (t--)
    {
        QAQ();
    }
    return 0;
}
