#include <iostream>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>   
#include "opencv2/legacy/legacy.hpp" 
#include <string>
#include <vector>
#include "MaxFlowAndMinCut.h"
using namespace std;
using namespace cv;

string g_sMainWindowName = "Mouse operation";

bool   g_bLeftButtonDown = false;
bool   g_bRightButtonDown = false;

vector<Point> g_vpLinePoints;
vector<Point> g_backLinePoints;
Point g_pPrevious; // previous point position
Point g_pCurrent; // current mouse position

//�û���ǵĶ��󼰱�������ֵ
vector<int> objPixelValue;
vector<int> bkgPixelValue;

int objOrBkgProbability[256][2];
float keno = 1.0f;//��
float K = 0.0f;

struct Pixel
{
	int value;//����ֵ
	float edgeUp = -1;
	float edgeDown = -1;
	float edgeLeft = -1;
	float edgeRight = -1;
};
ostream& operator<<(ostream &os, const Pixel &obj)
{
	cout << "up" << '\t' << obj.edgeUp << endl;
	cout << "down" << '\t' << obj.edgeDown << endl;
	cout << "left" << '\t' << obj.edgeLeft << endl;
	cout << "Right" << '\t' << obj.edgeUp << endl;

	cout << endl;
	return os;
};
Pixel **Link;//�������ص��Ȩ��ͼ

void drawResult(int row, int column)
{
	Mat result = Mat(row, column, CV_8UC3);
	for (int i = 0; i < result.rows; i++)
		for (int j = 0; j < result.cols; j++)
			result.at<Vec3b>(i, j) = Vec3b(255, 255, 255);


	for (int i = 0; i < EKQue.size(); i++)
	{
		int index = EKQue[i] - 1;
		result.at<Vec3b>(index / column, index%column) = Vec3b(0, 0, 0);
	}
	imshow("result", result);
	/*while (1)
	{
		if (waitKey(10) == 27)
		{
			return;
		}
	}*/
	waitKey(500);

}

void calculateWeight(Mat OrigImage)
{
	Mat image;
	cvtColor(OrigImage, image, CV_BGR2GRAY);
	int column = image.cols;
	int row = image.rows;

	Link = new Pixel*[row];
	for (int i = 0; i < row; i++)
		Link[i] = new Pixel[column];

	//ȡ��ͼ�������е�����ֵ
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < column; j++)
		{
			int grayValue = image.at<uchar>(i, j);
			Link[i][j].value = grayValue;
		}
	}
	cout << endl << endl;

	Mat result = Mat(row, column, CV_8UC3);
	cvtColor(result, result, CV_BGR2GRAY);
	for (int i = 0; i < result.rows; i++)
		for (int j = 0; j < result.cols; j++)
			result.at<uchar>(i, j) = Link[i][j].value;


	imshow("result", result);
	waitKey(500);

	//�����ڽ��ڵ��Ȩ��
	for (int i = 0; i < row; i++)
		for (int j = 0; j < column; j++)
		{
			float maxEdge = 0.0f;
			if (i != 0)
			{
				float exponent = Link[i - 1][j].value - Link[i][j].value;
				exponent = -(exponent*exponent) / (2 * keno*keno);
				exponent = exp(exponent);
				Link[i][j].edgeUp = exponent;
				maxEdge += exponent;
			}
			if (i != row - 1)
			{
				float exponent = Link[i + 1][j].value - Link[i][j].value;
				exponent = -(exponent*exponent) / (2 * keno*keno);
				exponent = exp(exponent);
				Link[i][j].edgeDown = exponent;
				maxEdge += exponent;
			}
			if (j != 0)
			{
				float exponent = Link[i][j - 1].value - Link[i][j].value;
				exponent = -(exponent*exponent) / (2 * keno*keno);
				exponent = exp(exponent);
				Link[i][j].edgeLeft = exponent;
				maxEdge += exponent;
			}
			if (j != column - 1)
			{
				float exponent = Link[i][j + 1].value - Link[i][j].value;
				exponent = -(exponent*exponent) / (2 * keno*keno);
				exponent = exp(exponent);
				Link[i][j].edgeRight = exponent;
				maxEdge += exponent;
			}

			K = K > maxEdge ? K : maxEdge;
		}
	cout << endl;


	//ͳ���û���ǵĶ���ͱ��������Ҷ�ֵ������
	int objGrayValueSum[256] = { 0 };
	int bkgGrayValueSum[256] = { 0 };
	for (int i = 0; i < 256; i++)
	{
		objGrayValueSum[i] = 1;
		bkgGrayValueSum[i] = 1;
	}

	for (int i = 0; i < objPixelValue.size(); i++)
		objGrayValueSum[objPixelValue[i]]++;
	for (int i = 0; i < bkgPixelValue.size(); i++)
		bkgGrayValueSum[bkgPixelValue[i]]++;


	cout << "���ͳ��ֵ��" << endl;
	for (int i = 0; i < 256; i++)
		cout << objGrayValueSum[i] << '\t';
	cout << endl;
	cout << endl;
	for (int i = 0; i < 256; i++)
		cout << bkgGrayValueSum[i] << '\t';
	cout << endl;


	for (int i = 0; i < 256; i++)
	{


		float po, pb;
		po = objGrayValueSum[i] / float(objGrayValueSum[i] + bkgGrayValueSum[i]);
		pb = bkgGrayValueSum[i] / float(objGrayValueSum[i] + bkgGrayValueSum[i]);
		float total = po + pb;
		po /= total;
		pb /= total;
		po = -log(po);
		pb = -log(pb);
		po = po * 10000000;
		pb = pb * 10000000;
		if (total != 0.0f)
		{
			objOrBkgProbability[i][0] = pb;
			objOrBkgProbability[i][1] = po;
		}
	}

}

void constructGraph(Mat image)
{
	calculateWeight(image);
	int column = image.cols;
	int row = image.rows;
	int total = row * column;
	//����Դ��
	Vertex source;
	source.index = 0;
	int count = 1;//��¼���ص�����
	vertexs.push_back(source);
	while (count <= row * column)
	{
		float lam = 1.0f;
		//����Դ��ı�
		Edge tempEdge;
		tempEdge.endVertex = count;
		tempEdge.weight = (objOrBkgProbability[Link[(count - 1) / column][(count - 1) % column].value][0]);
		vertexs[0].edge.push_back(tempEdge);


		//����count������ת��Ϊ����
		Vertex tempVertex;
		tempVertex.index = count;
		tempEdge.endVertex = total + 1;
		tempEdge.weight = (objOrBkgProbability[Link[(count - 1) / column][(count - 1) % column].value][1]);
		tempVertex.edge.push_back(tempEdge);
		if (Link[(count - 1) / column][(count - 1) % column].edgeLeft != -1.0f)
		{
			int t = Link[(count - 1) / column][(count - 1) % column].edgeLeft * 10000000;
			if (t)
			{
				tempEdge.endVertex = count - 1;
				tempEdge.weight = t;
				tempVertex.edge.push_back(tempEdge);
			}
		}
		if (Link[(count - 1) / column][(count - 1) % column].edgeRight != -1.0f)
		{
			int t = Link[(count - 1) / column][(count - 1) % column].edgeRight * 10000000;
			if (t)
			{
				tempEdge.endVertex = count + 1;
				tempEdge.weight = t;
				tempVertex.edge.push_back(tempEdge);
			}
		}
		if (Link[(count - 1) / column][(count - 1) % column].edgeUp != -1.0f)
		{
			int t = Link[(count - 1) / column][(count - 1) % column].edgeUp * 10000000;
			if (t)
			{
				tempEdge.endVertex = count - column;
				tempEdge.weight = t;
				tempVertex.edge.push_back(tempEdge);
			}
		}
		if (Link[(count - 1) / column][(count - 1) % column].edgeDown != -1.0f)
		{
			int t = Link[(count - 1) / column][(count - 1) % column].edgeDown * 10000000;
			if (t)
			{
				tempEdge.endVertex = count + column;
				tempEdge.weight = t;
				tempVertex.edge.push_back(tempEdge);

			}
		}
		vertexs.push_back(tempVertex);
		count++;
	}

	//���ӻ��
	Vertex end;
	end.index = total + 1;
	vertexs.push_back(end);

	//����Ԥ�ȱ��Ϊ��������ص���Դ��֮��ߵ�Ȩ�أ���ֹ������Դ���и�
	for (int i = 0; i < g_vpLinePoints.size(); i++)
	{
		int index = g_vpLinePoints[i].y*column + g_vpLinePoints[i].x;
		vertexs[0].edge[index].weight = (1 + K) * 10000000;
		vertexs[index + 1].edge[0].weight = 0;
	}

	//����Ԥ�ȱ��Ϊ���������ص�����֮��ߵ�Ȩ�أ���ֹ���������и�
	for (int i = 0; i < g_backLinePoints.size(); i++)
	{
		int index = g_backLinePoints[i].y*column + g_backLinePoints[i].x + 1;
		vertexs[index].edge[0].weight = (1 + K) * 10000000;
		vertexs[0].edge[index - 1].weight = 0;
	}
	//for(int i=0;i<vertexs.size();i++)
		//cout << vertexs[i];
	EKMaxFlow(total + 2);
	drawResult(row, column);
	//cout << "��������" << endl;
	/*for(int i=0;i<vertexs.size();i++)
		cout << vertexs[i];*/
	for (int i = 0; i < row; i++)
		delete[] Link[i];
	delete[] Link;
}


