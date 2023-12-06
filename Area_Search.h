//
// Created by Hyper on 02/12/2023.
//

#ifndef OPENCV_AREA_SEARCH_H
#define OPENCV_AREA_SEARCH_H
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2\imgproc\types_c.h>
#include <iostream>
#include "poisson.h"


using namespace std;
using namespace cv;

class Area
{
	int number;
	int* area_list;                                 //存储每个区域的面积
	int* area_size;                                 //面积升序排列
	vector<int> scope;                          //存储指定范围的区域
public:
	Area(const Mat& markers);
	~Area();
	int getAreaNumber() { return number; }
	void heapify(int len, int i);
	void heapSort();
	bool search();
	void mark(const Mat& markers, Mat& filled, const Vector2 samples[]);
	Mat HuffmanCoding();

};
vector<Vec3b> highlightColorLab(int total);
void swap(int* a, int* b);
#endif //OPENCV_AREA_SEARCH_H
