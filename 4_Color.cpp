//
// Created by Hyper on 12/10/2023.
//

#include "4_Color.h"

// 构建邻接表
std::unordered_map<int, Node_1> BuildAdjacencyList(const cv::Mat& watershedResult)
{
    std::unordered_map<int, Node_1> adjacencyList;
    // 遍历图像像素
    for (int y = 0; y < watershedResult.rows; y++) {
        for (int x = 0; x < watershedResult.cols; x++) {
            int label = watershedResult.at<int>(x, y);
            std::unordered_map<int, bool> processedLabels; // 用于跟踪已处理的label
            if (label == -1 ) {
                Node_1& currentNode = adjacencyList[label];
                // 检查像素周围的像素
                int nx1 = x - 1;
                int nx2 = x + 1;
                int ny1 = y + 1;
                int ny2 = y + 1;
                if(nx1 >= 0 && nx2 < watershedResult.cols &&  watershedResult.at<int>(nx1, y) != watershedResult.at<int>(nx2, y))
                {
                    Node_1& currentNode1 = adjacencyList[watershedResult.at<int>(nx1, y)];
                    Node_1& currentNode2 = adjacencyList[watershedResult.at<int>(nx2, y)];
                    //判断currentNode1的邻居中是否含有currentNode2的标签，如果没有，添加到currentNode1的邻居
                    if (std::find(currentNode1.neighbors.begin(), currentNode1.neighbors.end(), currentNode2.label) == currentNode1.neighbors.end()) {
                        currentNode1.neighbors.push_back(currentNode2.label);
                    }
                    if (std::find(currentNode2.neighbors.begin(), currentNode2.neighbors.end(), currentNode1.label) == currentNode2.neighbors.end()) {
                        currentNode2.neighbors.push_back(currentNode1.label);
                    }
                }
                if(ny1 >= 0 && ny2 < watershedResult.rows && watershedResult.at<int>(x, ny1) != watershedResult.at<int>(x, ny2))
                {
                    Node_1& currentNode1 = adjacencyList[watershedResult.at<int>(x, ny1)];
                    Node_1& currentNode2 = adjacencyList[watershedResult.at<int>(x, ny2)];
                    //判断currentNode1的邻居中是否含有currentNode2的标签，如果没有，添加到currentNode1的邻居
                    if (std::find(currentNode1.neighbors.begin(), currentNode1.neighbors.end(), currentNode2.label) == currentNode1.neighbors.end()) {
                        currentNode1.neighbors.push_back(currentNode2.label);
                    }
                    if (std::find(currentNode2.neighbors.begin(), currentNode2.neighbors.end(), currentNode1.label) == currentNode2.neighbors.end()) {
                        currentNode2.neighbors.push_back(currentNode1.label);
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