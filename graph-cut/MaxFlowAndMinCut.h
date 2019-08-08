#ifndef MAXFLOWANDMINCUT_H
#define MAXFLOWANDMINCUT_H
#include <iostream>
#include <vector>
#include <queue>

using namespace std;

struct Edge
{
	int endVertex;  //终点
	int weight;//容量
	int flow = 0;//流量
	bool isRevEdge = false;//是否为反向边
};
struct Vertex
{
	int index;
	vector<Edge> edge;
};

//重载<<符号，用于输出顶点信息
ostream& operator <<(ostream &os, Vertex v)
{
	cout << "输出当前顶点序号：" << v.index << endl;
	for (int i = 0; i < v.edge.size(); i++)
		cout << v.edge[i].endVertex << '\t' << v.edge[i].flow << "/" << v.edge[i].weight << endl;
	cout << endl;
	return os;
}

//重载<<符号，用于输出路径信息
ostream& operator <<(ostream &os, vector<int> path)
{
	int size = path.size();
	for (int i = 0; i < size; i++)
		cout << path[size - 1 - i] << '\t';
	cout << endl;
	return os;
}

vector<Vertex> vertexs;//用于存放网络图的顶点
vector<int> EKQue;//记录当前寻找的增广路径
queue<int> myQueue;//记录寻找过程中遍历的所有顶点



bool *isVisited;//标注顶点是否被访问
int *preVertex;//一维数组，大小与顶点个数相同，记录被访问顶点连接边的起点

void bfs(int, bool&);//利用广度优先算法寻找增广路径
void backFlow(Vertex &v, int &minFlow);

//输入顶点及其连接的边 以-1结束输入一个顶点的输入
void bulidGraph(int n)
{
	for (int i = 0; i < n; i++)
	{
		int temp;
		Vertex tempVertex;
		vector<Edge> tempEdge;
		cout << "输入第" << i << "个顶点：" << endl;
		while (1)
		{
			cin >> temp;
			if (temp == -1)
			{
				tempVertex.index = i;
				tempVertex.edge = tempEdge;
				vertexs.push_back(tempVertex);
				break;
			}
			Edge t;
			t.endVertex = temp;
			cin >> temp;
			t.weight = temp;
			tempEdge.push_back(t);

		}
	}
	//for (int i = 0; i < vertexs.size(); i++)
		//cout << vertexs[i];

}
void EKMaxFlow(int n)
{
	isVisited = new bool[n];
	for (int i = 0; i < n; i++)
		isVisited[i] = 0;
	preVertex = new int[n];
	for (int i = 0; i < n; i++)
		preVertex[i] = 0;
	bool ExistPath = 1;//表示找到了一条从源点到汇点的路径
	while (ExistPath)
	{
		ExistPath = 0;
		bfs(0, ExistPath);
		for (int i = 0; i < n; i++)
			isVisited[i] = 0;
		for (int i = 0; i < n; i++)
			preVertex[i] = 0;
		//cout << "正在运行" << endl;
		queue<int> empty;
		swap(empty, myQueue);
	}
	//for (int i = 0; i < vertexs.size(); i++)
		//cout << vertexs[i];
}

//寻找增广路径
void bfs(int vindex, bool &ExistPath)
{
	bool found = 1;
	myQueue.push(vindex);

	while (found && !myQueue.empty())
	{
		int p = myQueue.front();
		for (int i = 0; i < vertexs[p].edge.size(); i++)
		{
			int x = isVisited[vertexs[p].edge[i].endVertex];
			if (vertexs[p].edge[i].flow < vertexs[p].edge[i].weight && !isVisited[vertexs[p].edge[i].endVertex])
			{
				if (!vertexs[vertexs[p].edge[i].endVertex].edge.size())//如果找到了汇点
				{
					int minFlow = vertexs[p].edge[i].weight - vertexs[p].edge[i].flow;
					backFlow(vertexs[p], minFlow);
					vertexs[p].edge[i].flow += minFlow;
					found = 0;
					ExistPath = 1;
					vector<int>().swap(EKQue);
					break;
				}
				else
				{

					myQueue.push(vertexs[p].edge[i].endVertex);//将该顶点推入队列
					preVertex[vertexs[p].edge[i].endVertex] = p;
					if(vertexs[p].edge[i].isRevEdge==1&& vertexs[p].edge[i].flow!=0)
						EKQue.push_back(vertexs[p].edge[i].endVertex);//将该顶点加入树
					else if(vertexs[p].edge[i].isRevEdge == 0)
						EKQue.push_back(vertexs[p].edge[i].endVertex);

					isVisited[vertexs[p].edge[i].endVertex] = 1;//将该点标注为已访问
				}
			}
		}
		myQueue.pop();
	}
}
void backFlow(Vertex &v, int &minFlow)
{
	int index = preVertex[v.index];
	int i = 0;
	for (i; i < vertexs[index].edge.size(); i++)
		if (vertexs[index].edge[i].endVertex == v.index)
		{
			int tempFlow = vertexs[index].edge[i].weight - vertexs[index].edge[i].flow;
			minFlow = minFlow < tempFlow ? minFlow : tempFlow;
			if (index != 0)
			{
				backFlow(vertexs[index], minFlow);
			}
			break;
		}
	//增加流
	vertexs[index].edge[i].flow += minFlow;

	//添加反向边
	if (vertexs[index].index != 0 && vertexs[index].edge[i].isRevEdge == 0)
	{

		for (int i = 0; i < v.edge.size(); i++)
		{
			if (v.edge[i].endVertex == index)//如果已经存在这样的反向边
			{
				v.edge[i].weight += minFlow;
				break;
			}
			if (i == v.edge.size() - 1 && v.edge[i].endVertex != index)
			{
				Edge temp;
				temp.endVertex = index;
				temp.weight = minFlow;
				temp.isRevEdge = 1;
				v.edge.push_back(temp);
				break;
			}
		}

	}


}

//在我自己想的最大流算法中使用
//vector<Vertex> Que;//用于存储分割后属于源点的顶点
//bool *isCurrentPath;

//void findPath(Vertex &v, vector<int> &path, int numOfVertexs, int &addFlow);
//void minCut(const Vertex &v, vector<Vertex> &Que);

/*
void myMaxFlow(int n)
{
	vector<int> path;
	int addFlow;
	isCurrentPath = new bool[n];
	for (int i = 0; i < n; i++)
		isCurrentPath[i] = 0;
	//更新图
	while (1)
	{
		addFlow = vertexs[0].edge[0].weight;
		findPath(vertexs[0], path, n, addFlow);
		if (path.size() == 0)
			break;
		//cout << path;输出当前路径
		vector<int>().swap(path);
		for (int i = 0; i < n; i++)
			isCurrentPath[i] = 0;
		//if(path.size())
		//cout << "path未被清空";//验证path是否被清空
		//cout << endl << endl;
	}

	Que.push_back(vertexs[0]);
	isCurrentPath[0] = 1;
	minCut(vertexs[0], Que);

}*/
/*void findPath(Vertex &v, vector<int> &path, int numOfVertexs, int &addFlow)
{
	if (v.index == numOfVertexs - 1)
	{
		path.push_back(v.index);
		return;
	}
	else
	{
		for (int i = 0; i < v.edge.size(); i++)
		{
			if (isCurrentPath[v.edge[i].endVertex] == 0 && v.edge[i].flow < v.edge[i].weight)//如果当前点未被添加到路径中并且当前点还可以添加流量
			{
				int restCap = v.edge[i].weight - v.edge[i].flow;//剩余容量
				addFlow = restCap > addFlow ? addFlow : restCap;
				isCurrentPath[v.index] = 1;//将当前顶点暂时设为路径

				findPath(vertexs[v.edge[i].endVertex], path, numOfVertexs, addFlow);
				if (path.size() > 0)
				{
					if (path[0] == numOfVertexs - 1)
					{
						//增加流
						path.push_back(v.index);
						isCurrentPath[v.index] = 1;
						v.edge[i].flow += addFlow;


						//添加反向边
						if (v.index != 0 && v.edge[i].endVertex != numOfVertexs - 1 && v.edge[i].isRevEdge == 0)
						{
							Edge temp;
							int p = v.edge[i].endVertex;
							if (vertexs[p].edge.size() == 0)
							{
								temp.endVertex = v.index;
								temp.weight = addFlow;
								temp.isRevEdge = 1;
								vertexs[p].edge.push_back(temp);
							}
							else
							{
								for (int i = 0; i < vertexs[p].edge.size(); i++)
								{
									if (vertexs[p].edge[i].endVertex == v.index)//如果已经存在这样的反向边
									{
										vertexs[p].edge[i].weight += addFlow;
										break;
									}
									if (i == vertexs[p].edge.size() - 1 && vertexs[p].edge[i].endVertex != v.index)
									{
										temp.endVertex = v.index;
										temp.weight = addFlow;
										temp.isRevEdge = 1;
										vertexs[p].edge.push_back(temp);
										break;
									}
								}
							}
						}

						//cout << "当前结果：" << endl;
						//for (int i = 0; i < vertexs.size(); i++)
							//cout << vertexs[i];


					}
					break;
				}
				else if (path.size() == 0)
					isCurrentPath[v.index] = 0;
			}

		}
	}
}*/
/*void minCut(const Vertex &v, vector<Vertex> &Que)
{
	if (v.edge.size() != 0)
	{
		for (int i = 0; i < v.edge.size(); i++)
			if (v.edge[i].isRevEdge == true)
			{
				if (v.edge[i].flow != 0)
				{
					if (isCurrentPath[v.edge[i].endVertex] == 0)
					{
						Que.push_back(vertexs[v.edge[i].endVertex]);
						isCurrentPath[v.edge[i].endVertex] = 1;
						minCut(vertexs[v.edge[i].endVertex], Que);
					}
				}
			}
			else if (v.edge[i].flow < v.edge[i].weight)
			{
				if (isCurrentPath[v.edge[i].endVertex] == 0)
				{
					Que.push_back(vertexs[v.edge[i].endVertex]);
					isCurrentPath[v.edge[i].endVertex] = 1;
					minCut(vertexs[v.edge[i].endVertex], Que);
				}
			}
	}
}*/
#endif // !MAXFLOWANDMINCUT_H

