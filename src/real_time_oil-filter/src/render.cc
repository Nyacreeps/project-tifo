#include "render.hh"
#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/videoio.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/imgproc.hpp>

int main(int argc, char **argv)
{
    (void) argc;
    (void) argv;

    std::string input_file;
    std::string output_file;
    std::string mode = "image";
    int scale = 1;
    bool debug = false;

    CLI::App app{"oil-filter"};
    app.add_option("-i,--input", input_file, "Input image/video")
        ->check(CLI::ExistingFile);
    app.add_option("-o,--output", output_file, "Output image/video");
    app.add_option("-s,--scale", scale, "Scale factor for webcam output");
    app.add_set("-m,--mode", mode, {"image", "video", "webcam"}, "Either 'image', 'video' or 'webcam'");
    app.add_flag("-d,--debug", debug, "Enable debug mode");
    CLI11_PARSE(app, argc, argv);
    if (debug)
        spdlog::set_level(spdlog::level::debug);

    spdlog::info("Running on {}", input_file);

    if (mode == "image")
    {
        if (output_file.empty())
            output_file = "output.png";
        int width, height = 0;
        auto image_ = read_png(input_file.c_str(), &width, &height);
        auto image = flatten(image_, width * 3, height);
        for (int i = 0; i < height; free(image_[i++]));
        free(image_);
        auto result_ = oil_filter(image, width, height);
        free(image);
        auto result = unflatten(result_, width * 3, height);
        free(result_);
        write_png(output_file.c_str(), result, width, height);
        for (int i = 0; i < height; free(result[i++]));
        free(result);
    }
    else if (mode == "webcam") {
        cv::Mat cur_frame;
        cv::namedWindow("Capture", cv::WINDOW_AUTOSIZE);
        cv::VideoCapture video_capture(0);
        while (true) {
            video_capture >> cur_frame;
            std::vector<uchar> array;
            if (cur_frame.isContinuous()) {
                array.assign(cur_frame.data, cur_frame.data + cur_frame.total() * cur_frame.channels());
            } else {
                for (int i = 0; i < cur_frame.rows; ++i) {
                    array.insert(array.end(), cur_frame.ptr<uchar>(i),
                                 cur_frame.ptr<uchar>(i) + cur_frame.cols * cur_frame.channels());
                }
            }
            auto res = oil_filter(array.data(), cur_frame.cols, cur_frame.rows);
            cv::Mat result(cur_frame.rows, cur_frame.cols, CV_8UC3, res);
            cv::resize(result, result, cv::Size(cur_frame.cols * scale, cur_frame.rows * scale), 0, 0, cv::INTER_LINEAR);
            cv::imshow("Capture", result);
            free(res);
            int key = cv::waitKey(10) & 0xFF;
            if (key == 27) {
                cv::destroyAllWindows();
                break;
            }
        }
    }
    if (mode == "video") {
        if (output_file.empty())
            output_file = "output.avi";
        spdlog::debug("Reading {}", input_file);
        cv::VideoCapture cap(input_file);

        if (!cap.isOpened()) {
            std::cout << "Error opening video stream" << std::endl;
            return -1;
        }

        int frame_width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
        int frame_height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

        cv::VideoWriter video(output_file, cv::VideoWriter::fourcc('M','J','P','G'), 30, cv::Size(frame_width,frame_height));
        cv::namedWindow("Capture", cv::WINDOW_AUTOSIZE);
        while(true) {
            cv::Mat cur_frame;

            cap >> cur_frame;
            if (cur_frame.empty())
                break;

            std::vector<uchar> array;
            if (cur_frame.isContinuous()) {
                array.assign(cur_frame.data, cur_frame.data + cur_frame.total() * cur_frame.channels());
            } else {
                for (int i = 0; i < cur_frame.rows; ++i) {
                    array.insert(array.end(), cur_frame.ptr<uchar>(i),
                                 cur_frame.ptr<uchar>(i) + cur_frame.cols * cur_frame.channels());
                }
            }
            auto res = oil_filter(array.data(), cur_frame.cols, cur_frame.rows);
            cv::Mat result(cur_frame.rows, cur_frame.cols, CV_8UC3, res);
            free(res);

            video.write(result);
            cv::imshow("Capture", result);
            auto key = cv::waitKey(25) & 0xFF;
            if(key == 27)
                break;
        }
        cap.release();
        video.release();
        cv::destroyAllWindows();
    }
}