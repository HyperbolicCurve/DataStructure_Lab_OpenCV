//
// Created by Hyper on 27/09/2023.
//
#include <iostream>               // C++输入/输出流
#include <opencv2/opencv.hpp>     // OpenCV计算机视觉库
#include "poisson.h"            // 包括一个自定义的头文件（假设它包含了相关的声明）
#include <ctime>
using namespace cv;
using namespace std;

// 函数：生成一个随机颜色，给定一个整数值
Vec3b RandomColor(int value);

//计算main函数运行时间

int main( int argc, char* argv[] )
{
    clock_t start,end;
    // 禁用OpenCV的日志输出，以减少控制台输出
    utils::logging::setLogLevel(utils::logging::LOG_LEVEL_SILENT);

    start = clock();
    // 从文件中读取一张图像
    Mat image = imread("E:\\RC.jpg");
    imshow("The Origin image", image);

    // 将图像转换为灰度图
    //Mat imageGray;
    //cvtColor(image, imageGray, COLOR_RGB2GRAY);

    // 对灰度图应用高斯模糊
    //GaussianBlur(imageGray, imageGray, Size(5, 5), 2);
    //imshow("Gray image", imageGray);

    // 对灰度图应用Canny边缘检测
    //Canny(imageGray, imageGray, 80, 150);
    //imshow("Canny edge-detected image", imageGray);

    // 创建一个矩阵来存储分水岭算法的标记
    Mat marks(image.size(), CV_32S);
    marks = Scalar::all(0);

    // 调用一个自定义函数来使用泊松盘采样找到点
    findPoint(marks);

    // 应用分水岭算法对图像进行分割
    watershed(image, marks);

    // 创建一个用于分水岭算法结果的矩阵
    Mat afterWatershed;
    convertScaleAbs(marks, afterWatershed);
    //imshow("Watershed image", afterWatershed);

    // 创建一个用于以随机颜色可视化分割区域的图像
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
    //imshow("Final Watershed image", PerspectiveImage);

    // 创建原始图像和分割结果的加权组合
    Mat wshed;
    addWeighted(image, 0.4, PerspectiveImage, 0.6, 0, wshed);
    imshow("Final", wshed);
    end = clock();
    cout << "Time "<<(double)(end-start)/CLOCKS_PER_SEC<<endl;
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
