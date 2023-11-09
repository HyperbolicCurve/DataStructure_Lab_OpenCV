//
// Created by Hyper on 12/10/2023.
//

#ifndef OPENCV_4_COLOR_H
#define OPENCV_4_COLOR_H
#include <unordered_map>
#include <queue>
#include <vector>
#include <stack>
#include <opencv2/opencv.hpp>

// 定义图节点
struct Node_1 {
    int label;                 // 区域标签
	int area;                  // 区域面积
	int color;                 // 区域颜色
    std::set<int> neighbors;    // 与其它区域的邻接关系
};

// 测试函数
int LabelTest(int rows, int cols, const cv::Mat& watershedResult);

// 构建邻接表
std::unordered_map<int, Node_1> BuildAdjacencyList(const cv::Mat& watershedResult);
// 判断某个节点是否安全
bool IsSafe(const Node_1& node, const std::unordered_map<int, Node_1>& adjacencyList);
// 寻找最大度数的节点
int findMaxDegreeNode(std::unordered_map<int, Node_1>& AdjacencyList);
// 为图着色
void ColorGraph(std::unordered_map<int, Node_1>& AdjacencyList);

#endif //OPENCV_4_COLOR_H
