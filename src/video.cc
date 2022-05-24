#include <opencv2/opencv.hpp>

#include <string>
#include <iostream>
#include <stdlib.h>

#include "matrix2D.hh"
#include "image.hh"

using namespace cv;
int main(int argc, char** argv)
{
    int framerate = std::stoi(argv[3]);
    cv::VideoCapture cap(argv[1]);
    if (!cap.isOpened()) {
        std::cout << "Error opening stream.\n";
        return 1;
    }
    int frame_width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
	int frame_height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

    cv::VideoWriter video(argv[2], cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), framerate, cv::Size(frame_width, frame_height));
    int count = 0;
    while(true) {
        cv::Mat frame;
        cap >> frame;
        if (frame.empty())
            break;
        unsigned char *frame_data = frame.data;
        auto mat = Matrix2D<color::RGB>::loadFromCVBuffer(frame_data, frame_height, frame_width);
        mat = oilFilter(mat, 20, 3);
        mat.saveAsCVBuffer(frame.data);
        video.write(frame);
        count++;
    }
    cap.release();
    video.release();
    return 0;
}