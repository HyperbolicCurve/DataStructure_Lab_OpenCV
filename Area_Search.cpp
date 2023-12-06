//
// Created by Hyper on 02/12/2023.
//

#include "Area_Search.h"
#include "Huffman.h"

Area::Area(const Mat& markers)
{
	int num;
	number = 1;
	area_list = (int*)malloc(sizeof(int));
	area_list[0] = 0;
	for (int i = 0; i < markers.rows; i++)
	{
		for (int j = 0; j < markers.cols; j++)
		{
			num = markers.at<int>(i, j) + 1;
			if (num > 0)
			{
				if (num > number)
				{
					area_list = (int*)realloc(area_list, sizeof(int) * num);
					for (int k = number; k < num; k++)
					{
						area_list[k] = 0;
					}
					number = num;
				}
				area_list[num - 1]++;
			}
		}
	}
	area_size = (int*)malloc(sizeof(int) * number);
	for (int i = 0; i < number; i++)
	{
		area_size[i] = i;
	}
}

Area::~Area()
{
	free(area_list);
	free(area_size);
}

// 维护堆的性质
void Area::heapify(int len, int i)
{
	int largest = i;
	int lson = i * 2 + 1;
	int rson = i * 2 + 2;

	if (lson < len && area_list[area_size[largest]] < area_list[area_size[lson]])
		largest = lson;
	if (rson < len && area_list[area_size[largest]] < area_list[area_size[rson]])
		largest = rson;
	if (largest != i)
	{
		swap(&area_size[largest], &area_size[i]);
		heapify(len, largest);
	}
}

// 堆排序入口
void Area::heapSort()
{
	int i;
	// 建堆
	for (i = number / 2 - 1; i >= 0; i--)
		heapify(number, i);

	// 排序
	for (i = number - 1; i > 0; i--)
	{
		swap(&area_size[i], &area_size[0]);
		heapify(i, 0);
	}
	// 输出排序结果
	// for (i = 0;i < number;i++)
	//     cout << area_list[area_size[i]] << " ";
	// 提示最大面积和最小面积
	cout << "The largest area is " << area_list[area_size[number - 1]] << "." << endl;
	for(i = 0; i < number; i++)
	{
		if(area_list[area_size[i]] > 0)
		{
			cout << "The smallest area is " << area_list[area_size[i]] << "." << endl;
			break;
		}
	}
}

// 查找一定面积范围的区域
bool Area::search()
{
	// 输入面积范围
	int upper, lower;
	cout << "Input the upper bound of area : ";
	cin >> upper;
	while (cin.fail() || upper <= 0)
	{
		cin.clear();
		cin.ignore();
		cout << "Number not valid, please input again : ";
		cin >> upper;
	}
	cout << "Input the lower bound of area : ";
	cin >> lower;
	while (cin.fail() || lower <= 0 || lower > upper)
	{
		cin.clear();
		cin.ignore();
		cout << "Number not valid, please input again : ";
		cin >> lower;
	}
	// 二分查找
	int max = 0, min = 0;
	bool find = false;
	// 找上确界
	int left = 0, right = number - 1, mid;
	while (left <= right)
	{
		mid = (left + right) / 2;
		if (area_list[area_size[mid]] > upper)
			right = mid - 1;
		else if (area_list[area_size[mid]] < upper)
			left = mid + 1;
		else
		{
			max = mid;
			find = true;
			break;
		}
	}
	if (!find)
		max = right;
	// 找下确界
	left = 0, right = number - 1;
	while (left <= right)
	{
		mid = (left + right) / 2;
		if (area_list[area_size[mid]] > lower)
			right = mid - 1;
		else if (area_list[area_size[mid]] < lower)
			left = mid + 1;
		else
		{
			min = mid;
			find = true;
			break;
		}
	}
	if (!find)
		min = left;
	if(min > max)
	{
		cout << "No area in this range." << endl;
		return false;
	}
	// 存储结果
	for (int i = min; i <= max; i++)
	{
		scope.push_back(area_size[i]);
	}
	return true;
}
// 哈夫曼编码
Mat Area::HuffmanCoding()
{
	HuffmanTree H;
	int n = scope.size();
	int arr[n];

	// 将scope中的面积值拷贝到arr数组
	for (int i = 0; i < n; i++)
	{
		arr[i] = area_list[scope[i]];
	}
	InitTree(H, n, arr);
	CreateHuffmanTree(H, n);

	vector<string> paths;
	vector<int> vals;
	GeneratePaths(H, 2 * n - 1, paths, vals);
	auto pic = Draw(paths, vals, 30);
	free(H);
	return pic;
}

// 随机产生高亮颜色
vector<Vec3b> highlightColorLab(int total)
{
	srand((unsigned)time(NULL)); // 用时间做种，每次产生随机数不一样
	vector<Vec3b> colorLab;
	int randomIndex, randomValue;
	int value1, value2, value3;
	for (int i = 0; i < total; i++)
	{
		randomIndex = rand() % 3; // 生成0~2之间的随机数
		randomValue = rand() % 256; // 生成0~255之间的随机数
		// 根据随机生成的索引，将0、255和randomValue赋值给三个变量
		switch (randomIndex)
		{
			case 0:
				value1 = 0;
				value2 = 255;
				value3 = randomValue;
				break;
			case 1:
				value1 = 255;
				value2 = 0;
				value3 = randomValue;
				break;
			case 2:
				value1 = randomValue;
				value2 = 0;
				value3 = 255;
				break;
		}
		colorLab.push_back(Vec3b(value1, value2, value3));
	}

	return colorLab;
}

// 交换两个数
void swap(int* a, int* b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}
// 高亮标记指定范围的区域
void Area::mark(const Mat& markers, Mat& filled, const Vector2 samples[])
{
	int index;
	vector<int>::iterator iter;
	vector<Vec3b> colorLab = highlightColorLab(scope.size());
	for (int i = 0; i < markers.rows; i++)
	{
		for (int j = 0; j < markers.cols; j++)
		{
			index = markers.at<int>(i, j);
			if (index == -1)
			{
				filled.at<Vec3b>(i, j) = Vec3b(255, 255, 255);
				continue;
			}
			iter = find(scope.begin(), scope.end(), index);
			if (iter != scope.end())
			{
				filled.at<Vec3b>(i, j) = colorLab[iter - scope.begin()];
			}
			else
			{
				filled.at<Vec3b>(i, j) = Vec3b(0, 0, 0);
			}
		}
	}
	// 标记面积值
	for (int i = 0;i < scope.size();i++)
	{
		if (scope[i] <= 0)
			continue;
		auto p = samples[scope[i] - 1];
		circle(filled, Point(p.x, p.y), 2, Scalar::all(0), -1, 8, 0);
		putText(filled, to_string(area_list[scope[i]]), Point(p.x, p.y), FONT_HERSHEY_SIMPLEX, 0.3, CV_RGB(0, 0, 0), 1.8);
	}
}