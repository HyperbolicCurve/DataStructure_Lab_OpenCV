//
// Created by Hyper on 27/09/2023.
//

#ifndef OPENCV_POISSON_H
#define OPENCV_POISSON_H

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/utils/logger.hpp>
#include <cstdio>          // 标准输入/输出函数
#include <cmath>           // 数学函数
#include <cstdlib>         // 通用实用程序库
#include <iostream>         // 用于C++的输入/输出流
#include <random>           // 随机数生成
#include "poisson.h"

typedef struct {
    float x;
    float y;
} Vector2;

bool check_collision(Vector2 p, int index, int **grid, Vector2 samples[]);
int poisson_disk_sample(int desired_samples, int **grid, Vector2 samples[]);
int findPoint(cv::Mat marks, Vector2 samples[], int* num_samples);

#endif //OPENCV_POISSON_H
