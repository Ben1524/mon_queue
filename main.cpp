#include<iostream>
#include<fstream>
#include<set>
#include<map>
#include<vector>

using namespace std;

const string filename = "graph4.txt";
struct Vertex;

struct Edge
{
    Vertex *start;  // 起点
    Vertex *end;   // 终点
    double weight; // 权重
};

struct Vertex
{
    string name; // 顶点名称
    set<Edge* > edges;  // 顶点的边
};

struct Graph
{
    set<Vertex* > vertexs; // 顶点集合
    set<Edge* > edges;  // 边集合
    map<string, Vertex* > vertexMap; // 顶点名称到顶点的映射, 用于快速查找顶点
};

void initGraphFromFile(Graph & G)
{
    ifstream ifs(filename, ios::in);
    string v,s,e;
    double w;
    while(!ifs.eof())
    {
        ifs >> v;
        if(v=="vertex")
        {
            ifs >> v;
            Vertex * vertex = new Vertex; // 创建顶点
            vertex->name = v; // 设置顶点名称
            G.vertexs.insert(vertex); // 将顶点加入到图中
            G.vertexMap[v] = vertex; // 将顶点加入到顶点名称到顶点的映射中
        }
        else if(v=="edge")
        {
            ifs>>s>>e>>w;
            Edge * edge = new Edge; // 创建边
            edge->start = G.vertexMap[s]; // 设置边的起点
            edge->end = G.vertexMap[e]; // 设置边的终点
            edge->weight = w; // 设置边的权重
            G.edges.insert(edge); // 将边加入到图中
            G.vertexMap[s]->edges.insert(edge); // 将边加入到起点的边集合中
        }
    }
}

void bellmanFord(const Graph&G,const Vertex* start) //start为源点
{
// 初始化
    map<Vertex*, double> dist; // 顶点到源点的距离,这里的源点为图中的第一个顶点
    map<Vertex*, Vertex*> path; // 顶点到源点的路径  // 这里的源点为图中的第一个顶点
    for(auto & v : G.vertexs)
    {
        dist[v] = INT_MAX; // 初始化距离为无穷大
        path[v] = nullptr; // 初始化路径为空
    }
    dist[const_cast<Vertex*>(start)] = 0; // 源点到源点的距离为0

    // 松弛
    for(int i=0; i<G.vertexs.size()-1; ++i) // 执行n-1次松弛操作
    {
        for(auto & e : G.edges) // 遍历所有边
        {
            if(dist[e->start]!=INT_MAX && dist[e->end]>dist[e->start]+e->weight) // 如果存在更短的路径
            {
                dist[e->end] = dist[e->start]+e->weight; // 更新距离
                path[e->end] = e->start; // 更新路径
            }
        }
    }

    // 检测负权回路
    for(auto & e : G.edges) // 遍历所有边
    {
        if(dist[e->start]!=INT_MAX && dist[e->end]>dist[e->start]+e->weight) // 如果存在更短的路径
        {
            cout << "存在负权回路" << endl;
            return; // 算法不同于Dijkstra算法, 如果存在负权回路, 则算法无法继续执行
        }
    }

    // 输出结果
    for(auto & v : G.vertexs)
    {
        cout << v->name << ": ";
        if(dist[v]==INT_MAX)
        {
            cout << "不可达" << endl;
            continue;
        }
        cout << "dist: " << dist[v] << " ";
        cout << "path: ";
        Vertex * p = path[v];
        while(p!=nullptr)
        {
            cout << p->name << " ";
            p = path[p];
        }
        cout << endl;
    }
}


int main()
{
    Graph G;
    initGraphFromFile(G);

    // --------测试代码--------

//    cout << "顶点个数: " << G.vertexs.size() << endl;
//    cout << "边个数: " << G.edges.size() << endl;
//    cout << "顶点名称到顶点的映射大小: " << G.vertexMap.size() << endl;
//    cout << "顶点集合: " << endl;
//    for(auto & v : G.vertexs)
//    {
//        cout << v->name << endl;
//    }
//    cout << "边集合: " << endl;
//    for(auto & e : G.edges)
//    {
//        cout << e->start->name << " " << e->end->name << " " << e->weight << endl;
//    }
//    cout << "顶点的边集合: " << endl;
//    for(auto & v : G.vertexs)
//    {
//        cout << v->name << ": ";
//        for(auto & e : v->edges)
//        {
//            cout << e->start->name << " " << e->end->name << " " << e->weight << " ";
//        }
//        cout << endl;
//    }

    // --------测试代码--------
    cout<<"请输入源点："<<endl;
    string start;
    cin>>start;
    for(auto & v : G.vertexs)
    {
        if(v->name==start)
        {
            bellmanFord(G,v);
            return 0;
        }
    }
}