#include <math.h>
#include <queue>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2\imgproc\types_c.h>
#include "Huffman.h"


void InitTree(HuffmanTree& H, int n, int arr[])//初始化哈夫曼树
{
	int i;
	H = (HTNode*)malloc(sizeof(HTNode) * (2 * n));//申请一个2*n的空间
	for (i = 0;i < 2 * n;i++)//利用循环对申请的空间进行初始化
		H[i].weight = H[i].depth = H[i].parent = H[i].lchild = H[i].rchild = 0;
	for (i = 0;i < n;i++)//利用循环对n个结点权值赋值
	{
		H[i + 1].weight = arr[i];
	}
}

void Select(HuffmanTree H, int n, int& s1, int& s2)//选择函数 ，输入要循环的总次数n，并返回其权值最小的两个数的结点序号s1和s2
{
	int i,//循环次数i
	a,//权值赋值变量
	b;//结点序号赋值变量
	for (i = 0;i < n;i++)//循环赋值
	{
		if (H[i + 1].parent == 0)//找出第一个parent是0的结点，对变量a,b进行赋值
		{
			a = H[i + 1].weight;
			b = i + 1;
			break;//循环终止条件
		}
	}
	for (i = 0;i < n;i++)//利用循环找出最小值
	{
		if (a > H[i + 1].weight && H[i + 1].parent == 0)// parent为0的结点的中的最小值， 赋值给a和b,b为序号
		{
			a = H[i + 1].weight;
			b = i + 1;
		}
		if (a == H[i + 1].weight && H[i + 1].parent == 0)//如果两个值相同，取深度小的一个
		{
			if (H[b].depth > H[i + 1].depth)
			{
				b = i + 1;
			}
		}
	}
	s1 = b;
	for (i = 0;i < n;i++)
	{
		if (H[i + 1].parent == 0 && (i + 1) != s1)//找出第一个parent是0且不是最小权值的结点，对变量a,b进行赋值
		{
			a = H[i + 1].weight;
			b = i + 1;
			break;
		}
	}
	for (i = 0;i < n;i++)//利用循环找出除s1的最小值
	{
		if (a > H[i + 1].weight && H[i + 1].parent == 0 && (i + 1) != s1)
		{
			a = H[i + 1].weight;
			b = i + 1;
		}
		if (a == H[i + 1].weight && H[i + 1].parent == 0 && (i + 1) != s1)
		{
			if (H[b].depth > H[i + 1].depth)
			{
				b = i + 1;
			}
		}
	}
	s2 = b;
	// 深度大的作为右节点
	auto temp = s1;
	if (H[s1].depth > H[s2].depth)
	{
		s1 = s2;
		s2 = temp;
	}

}

void CreateHuffmanTree(HuffmanTree& H, int n)//哈夫曼树的创建
{
	int s1, s2;//申请变量，方便接挑选函数返回的值
	int i;
	for (i = n + 1;i < 2 * n;i++)//通过n-1次的选择，删除，合并来构建哈夫曼树
	{
		Select(H, i - 1, s1, s2);//选出最小的两个数
		H[s1].parent = i;
		H[s2].parent = i;//得到新结点i，从森林中删除s1，s2，并将s1,s2的双亲域由0改成i
		H[i].lchild = s1;
		H[i].rchild = s2;//s1和s2分别作为i的左右孩子
		H[i].weight = H[s1].weight + H[s2].weight;//i的权值为左右孩子的权值之和
		H[i].depth = H[s1].depth > H[s2].depth ? H[s1].depth + 1 : H[s2].depth + 1;//i的深度为左右孩子的深度较大者加1
	}
}

void CreatHuffmanCode(HuffmanTree H, int n)//哈夫曼编码的输出，递归实现
{
	if (H[n].parent == 0)//递归终止条件
		return;
	CreatHuffmanCode(H, H[n].parent);//递归双亲结点
	if (H[H[n].parent].lchild == n)//如果n的双亲的左孩子等于n，则输出0，否则输出1
		printf("0");
	else
		printf("1");
}


int GetFlag(HuffmanTree H, int node)
{
	if (H[node].lchild == 0 && H[node].rchild == 0)
		return 0;
	else if (H[node].lchild != 0 && H[node].rchild == 0)
		return 1;
	else if (H[node].lchild == 0 && H[node].rchild != 0)
		return 2;
	else
		return 3;
}

int GetCenterXPos(int h, int n, int size)
{
	return pow(2, h) * (n + 0.5) * size;
}

void GeneratePaths(HuffmanTree H, int node, vector<string>& paths, vector<int>& vals)
{
	queue<pair<int, string>> q;
	q.push(pair<int, string>(node, ""));
	paths.push_back(to_string(GetFlag(H, node)) + "");
	vals.push_back(H[node].weight);
	while (!q.empty())
	{
		pair<int, string> p = q.front();
		int temp = p.first;
		string path = p.second;
		q.pop();
		if (H[temp].lchild != 0)
		{
			q.push(pair<int, string>(H[temp].lchild, path + "0"));
			paths.push_back(to_string(GetFlag(H, H[temp].lchild)) + path + "0");
			vals.push_back(H[H[temp].lchild].weight);
		}
		if (H[temp].rchild != 0)
		{
			q.push(pair<int, string>(H[temp].rchild, path + "1"));
			paths.push_back(to_string(GetFlag(H, H[temp].rchild)) + path + "1");
			vals.push_back(H[H[temp].rchild].weight);
		}
	}
}

void DrawOneNodeByPath(cv::Mat& pic, int height, string path, int val, int size)
{
	int flag = path.at(0) - '0';
	path = path.substr(1, path.size() - 1);
	int i = path.size();
	int j = 0;
	for (int k = path.size() - 1; k >= 0; k--)
	{
		j += pow(2, path.size() - k - 1) * (path[k] == '1' ? 1 : 0);
	}
	DrawOneNode(pic, height, i, j, val, size, flag);
}

void DrawOneNode(cv::Mat& pic, int height, int i, int j, int val, int size, int flag)
{
	/*1.计算圆心坐标*/
	int h = height - i;
	int x_pos = GetCenterXPos(h, j, size);
	cv::Point center(x_pos, i * size + size / 2);

	/*2.画连线*/
	if (i != height)
	{
		if (flag == 1 || flag == 3)
			cv::line(pic, center, cv::Point(GetCenterXPos(h - 1, j * 2, size), center.y + size), cv::Scalar(0, 0, 0));
		if (flag == 2 || flag == 3)
			cv::line(pic, center, cv::Point(GetCenterXPos(h - 1, j * 2 + 1, size), center.y + size), cv::Scalar(0, 0, 0));
	}

	/*3.画结点*/
	cv::circle(pic, center, size / 2, cv::Scalar(255, 255, 255), -1);  //画个白的圆
	cv::circle(pic, center, size / 2, cv::Scalar(0, 0, 0), 1);         //画个黑框圆

	/*4.写结点值*/
	string str = to_string(val);
	int base_line = 0;
	cv::Size text_size = cv::getTextSize(str, cv::FONT_HERSHEY_PLAIN, 0.6, 1, &base_line);
	// 只有叶子结点写节点值
	if (flag == 0)
		cv::putText(pic, str, center + cv::Point(-text_size.width / 2, text_size.height / 2), cv::FONT_HERSHEY_PLAIN, 0.6, cv::Scalar(0, 0, 0)
		);
}

cv::Mat Draw(const vector<string>& paths, const vector<int>& vals, int size)
{
	/*1.计算整颗树的高度*/
	int height = 0;
	for (int i = 0; i < paths.size(); i++)
	{
		if (paths.at(i).size() - 1 > height)
			height = paths.at(i).size() - 1;
	}
	/*2.生成纯白图片*/
	cv::Mat pic = 255 - cv::Mat::zeros(cv::Size(pow(2, height) * size, (height + 1) * size), CV_8UC1);
	/*3.画出每个结点和连线*/
	for (int i = 0; i < paths.size(); i++)
	{
		DrawOneNodeByPath(pic, height, paths.at(i), vals.at(i), size);
	}
	return pic;
}