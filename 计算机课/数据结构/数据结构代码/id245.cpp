#include <iostream>
#include <vector>
#include <string>
#include <queue>
using namespace std;

//定义顶点类
class Vertex {
public:
    int indexNo; // 节点编号
    bool hasEnterQueue; // 是否进入队列
    int early; // 最早开始时间
    int later; // 最晚开始时间

    Vertex(int indexNo) {
        this->indexNo = indexNo;
        this->hasEnterQueue = false;
        early = -1;
        later = 0x7FFFF;
    }
    // 更新最早开始时间
    void updateEarly(int parentEarly, int edgeValue) {
        int newEarly = parentEarly + edgeValue;
        if (newEarly > this->early)
            this->early = newEarly;
    }
    // 更新最晚开始时间
    void updateLater(int childLater, int edgeValue) {
        int newLater = childLater - edgeValue;
        if (newLater < this->later)
            this->later = newLater;
    }
};


class Graph {
public:
    vector<Vertex> vertexes;
    vector<vector<int> > adjMat;
    int n;
public:
    void readVertexes() {
        //TODO: 将顶点数读入成员变量n
        cin>>n;
        //TODO： 从输入初始化vertexes数组
        vertexes.clear();
        int i=0;
        for(; i<n; ++i) {
            Vertex v(i);
            this->vertexes.push_back(v);
        }
        
        //为成员变量adjMat创建内存，赋初值
        for(i=0; i<n; ++i) {
            vector<int> row;
            int j=0;
            for(; j<n; ++j) {
                //TODO： 将0增加到row最后
                row.push_back(0);
            }
           //TODO： 将row增加到adjMat最后
            adjMat.push_back(row);
        }
    }
    void readAdjMatrix() {
        //read the adjacent info into this->adjMat
        int edges;
        cin >> edges;
        int i=0;
        int s, t, w;  //s源顶点编号，t目的顶点编号，w边长
        for(; i<edges; ++i) {
            cin >> s >> t >> w;
            adjMat[s][t] = w;
            //TODO: 读入s,t,w，并将adjMat的第s行、第t列的值改为w.
        }
    }

    void updateEarly(int parentNo) {
        int parentEarly = vertexes[parentNo].early;  //读入父结点early值
        int j=0;
        for(; j<n; ++j) {
            int edgeValue = adjMat[parentNo][j];
            if (edgeValue == 0) continue;  //若父结点与结点j没有边相连，pass

            Vertex& child = vertexes[j];
            child.updateEarly(parentEarly, edgeValue); //更新子结点j的early信息
            updateEarly(j); //递归更新子结点j的early信息
        }
    }
    void updateLater(int childNo) {
        //TODO: 更新每个顶点的最迟开始时间
        int childLater = vertexes[childNo].later;  //读入子结点later值
        int j=0;
        for(; j<n; ++j) {
            int edgeValue = adjMat[j][childNo];
            if (edgeValue == 0) continue;  //若父结点与结点j没有边相连，pass
            vertexes[j].updateLater(childLater, edgeValue); //更新父结点j的later信息
            updateLater(j); //递归更新父结点j的later信息
        }
    }

    int getRoot() {
        //获取入度为0的顶点
        int j=0;
        for(; j<n; ++j) {
            int i=0;
            for(; i<n && adjMat[i][j] == 0; ++i);
            if (i>=n&&vertexes[j].hasEnterQueue == false) return j; //j has not any in-edges.
        }
        return -1;  //表示没找到
    }
    int getLeaf() {
        //TODO： 获取出度为0的顶点
        int j=0;
        for(; j<n; ++j) {
            int i=0;
            for(; i<n && adjMat[j][i] == 0; ++i);
            if (i>=n&&vertexes[j].hasEnterQueue == false) return j; //j has not any out-edges.
        }
        return -1;  //表示没找到
    }

    void printEarlyLater(bool isEarly) {
        int i=0;
        for(; i<n; ++i) {
            Vertex& v = vertexes[i];
            if (isEarly)
                cout << v.early << " ";
            else {
                cout << v.later << " ";
            }
        }
        cout << endl;
    }

    void findEarly() {
        //执行关键路径算法，求每个顶点的最早开始时间。
        clearEnterQueue();
        int root=getRoot();
        while (root!=-1) 
        {
            vertexes[root].early = 0;
            vertexes[root].hasEnterQueue = true;
            root=getRoot();
        }
        clearEnterQueue();
        root=getRoot();
        while (root!=-1){
            updateEarly(root);
            vertexes[root].hasEnterQueue = true;
            root=getRoot();
        }
        printEarlyLater(true);
    }
    // 清除每个顶点的hasEnterQueue属性为false
    void clearEnterQueue() {
        int i=0;
        for(; i<n; ++i) {
            vertexes[i].hasEnterQueue = false;
        }
    }
    // 执行关键路径算法，求每个顶点的最迟开始时间。
    void findLater() {
        //TODO：调用clearEnterQueue，以清除每个顶点的hasEnterQueue=false
        clearEnterQueue();
        int leaf=getLeaf();
        while (leaf!=-1) {
            vertexes[leaf].later = vertexes[leaf].early;
            vertexes[leaf].hasEnterQueue = true;
            leaf=getLeaf();
        }
        clearEnterQueue();
        leaf=getLeaf();
        while (leaf!=-1) {
            updateLater(leaf);
            vertexes[leaf].hasEnterQueue = true;
            leaf=getLeaf();
        }
        printEarlyLater(false);
    }

    // 主函数，执行整个算法
    void main() {
        readVertexes();
        readAdjMatrix();
        findEarly();
        findLater();
    }
};


int main() {
    // 如果不是在线编译，则重定向输入输出文件
    #ifndef ONLINE_JUDGE
    freopen("in.in", "r", stdin);
    freopen("out.out", "w", stdout);
    #endif
    int t=1;
    //cin >> t;
    // 循环t次
    while (t--) {
        Graph g;
        // 调用Graph类的main函数
        g.main();
    }
    return 0;
}