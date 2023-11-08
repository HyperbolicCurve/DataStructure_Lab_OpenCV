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
					adjacencyList[label] = node;
				} else {
					adjacencyList[label].area++;
				}
			} else {
				if(y - 1 >= 0 && x - 1 >= 0){
					int label_1 = watershedResult.at<int>(y, x - 1);
					int label_2 = watershedResult.at<int>(y - 1, x);
					if (label_2 != -1 && label_1 != -1 && label_1 != label_2){
						adjacencyList[label_1].neighbors.insert(label_2);
						adjacencyList[label_2].neighbors.insert(label_1);
					}
				}
			}
        }
    }
    return adjacencyList;
}
// 广度优先搜索
void BFS (const std::unordered_map<int, Node_1>& adjacencyList, int startLabel, std::unordered_map<int, std::vector<int>>& adjacencyRelations)
{
    std::queue<int> queue;
    std::unordered_map<int, bool> visited;

    queue.push(startLabel);
    visited[startLabel] = true;

    while (!queue.empty())
    {
        int label = queue.front();
        queue.pop();
        const Node_1& node = adjacencyList.at(label);

        for (int neighborLabel : node.neighbors)
        {
            if (!visited[neighborLabel])
            {
                queue.push(neighborLabel);
                visited[neighborLabel] = true;
                adjacencyRelations[label].push_back(neighborLabel);
            }
        }
    }
}