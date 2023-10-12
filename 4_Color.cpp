//
// Created by Hyper on 12/10/2023.
//

#include "4_Color.h"

std::unordered_map<int, Node> BuildAdjacencyList(const cv::Mat& watershedResult) {
    std::unordered_map<int, Node> adjacencyList;

    // 遍历图像像素
    for (int y = 0; y < watershedResult.rows; y++) {
        for (int x = 0; x < watershedResult.cols; x++) {
            int label = watershedResult.at<int>(y, x);
            Node& node = adjacencyList[label];
            node.label = label;

            // 检查周围像素的标签
            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    int nx = x + dx;
                    int ny = y + dy;
                    if (nx >= 0 && nx < watershedResult.cols && ny >= 0 && ny < watershedResult.rows) {
                        int neighborLabel = watershedResult.at<int>(ny, nx);
                        if (neighborLabel != label) {
                            node.neighbors.push_back(neighborLabel);
                        }
                    }
                }
            }
        }
    }
    return adjacencyList;
}

// 广度优先搜索
void BFS(const std::unordered_map<int, Node>& adjacencyList, int startLabel, std::unordered_map<int, std::vector<int>>& adjacencyRelations) {
    std::queue<int> queue;
    std::unordered_map<int, bool> visited;

    queue.push(startLabel);
    visited[startLabel] = true;

    while (!queue.empty()) {
        int label = queue.front();
        queue.pop();
        const Node& node = adjacencyList.at(label);

        for (int neighborLabel : node.neighbors) {
            if (!visited[neighborLabel]) {
                queue.push(neighborLabel);
                visited[neighborLabel] = true;
                adjacencyRelations[label].push_back(neighborLabel);
            }
        }
    }
}