#include <opencv2/opencv.hpp>

#include <string>
#include <iostream>

using namespace cv;
int main(int argc, char** argv)
{
    std::cout << cv::getBuildInformation() << std::endl;
    cv::VideoCapture cap(argv[1]);
    if (!cap.isOpened()) {
        std::cout << "Error opening stream.\n";
        return 1;
    }
    int count = 0;
    while(true) {
        cv::Mat frame;
        cap >> frame;
        if (frame.empty())
            break;
        count++;
    }
    cap.release();
    std::cout << "nbFrames " << std::to_string(count) << '\n';
    return 0;
}