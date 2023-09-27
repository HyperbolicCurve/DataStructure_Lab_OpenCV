#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

//Add these parameters to linker(compiler) commandline
//-lopencv_core452 -lopencv_imgcodecs452 -lopencv_imgproc452 -lopencv_calib3d452 -lopencv_dnn452 -lopencv_features2d452 -lopencv_flann452 -lopencv_gapi452 -lopencv_highgui452 -lopencv_ml452 -lopencv_objdetect452 -lopencv_photo452 -lopencv_stitching452 -lopencv_video452 -lopencv_videoio452

int main()
{
	Mat src=imread("1.png");
    if(src.empty())
		return -1;
    Mat blurDst;
    GaussianBlur(src,blurDst,Size(9,9),11,11);
    imshow("GaussianDst",blurDst);
    waitKey(-1);
    return 0;
}
