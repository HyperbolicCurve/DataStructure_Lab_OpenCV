//
// Created by Hyper on 11/11/2023.
//

#ifndef OPENCV_HUFFMAN_H
#define OPENCV_HUFFMAN_H
#include <vector>
#include <string>
#include <opencv2/core.hpp>
using namespace std;

typedef struct
{
	int weight;//结点的权值
	int depth;//结点的深度
	int parent, lchild, rchild;//结点的双亲，左孩子，右孩子的下标
}HTNode, * HuffmanTree;//动态分配数组存储哈夫曼树


void InitTree(HuffmanTree& H, int n, int arr[]);
void Select(HuffmanTree H, int n, int& s1, int& s2);
void CreateHuffmanTree(HuffmanTree& H, int n);
void CreatHuffmanCode(HuffmanTree H, int n);

int GetFlag(HuffmanTree H, int node);
int GetCenterXPos(int h, int n, int size);
void GeneratePaths(HuffmanTree H, int node, vector<string>& paths, vector<int>& vals);
void DrawOneNodeByPath(cv::Mat& pic, int height, string path, int val, int size);
void DrawOneNode(cv::Mat& pic, int height, int i, int j, int val, int size, int flag);
cv::Mat Draw(const vector<string>& paths, const vector<int>& vals, int size);
#endif //OPENCV_HUFFMAN_H
