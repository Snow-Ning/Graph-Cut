#ifndef MAXFLOWANDMINCUT_H
#define MAXFLOWANDMINCUT_H
#include <iostream>
#include <vector>
#include <queue>

using namespace std;

struct Edge
{
	int endVertex;  //�յ�
	int weight;//����
	int flow = 0;//����
	bool isRevEdge = false;//�Ƿ�Ϊ�����
};
struct Vertex
{
	int index;
	vector<Edge> edge;
};

//����<<���ţ��������������Ϣ
ostream& operator <<(ostream &os, Vertex v)
{
	cout << "�����ǰ������ţ�" << v.index << endl;
	for (int i = 0; i < v.edge.size(); i++)
		cout << v.edge[i].endVertex << '\t' << v.edge[i].flow << "/" << v.edge[i].weight << endl;
	cout << endl;
	return os;
}

//����<<���ţ��������·����Ϣ
ostream& operator <<(ostream &os, vector<int> path)
{
	int size = path.size();
	for (int i = 0; i < size; i++)
		cout << path[size - 1 - i] << '\t';
	cout << endl;
	return os;
}

vector<Vertex> vertexs;//���ڴ������ͼ�Ķ���
vector<int> EKQue;//��¼��ǰѰ�ҵ�����·��
queue<int> myQueue;//��¼Ѱ�ҹ����б��������ж���



bool *isVisited;//��ע�����Ƿ񱻷���
int *preVertex;//һά���飬��С�붥�������ͬ����¼�����ʶ������ӱߵ����

void bfs(int, bool&);//���ù�������㷨Ѱ������·��
void backFlow(Vertex &v, int &minFlow);

//���붥�㼰�����ӵı� ��-1��������һ�����������
void bulidGraph(int n)
{
	for (int i = 0; i < n; i++)
	{
		int temp;
		Vertex tempVertex;
		vector<Edge> tempEdge;
		cout << "�����" << i << "�����㣺" << endl;
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
	bool ExistPath = 1;//��ʾ�ҵ���һ����Դ�㵽����·��
	while (ExistPath)
	{
		ExistPath = 0;
		bfs(0, ExistPath);
		for (int i = 0; i < n; i++)
			isVisited[i] = 0;
		for (int i = 0; i < n; i++)
			preVertex[i] = 0;
		//cout << "��������" << endl;
		queue<int> empty;
		swap(empty, myQueue);
	}
	//for (int i = 0; i < vertexs.size(); i++)
		//cout << vertexs[i];
}

//Ѱ������·��
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
				if (!vertexs[vertexs[p].edge[i].endVertex].edge.size())//����ҵ��˻��
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

					myQueue.push(vertexs[p].edge[i].endVertex);//���ö����������
					preVertex[vertexs[p].edge[i].endVertex] = p;
					if(vertexs[p].edge[i].isRevEdge==1&& vertexs[p].edge[i].flow!=0)
						EKQue.push_back(vertexs[p].edge[i].endVertex);//���ö��������
					else if(vertexs[p].edge[i].isRevEdge == 0)
						EKQue.push_back(vertexs[p].edge[i].endVertex);

					isVisited[vertexs[p].edge[i].endVertex] = 1;//���õ��עΪ�ѷ���
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
	//������
	vertexs[index].edge[i].flow += minFlow;

	//��ӷ����
	if (vertexs[index].index != 0 && vertexs[index].edge[i].isRevEdge == 0)
	{

		for (int i = 0; i < v.edge.size(); i++)
		{
			if (v.edge[i].endVertex == index)//����Ѿ����������ķ����
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

#endif // !MAXFLOWANDMINCUT_H

