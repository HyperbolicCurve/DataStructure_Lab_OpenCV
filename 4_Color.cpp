//
// Created by Hyper on 12/10/2023.
//

#include "4_Color.h"

// 测试函数】
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
					node.color = 0;
					adjacencyList[label] = node;
				} else {
					adjacencyList[label].area++;
				}
			} else {
				// 检查当前像素的上下左右像素，如果有两个以上的像素值不为-1，且不相等，则将其加入邻接表
	            // 使用一个数组来保存邻接的标签，避免重复的if语句
	            int labels[4] = {-1, -1, -1, -1};

				// 常量用于代替硬编码的数字，增加代码的可读性
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


//判断是否重色
bool IsSafe(int node, int color, const std::unordered_map<int, Node_1>& adjacencyList, const std::vector<int>& nodeColors) {
	for (int neighbor : adjacencyList.at(node).neighbors) {
		if (nodeColors[neighbor] == color) {
			return false;
		}
	}
	return true;
}


bool GraphColoringUtil(int node, std::vector<int>& nodeColors, const std::unordered_map<int, Node_1>& adjacencyList, std::vector<int>& visited) {
	// 如果所有节点都已经被着色，那么就完成了
	if (node == adjacencyList.size()) {
		return true;
	}

	for (int c = 1; c <= 4; c++) {
		// 检查颜色c是否可以分配给节点node
		if (IsSafe(node, c, adjacencyList, nodeColors)) {
			nodeColors[node] = c;  // 为节点node分配颜色c

			// 选择下一个节点
			int nextNode = -1;
			for (int neighbor :  adjacencyList.at(node).neighbors) {
				if (visited[neighbor] == 0) {
					nextNode = neighbor;
					break;
				}
			}

			// 如果没有下一个节点，说明所有节点已经着色
			if (nextNode == -1)
				return true;

			// 将当前节点标记为已访问
			visited[node] = 1;
			if (GraphColoringUtil(nextNode, nodeColors, adjacencyList, visited))
				return true;  // 递归着色剩余节点

			// 如果分配颜色c导致无解，则回溯
			visited[node] = 0;

			// 将当前节点标记为未访问
			nodeColors[node] = 0;
		}
	}
	// 如果没有颜色可以分配给node，返回false，需要回溯
	return false;
}

void GraphColoring(const std::unordered_map<int, Node_1>& adjacencyList, const int N) {
	std::vector<int> nodeColors(N + 1, 0);  // 创建颜色数组，大小为N+1，因为节点编号从1开始
	std::vector<int> visited(N + 1, 0);     // 记录节点是否访问过

	// 尝试为第一个节点着色
	if (!GraphColoringUtil(1, nodeColors, adjacencyList, visited)) {
		std::cout << "Solution does not exist";
		return;
	}
	// 打印解决方案
	for (int i = 1; i <= N; i++) {
		std::cout << "Node " << i << " has color " << nodeColors[i] << std::endl;
	}
}