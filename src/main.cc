#include <iostream>
#include <ctime>
#include <string>

#include "image.hh"

int main(int argc, char** argv) {
    std::string inputFile = "";
    std::string outputFile = "";
    bool videoMode = false;
    bool webcamMode = false;
    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "--input") {
            if (i + 1 >= argc) {
                std::cout << "End of args after --input\n";
                return 1;
            }
            inputFile = std::string(argv[i + 1]);
        }
        if (std::string(argv[i]) == "--output") {
            if (i + 1 >= argc) {
                std::cout << "End of args after --output\n";
                return 1;
            }
            outputFile = std::string(argv[i + 1]);
        }
        if (std::string(argv[i]) == "--video") {
            videoMode = true;
        }
        if (std::string(argv[i]) == "--webcam") {
            webcamMode = true;
        }
    }
    if (!videoMode && !webcamMode) {
        if (inputFile == "") {
            std::cout << "Missing input file\n";
            return 1;
        }
        if (outputFile == "") {
            std::cout << "Missing output file\n";
            return 1;
        }
        auto image = Matrix2D<color::RGB>::loadFromPNG(inputFile);
        clock_t tStart = clock();
        auto filtered = oilFilter(image, 20, 5);
        printf("Time taken : %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
        filtered.saveAsPNG(outputFile);
    }
}