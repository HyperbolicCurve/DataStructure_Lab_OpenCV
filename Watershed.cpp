//
// Created by Hyper on 27/09/2023.
//
#include <iostream>               // C++输入/输出流
#include <opencv2/opencv.hpp>     // OpenCV计算机视觉库
#include "poisson.h"            // 包括一个自定义的头文件（假设它包含了相关的声明）
#include "4_Color.h"
#include <ctime>
#define desired_samples1 100000   // 所需样本数
using namespace cv;
using namespace std;

//函数：生成一个随机颜色，给定一个整数值
Vec3b RandomColor(int value);
//函数：根据整数值生成颜色
Vec3b FourColor(int value);
// 用于存储生成的样本的数组
Vector2 samples_[desired_samples1];

int main( )
{
    int num_samples = 0;
    clock_t start,end;
	//禁用OpenCV的日志输出，以减少控制台输出
    utils::logging::setLogLevel(utils::logging::LOG_LEVEL_SILENT);

     //从文件中读取一张图像
    Mat image = imread("E:\\RC.jpg");

    // 创建一个矩阵来存储分水岭算法的标记
    Mat marks(image.size(), CV_32S);
    marks = Scalar::all(0);

    // 调用一个自定义函数来使用泊松盘采样找到点
    findPoint(marks, samples_, &num_samples, &start);
    imshow("The Origin image", image);

    // 应用分水岭算法对图像进行分割
    watershed(image, marks);

 	// 构建邻接表
    std::unordered_map<int, Node_1> adjacencyList = BuildAdjacencyList(marks);

	// 使用回溯法为图着色
	ColorGraph(adjacencyList);

	// 打印每个区域的颜色
	for (auto& node : adjacencyList) {
		std::cout << "Label: " << node.first << " Color: " << node.second.color << std::endl;
	}

	// 创建一个用于分水岭算法结果的矩阵
	Mat afterWatershed;
    convertScaleAbs(marks, afterWatershed);
	//创建一个用于以随机颜色可视化分割区域的图像
    Mat PerspectiveImage = Mat::zeros(image.size(), CV_8UC3);
    for (int i = 0; i < marks.rows; i++) {
        for (int j = 0; j < marks.cols; j++) {
            int index = marks.at<int>(i, j);
            if (marks.at<int>(i, j) == -1) {
                PerspectiveImage.at<Vec3b>(i, j) = Vec3b(255, 255, 255); // 未标记区域使用白色
            } else {
                PerspectiveImage.at<Vec3b>(i, j) = RandomColor(index);
            }
        }
    }

    // 创建原始图像和分割结果的加权组合
    Mat wshed;
    addWeighted(image, 0.4, PerspectiveImage, 0.6, 0, wshed);

    // 在最终图像所分割的区域进行数字编号
    cv::Point point;
    for (int i = 0; i < num_samples; i++) {
        point.x = (int)(samples_[i].x * 600);
        point.y = (int)(samples_[i].y * 600);
        circle(wshed, point, 3, Scalar(0, 0, 255), -1, 8, 0);
        putText(wshed, to_string(i + 1), point, FONT_HERSHEY_SIMPLEX, 0.3, Scalar(255, 255, 255), 1);
    }
    imshow("Final", wshed);
    end = clock();
    cout << "Time "<<(double)(end-start)/CLOCKS_PER_SEC<<"s"<<endl;
    // 等待按键以保持窗口打开
    waitKey();
}

// 函数：根据整数值生成随机颜色
Vec3b RandomColor(int value)
{
    value = value % 255;  // 确保值在有效的颜色范围内（0-255）
    RNG rng;
    int aa = rng.uniform(0, value);
    int bb = rng.uniform(0, value);
    int cc = rng.uniform(0, value);
    return Vec3b(aa, bb, cc);
}

// 涂色函数，1-4分别代表四种颜色：红、绿、蓝、黄
Vec3b FourColor(int value)
{
	switch (value) {
		case 1:
			return Vec3b(0, 0, 255);    // 红色
		case 2:
			return Vec3b(0, 255, 0);    // 绿色
		case 3:
			return Vec3b(255, 0, 0);    // 蓝色
		case 4:
			return Vec3b(0, 255, 255);  // 黄色
		default:
			return Vec3b(0, 0, 0);      // 黑色
	}
}
