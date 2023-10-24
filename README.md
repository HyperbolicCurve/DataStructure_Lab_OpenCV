# DataStructure_Lab_OpenCV
This project contains my lab of Data Structure and Algorithm 2023 with OpenCV(cpp version).

School of Huazhong University of Science and Technology.

Department of Artificial Intelligence and Automation.

<font color="使用前记得修改CMakeLists.txt中OpenCV库的路径以及Watershed.Cpp中图片路径">这段文字将显示为红色字体。</font>

There are three projects to be completed:

## Project 1-均匀随机采样

使用基于种子标记的分水岭算法(`cv::Watershed`)对输入图像进行过分割。用户输入图像和整数K，要求程序自动计算
K个随机种子点，确保个种子带你之间的距离均>`sqrt(M*N/K)`(参考泊松圆盘采样+贪心策略），然后让程序在原图中
标出个种子点的位置以及编号，并采用半透明+随机着色的方式给出分水岭算法的可视化结果。

Using the Watershed algorithm based on seed markers (`cv::Watershed`) to over-segment an input image. 
The program allows users to input an image and an integer K, and it automatically calculates K random seed points, 
ensuring that the distance between each seed point is greater than `sqrt(M*N/K)` (referencing the Poisson disc sampling and greedy strategy). 
The program then marks the positions and assigns numbers to each seed point on the original image. The result of the Watershed algorithm is 
visualized with a semi-transparent effect and random coloring.

![image](https://github.com/Windblowing-china/DataStructure_Lab_OpenCV/assets/60433594/519d0267-103f-4681-a93a-854c51403a14)


## Project 2-四原图着色

使用邻接表统计分水岭结果中各区域的邻接关系，并采用四原色法(合理选择初始着色区域，并基于图的广度优先遍历，采用队列对其他待着色区域进行着色顺利
梳理，加速全图着色过程)，对分水岭的结果重新着色（使用堆栈+回溯策略，优化回退率）。

Using an adjacency list, the watershed results are analyzed to determine the neighboring relationships between different regions. The four-color theorem is applied, with a careful choice of the initial coloring regions. Based on a breadth-first search of the graph, a queue is used to efficiently color the remaining regions, speeding up the overall coloring process. This recolors the watershed results using a stack and a backtracking strategy to optimize the rate of backtracking.

![image](https://github.com/Windblowing-china/DataStructure_Lab_OpenCV/assets/60433594/eb382c5f-5f25-4995-84d9-86d7607165aa)


## Project 3-排序查找+哈夫曼

根据分水岭结果中各区域面积大小的堆排序结果，提示最大面积和最小面积，用户输入查找范围(面积下界和上界)，使用折半查找，程序对所有符合要求的分水岭结果
(标记区域面积)进行突出显示，并以这些高亮区域的面积大小作为权值，进行哈夫曼编码(考虑深度+递归策略),绘制该哈夫曼树。

Based on the heap-sorted results of the areas of regions in the watershed outcome, the program provides information about the largest and smallest areas. Users can input a search range defined by lower and upper area bounds. Using binary search, the program highlights all watershed results (marked region areas) that meet the specified criteria. It uses the highlighted region areas as weights to construct a Huffman tree, considering depth and recursive strategies. The program then visualizes the Huffman tree.

![image](https://github.com/Windblowing-china/DataStructure_Lab_OpenCV/assets/60433594/6119c262-70fc-459e-a337-4e5a4d04698b)
