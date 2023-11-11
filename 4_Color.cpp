#include "4_Color.h"
#include <vector>
#include <set>

// 构造函数
Graph::Graph() :vernum(0), ColoredNums(0), finish(false)
{

}
// 构造函数
Graph::Graph(int vnum) :vernum(vnum), ColoredNums(0), finish(false)
{
	for (int i = 0; i < vnum;i++)
	{
		auto pv = new Vertex(i + 1);
		vertices.push_back(pv);
	}
}
// 析构函数
Graph::~Graph()
{
	for (auto pv : vertices)
		delete pv;
}
// 从剩余可选颜色最少的节点中选择度最大的节点
Vertex* Graph::Maxdegree(int mincolor)
{
	// 从剩余可选颜色最少的节点中选择度最大的节点
	vector<Vertex*> Maxdegree;
	for (int i = 0;i < vernum ;i++)
	{
		// 如果节点的可选颜色数等于最少可选颜色数，那么加入Maxdegree
		if (vertices[i]->colorAccessNumber() == mincolor && vertices[i]->color == 0)
		{
			Maxdegree.push_back(vertices[i]);
		}
	}
	// 按照节点的度数从大到小排序
	sort(Maxdegree.begin(), Maxdegree.end(), [](const Vertex* a, const Vertex* b) {return a->adjv.size() > b->adjv.size(); });
	// 选择度最大的节点
	Vertex* next = Maxdegree[0];
	// 如果度最大的节点的可选颜色数大于1，那么返回该节点
	return next;
}
// 从剩余可选颜色最少的节点中选择度最大的节点
Vertex* Graph::MaxResrict(Vertex* vertex, vector<Vertex*>& BackMaxRes)
{
	for (auto v : vertex->adjv)
	{
		if (v->checkColor(vertex->color))
		{
			BackMaxRes.push_back(v);// 保存当前节点的相邻节点
			v->DeColor(vertex->color);// 删除当前节点的相邻节点的可选颜色
			if (v->colorAccessNumber() == 0) return vertex;// 如果当前节点的相邻节点的可选颜色为0，那么当前节点也不可选
		}
	}
	int minColor = COLORNUMBER;
	Vertex* next = nullptr;
	for (auto v : vertices)
	{
		if (minColor >= v->colorAccessNumber() && v->color == 0)
		{
			minColor = v->colorAccessNumber();
			next = v;
		}
	}
	// 如果没有可选节点，那么返回nullptr
	if (next == nullptr) return next;
	next = Maxdegree(minColor);    // 从剩余可选颜色最少的节点中选择度最大的节点
	return next;
}
// 恢复节点可涂颜色
void Graph::MaxResrictVRecover(Vertex* vertex, const vector<Vertex*>& BackMaxRes)
{
	for (auto v : BackMaxRes)
		v->AddColor(vertex->color);
}
// 更新冲突集，即与涂色失败的点相邻的点，递归直接回退到冲突点。
void Graph::updateConflictVertices(Vertex* vertex)
{
	if (conflict_vertices.size() != 0)
		conflict_vertices.clear();
	for (auto v : vertex->adjv)
	{
		if (v->color != 0)     // 未上色的相邻点冲突为冲突点
		{
			conflict_vertices.push_back(v);
		}
	}
}

void Graph::fillColor(Vertex* vertex)
{
	// 如果所有节点都已经上色，那么返回
	if (ColoredNums >= vernum)
	{
		finish = true;
		return;
	}
	for (int i = 1;i <= COLORNUMBER;i++)
	{
		vertex->color = i;
		if (!vertex->conflict())    // 如果当前节点不冲突，那么继续填涂下一个节点
		{
			ColoredNums++;
			// 回溯最大限制的节点
			vector<Vertex*>BackMaxRes;
			auto next = MaxResrict(vertex, BackMaxRes);
			if (next == vertex)
			{
				// 如果当前节点的相邻节点的可选颜色为0，那么回退到上一个节点
				MaxResrictVRecover(vertex, BackMaxRes);
				ColoredNums--;
				vertex->color = 0;
				continue;
			}
			fillColor(next);    // 递归填涂下一个节点
			if (finish) return;
			if (find(conflict_vertices.begin(), conflict_vertices.end(), vertex) == conflict_vertices.end())
			{
				// 如果当前节点不是冲突点，那么回退到上一个节点
				MaxResrictVRecover(vertex, BackMaxRes);
				ColoredNums--;
				vertex->color = 0;
				return;
			}
			MaxResrictVRecover(vertex, BackMaxRes);
			ColoredNums--;
		}
		vertex->color = 0;
	}
	updateConflictVertices(vertex);
}

void Graph::Show4ColorImage(const Mat& markers, Mat& ColoredMap)
{
	Vec3b colortab[COLORNUMBER] = { Vec3b(0, 0, 255), Vec3b(0, 255, 0), Vec3b(255, 0, 0), Vec3b(255, 255, 0) };
	for (int i = 0; i < markers.rows; i++)
	{
		for (int j = 0; j < markers.cols; j++)
		{
			int index = markers.at<int>(i, j);
			if (index == -1)
				ColoredMap.at<Vec3b>(i, j) = Vec3b(255, 255, 255);
			else if (index <= 0)
				ColoredMap.at<Vec3b>(i, j) = colortab[vertices[vernum - 1]->color];
			else
				ColoredMap.at<Vec3b>(i, j) = colortab[vertices[index - 1]->color - 1];
		}
	}
}

Graph CreateAdjList(const Mat& markers, int TotalNum)
{
	int TotalNum0 = TotalNum + 1;
	// 根据markers创建二维数组地图
	int markers_map[markers.rows][markers.cols];
	for (int i = 0;i < markers.rows;i++)
	{
		for (int j = 0;j < markers.cols;j++)
		{
			// 把0换成vertnum + 1
			if (markers.at<int>(i, j) == 0)
			{
				TotalNum = TotalNum0;
				markers_map[i][j] = TotalNum;
			}
			else
				markers_map[i][j] = markers.at<int>(i, j);
		}
	}
	Graph G(TotalNum);
	// 遍历标记矩阵的每个元素，检查其周围八个元素是否有不同的标签值，并将它们作为图中的边
	for (int i = 1; i < markers.rows - 1; i++)
	{
		for (int j = 1; j < markers.cols - 1; j++)
		{
			// 获取当前元素的标签值
			int curr = markers.at<int>(i, j);
			// 跳过不是-1的元素
			if (curr == -1)
			{
				// 获取周围八个元素的标签值
				vector<int> neighbors;
				neighbors.push_back(markers_map[i - 1][j - 1]);
				neighbors.push_back(markers_map[i - 1][j]);
				neighbors.push_back(markers_map[i - 1][j + 1]);
				neighbors.push_back(markers_map[i][j - 1]);
				neighbors.push_back(markers_map[i][j + 1]);
				neighbors.push_back(markers_map[i + 1][j - 1]);
				neighbors.push_back(markers_map[i + 1][j]);
				neighbors.push_back(markers_map[i + 1][j + 1]);
				// 去除重复的标签值
				set<int> s(neighbors.begin(), neighbors.end());
				s.erase(curr);
				set<int>::iterator key;
				// 如果周围有不同的标签值，那么当前元素是一个边界像素
				if (s.size() > 1)
				{
					for (key = s.begin(); key != s.end(); key++)
					{
						for (auto value = s.begin(); value != s.end(); value++)
						{
							if (value == key) continue;
							else G.vertices[*key - 1]->addAdjv(G.vertices[*value - 1]);
						}
					}
				}
			}
		}
	}
	for (auto v : G.vertices)
	{
		set<Vertex*, VertexCompare> s(v->adjv.begin(), v->adjv.end());
		v->adjv.assign(s.begin(), s.end());
	}
	return G;
}

void fourColor(const Mat& markers, Mat& ColoredMap, int TotalNum, clock_t & end)
{
	clock_t t2;
	t2 = clock();
	Graph G = CreateAdjList(markers, TotalNum);
	// 从度最大选择开始
	auto start = G.Maxdegree(COLORNUMBER);
	// 着色
	G.fillColor(start);
	// 四色法染色
	G.Show4ColorImage(markers, ColoredMap);
	// 显示结果
	imshow("ColoredMap", ColoredMap);
	end = clock();
	cout << "Four_Color Time " << (double)(end - t2) / CLOCKS_PER_SEC << "s" << endl;
	waitKey();
}