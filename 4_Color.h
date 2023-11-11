//
// Created by Hyper on 12/10/2023.
//
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2\imgproc\types_c.h>
#include <iostream>
#define COLORNUMBER 4

using namespace std;
using namespace cv;

class Vertex
{
private:
	int Available_Color_Num;    // 可选颜色数
	vector<bool> Available_Colors;      // 可选颜色
public:
	int label;      // 标签
	int color;      // 颜色
	vector<Vertex*> adjv;   // 相邻节点
	// 构造函数
	Vertex(int id) : label(id), color(0), Available_Colors(vector<bool>(COLORNUMBER + 1, true)), Available_Color_Num(COLORNUMBER) {}
	// 析构函数
	~Vertex() {}
	// 返回相邻节点数
	int degree() { return adjv.size(); }
	// 返回可选颜色数
	int colorAccessNumber() { return Available_Color_Num; }
	// 检查颜色是否可选
	bool checkColor(int color)
	{
		if (Available_Colors[color] == true) return true;
		else return false;
	}
	// 添加颜色
	void AddColor(int id)
	{
		Available_Colors[id] = true;
		Available_Color_Num++;
	}
	// 删除颜色
	void DeColor(int id)
	{
		Available_Colors[id] = false;
		Available_Color_Num--;
	}
	// 添加相邻节点
	void addAdjv(Vertex* v) { adjv.push_back(v); }
	// 检查是否有冲突
	bool conflict()
	{
		for (auto v : adjv)
		{
			if (color == v->color)// 如果相邻且颜色相同则返回true，不同返回false
			{
				return true;
			}
		}
		return false;
	}
};

class Graph
{
public:
	int ColoredNums;   // 已经上色的节点数
	bool finish;    // 是否完成
	int vernum;     // 节点数
	vector<Vertex*> vertices;   // 节点集
	vector<Vertex*> conflict_vertices;  // 冲突点集
	// 默认构造函数
	Graph();
	// 构造函数
	Graph(int vnum);
	// 析构函数
	~Graph();
	// 寻找度最大的节点
	Vertex* Maxdegree(int mincolor);
	// 寻找剩余可选颜色最少的节点
	Vertex* MaxResrict(Vertex* vertex, vector<Vertex*>& MRVRecover);
	// 恢复节点可涂颜色
	void MaxResrictVRecover(Vertex* vertex, const vector<Vertex*>& MRVRecover);
	// 更新冲突集，即与涂色失败的点相邻的点，递归直接回退到冲突点。
	void updateConflictVertices(Vertex* vertex);
	// 填充颜色
	void fillColor(Vertex* vertex);
	// 展示上色结果
	void Show4ColorImage(const Mat& markers, Mat& filled);
};

// 创建邻接表
Graph CreateAdjList(const Mat& markers, int vernum);
// 四色着色
void fourColor(const Mat& markers, Mat& filled, int vernum, clock_t & end);
// 随机着色
void randomColorDye(const Mat& markers, Mat& filled, int vernum);
// 最大度优先着色
class VertexCompare
{
public:
	bool operator()(const Vertex* v1, const Vertex* v2) const { return v1->label < v2->label; }
};