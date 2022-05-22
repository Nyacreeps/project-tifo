#include <iostream>
#include <ctime>

#include "image.hh"

int main(int argc, char** argv) {
    clock_t tStart = clock();
    auto image = Matrix2D<color::RGB>::loadFromPNG("../resources/forest.png");
    auto filtered = oilFilter(image, 20, 5);
    filtered.saveAsPNG("../resources/test.png");
    printf("Time taken (C++): %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
}