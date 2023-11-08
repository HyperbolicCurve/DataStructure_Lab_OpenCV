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
struct Node_1 {
    int label;                  // 区域标签
	int area;                  // 区域面积
    std::set<int> neighbors;    // 与其它区域的邻接关系
};

// 测试函数
int LabelTest(int rows, int cols, const cv::Mat& watershedResult);

// 构建邻接表
std::unordered_map<int, Node_1> BuildAdjacencyList(const cv::Mat& watershedResult);

// 广度优先搜索
void BFS(const std::unordered_map<int, Node_1>& adjacencyList, int startLabel, std::unordered_map<int, std::vector<int>>& adjacencyRelations);

#endif //OPENCV_4_COLOR_H
