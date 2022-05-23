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
        auto image = read_png(input_file.c_str(), &width, &height);
        auto result = oil_filter(image, width, height);
        write_png(output_file.c_str(), result, width, height);
        free(image);
    }
    else if (mode == "webcam") {
        launch_webcam(input_file.c_str());
    }
}