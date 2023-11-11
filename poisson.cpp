#include "poisson.h"
#define desired_samples1 100000   // 所需样本数
#define TestNum 90                // 每个样本的测试次数
int grid_n;                       // 网格大小

double dx, inv_dx, radius;       // 用于圆盘采样的变量
// 检查样本之间是否发生碰撞的函数
bool check_collision(Vector2 p, int index, int **grid, Vector2 samples[]) {
    int x = index / grid_n;
    int y = index % grid_n;
    bool collision = false;

    for (int i = fmax(0, x - 2); i < fmin(grid_n, x + 3); i++) {
        for (int j = fmax(0, y - 2); j < fmin(grid_n, y + 3); j++) {
            if (grid[i][j] != -1) {
                Vector2 q = samples[grid[i][j]];
                double distance = sqrt( pow((q.x - p.x), 2) + pow((q.y - p.y), 2) );
                if (distance < radius - 1e-6) {
                    collision = true;
                }
            }
        }
    }
    return collision;
}

// 执行泊松盘采样的函数
int poisson_disk_sample(int desired_samples, int **grid, Vector2 samples[]) {
    samples[0].x = 0.5;
    samples[0].y = 0.5;
    grid[(int)(grid_n * 0.5)][(int)(grid_n * 0.5)] = 0;

    int head = 0;
    int tail = 1;

    while (head < tail && head < desired_samples) {
        Vector2 source_x = samples[head];
        head++;
        double theta = 0;
        for (int count = 0; count < TestNum; count++) {
            std::random_device rd;//随机数引擎
            std::mt19937 engine(rd());//随机数种子
            std::uniform_real_distribution<float> dist(0.0f, 1.0f);
            if (count == 0) {
                theta = dist(engine) * 2 * M_PI;
            } else {
                theta += 2 * M_PI / TestNum;
            }
            double offset = (1.0f) * radius;
            Vector2 new_x;
            new_x.x = source_x.x + offset * cos(theta);
            new_x.y = source_x.y + offset * sin(theta);
            int new_index = (int)(new_x.x * inv_dx) * grid_n + (int)(new_x.y * inv_dx);

            if (0 <= new_x.x && new_x.x < 1 && 0 <= new_x.y && new_x.y < 1) {
                bool collision = check_collision(new_x, new_index, grid, samples);
                if (!collision) {
                    samples[tail] = new_x;
                    grid[new_index / grid_n][new_index % grid_n] = tail;
                    tail++;
                }
            }
        }
    }
    return tail;
}

// 使用泊松盘采样找到点的函数
int findPoint(cv::Mat marks, Vector2 samples[], int* num_samples, clock_t *start) {
    int desired_samples ;
    std::cout << "Enter the number of the seeds:" << std::endl;
    std::cin >> desired_samples;
    *start = clock();
    double a ;
    a = pow(1 * 1.0 / desired_samples, 0.5);
    radius = a;
    dx = radius / sqrt(2);
    grid_n = (int)1 / dx;//网格大小
    inv_dx = grid_n;//

    // 为网格分配内存并初始化
    int **grid = new int *[grid_n];
    for (int i = 0; i < grid_n; i++) {
        grid[i] = new int[grid_n];
    }
    for (int i = 0; i < grid_n; i++) {
        for (int j = 0; j < grid_n; j++) {
            grid[i][j] = -1;
        }
    }

    // 执行泊松盘采样
    *num_samples = poisson_disk_sample(desired_samples, grid, samples);

    // 释放为网格分配的内存
    for (int i = 0; i < grid_n; i++) {
        delete[] grid[i];
    }
    delete[] grid;

    //显示生成的样本
    printf("Number Sample:%d\n", *num_samples);
    cv::Point point;
    for (int i = 0; i < *num_samples; i++) {
        point.x = (int)(samples[i].x * marks.cols);
        point.y = (int)(samples[i].y * marks.rows);
        marks.at<int>(point.y, point.x) = i + 1;
    }

    return 0;
}
