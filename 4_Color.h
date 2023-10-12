//
// Created by Hyper on 12/10/2023.
//

#ifndef OPENCV_4_COLOR_H
#define OPENCV_4_COLOR_H
#include <unordered_map>
#include <queue>
#include <vector>
#include <opencv2/opencv.hpp>

// 定义图节点
struct Node {
    int label;                 // 区域标签
    std::vector<int> neighbors; // 与其它区域的邻接关系
};
// 构建邻接表
std::unordered_map<int, Node> BuildAdjacencyList(const cv::Mat& watershedResult);
// 广度优先搜索
void BFS(const std::unordered_map<int, Node>& adjacencyList, int startLabel, std::unordered_map<int, std::vector<int>>& adjacencyRelations);

#endif //OPENCV_4_COLOR_H
