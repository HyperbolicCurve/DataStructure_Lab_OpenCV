//
// Created by Hyper on 27/09/2023.
//
#include <iostream>               // C++����/�����
#include <opencv2/opencv.hpp>     // OpenCV������Ӿ���
#include "poisson.h"            // ����һ���Զ����ͷ�ļ�����������������ص�������
#include "4_Color.h"
#include <ctime>
#define desired_samples1 100000   // ����������
using namespace cv;
using namespace std;

//����������һ�������ɫ������һ������ֵ
Vec3b RandomColor(int value);
Vector2 samples_[desired_samples1]; // ���ڴ洢���ɵ�����������
//����main��������ʱ��

int main( )
{
    int num_samples = 0;
    clock_t start,end;
	//����OpenCV����־������Լ��ٿ���̨���
    utils::logging::setLogLevel(utils::logging::LOG_LEVEL_SILENT);

     //���ļ��ж�ȡһ��ͼ��
    Mat image = imread("E:\\RC.jpg");

    // ����һ���������洢��ˮ���㷨�ı��
    Mat marks(image.size(), CV_32S);
    marks = Scalar::all(0);

    // ����һ���Զ��庯����ʹ�ò����̲����ҵ���
    findPoint(marks, samples_, &num_samples, &start);
    imshow("The Origin image", image);

    // Ӧ�÷�ˮ���㷨��ͼ����зָ�
    watershed(image, marks);

	// ���Ժ���
	cout << "LabelTest(122, 155, marks) = " << LabelTest(122, 155, marks) << endl;
 	// �����ڽӱ�
    std::unordered_map<int, Node_1> adjacencyList = BuildAdjacencyList(marks);
    // �����������ڽӹ�ϵ
	for (auto& node : adjacencyList) {
		std::cout << "Label: " << node.first << std::endl;
		std::cout << "Area: " << node.second.area << std::endl;
		std::cout << "Neighbors: ";
		for (int neighborLabel : node.second.neighbors) {
			std::cout << neighborLabel << " ";
		}
		std::cout << std::endl;
	}

	//����һ�����ڷ�ˮ���㷨����ľ���
	Mat afterWatershed;
    convertScaleAbs(marks, afterWatershed);
	//����һ�������������ɫ���ӻ��ָ������ͼ��
    Mat PerspectiveImage = Mat::zeros(image.size(), CV_8UC3);
    for (int i = 0; i < marks.rows; i++) {
        for (int j = 0; j < marks.cols; j++) {
            int index = marks.at<int>(i, j);
            if (marks.at<int>(i, j) == -1) {
                PerspectiveImage.at<Vec3b>(i, j) = Vec3b(255, 255, 255); // δ�������ʹ�ð�ɫ
            } else {
                PerspectiveImage.at<Vec3b>(i, j) = RandomColor(index);
            }
        }
    }

    // ����ԭʼͼ��ͷָ����ļ�Ȩ���
    Mat wshed;
    addWeighted(image, 0.4, PerspectiveImage, 0.6, 0, wshed);

    // ������ͼ�����ָ������������ֱ��
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
    // �ȴ������Ա��ִ��ڴ�
    waitKey();
}

// ��������������ֵ���������ɫ
Vec3b RandomColor(int value)
{
    value = value % 255;  // ȷ��ֵ����Ч����ɫ��Χ�ڣ�0-255��
    RNG rng;
    int aa = rng.uniform(0, value);
    int bb = rng.uniform(0, value);
    int cc = rng.uniform(0, value);
    return Vec3b(aa, bb, cc);
}
