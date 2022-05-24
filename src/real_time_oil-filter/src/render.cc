#include "render.hh"

#include "video.hh"

int main(int argc, char **argv)
{
    (void) argc;
    (void) argv;

    std::string input_file = "input.png";
    std::string output_file = "output.png";
    std::string mode = "image";
    bool debug = false;

    CLI::App app{"oil-filter"};
    app.add_option("-i,--input", input_file, "Input image/video")
        ->check(CLI::ExistingFile);
    app.add_option("-o,--output", output_file, "Output image/video");
    app.add_set("-m,--mode", mode, {"image", "video", "webcam"}, "Either 'image', 'video' or 'webcam'");
    app.add_flag("-d,--debug", debug, "Enable debug mode");
    CLI11_PARSE(app, argc, argv);
    if (debug)
        spdlog::set_level(spdlog::level::debug);

    spdlog::info("Running on {}", input_file);

    if (mode == "image")
    {
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
        launch_webcam(input_file.c_str());
    }
}