//
// Created by Hyper on 12/10/2023.
//

#include "4_Color.h"

// 测试函数
int LabelTest(int rows, int cols, const cv::Mat& watershedResult)
{
	return watershedResult.at<int>(rows, cols);
}

// 构建邻接表
std::unordered_map<int, Node_1> BuildAdjacencyList(const cv::Mat& watershedResult)
{
    std::unordered_map<int, Node_1> adjacencyList;
    // 遍历图像像素,如果像素值非-1，则计算其面积，将其加入邻接表
	// 如果像素值为-1，则进行邻接关系的检查
    for (int y = 0; y < watershedResult.rows; y++) {
        for (int x = 0; x < watershedResult.cols; x++) {
            if(watershedResult.at<int>(y, x) != -1) {
				int label = watershedResult.at<int>(y, x);
				if (adjacencyList.find(label) == adjacencyList.end()) {
					Node_1 node;
					node.label = label;
					node.area = 1;
					node.color = -1;
					adjacencyList[label] = node;
				} else {
					adjacencyList[label].area++;
				}
			} else {
				// 检查当前像素的上下左右像素，如果有两个以上的像素值不为-1，且不相等，则将其加入邻接表
	            // 使用一个数组来保存邻接的标签，避免重复的if语句
	            int labels[4] = {-1, -1, -1, -1};

	            const int DIRS = 4;

				// 检查四个方向并填充标签数组
	            if (y - 1 >= 0) labels[0] = watershedResult.at<int>(y - 1, x);
	            if (y + 1 < watershedResult.rows) labels[1] = watershedResult.at<int>(y + 1, x);
	            if (x - 1 >= 0) labels[2] = watershedResult.at<int>(y, x - 1);
	            if (x + 1 < watershedResult.cols) labels[3] = watershedResult.at<int>(y, x + 1);

				// 使用两个嵌套循环来对所有可能的标签对进行检查
	            for (int i = 0; i < DIRS - 1; ++i) {
		            for (int j = i + 1; j < DIRS; ++j) {
			            if (labels[i] != -1 && labels[j] != -1 && labels[i] != labels[j]) {
				            adjacencyList[labels[i]].neighbors.insert(labels[j]);
				            adjacencyList[labels[j]].neighbors.insert(labels[i]);
			            }
		            }
	            }
			}
        }
    }
    return adjacencyList;
}

// 判断某个节点是否安全
bool isSafeColor(std::unordered_map<int, Node_1>& AdjacencyList, int nodeLabel, int color) {
	const Node_1& node = AdjacencyList[nodeLabel];
	// 遍历所有邻接节点，如果有一个邻接节点的颜色与color相同，则返回false
	for (int neighborLabel : node.neighbors) {
		const Node_1& neighborNode = AdjacencyList[neighborLabel];
		if (neighborNode.color == color) {
			return false;
		}
	}
	return true;
}

// 寻找邻接表中度数最大的节点
int findMaxDegreeNode(std::unordered_map<int, Node_1>& AdjacencyList) {
	int maxDegree = 0;
	int maxDegreeNode = -1;
	// it是一个pair<int, Node_1>类型的迭代器
	for (auto it = AdjacencyList.begin(); it != AdjacencyList.end(); ++it) {
		if (it->second.neighbors.size() > maxDegree) {
			maxDegree = it->second.neighbors.size();
			maxDegreeNode = it->first;
		}
	}
	return maxDegreeNode;
}

void ColorGraph(std::unordered_map<int, Node_1>& AdjacencyList) {
	std::queue<int> q;
	int maxDegreeNode = findMaxDegreeNode(AdjacencyList);
	AdjacencyList[maxDegreeNode].color = -1;
	q.push(maxDegreeNode);

	while (!q.empty()) {
		int color;
		std::stack<Node_1> s;
		int nodeLabel = q.front();
		q.pop();
		Node_1 node = AdjacencyList[nodeLabel];

		if (AdjacencyList[nodeLabel].color != 0) {
			continue;
		}

		for (color = 1; color <= 4; ++color) {
			if (isSafeColor(AdjacencyList, nodeLabel, color)) {
				AdjacencyList[nodeLabel].color = color;
				// 压入栈中
				s.push(node);
				break;
			}
		}

		if (color == 4 && !isSafeColor(AdjacencyList, nodeLabel, color)) {
			while (!s.empty()) {
				int topColor = AdjacencyList[s.top().label].color;
				s.pop();
				if (topColor < 4 && isSafeColor(AdjacencyList, nodeLabel, topColor + 1)) {
					AdjacencyList[nodeLabel].color = topColor + 1;
					s.push(node);
					break;
				}
			}
		}

		for (int neighborLabel : AdjacencyList[nodeLabel].neighbors) {
			if (AdjacencyList[neighborLabel].color == -1) {
				AdjacencyList[neighborLabel].color = 0;
				q.push(neighborLabel);
			}
		}
	}
}


