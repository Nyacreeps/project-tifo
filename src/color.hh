#pragma once

#include <string>

namespace color {
    struct RGB {
        size_t red   : 8;
        size_t green : 8;
        size_t blue  : 8;
        RGB(size_t red, size_t green, size_t blue) :
            red(red), green(green), blue(blue) {};
        RGB() :
            red(0), green(0), blue(0) {};
        std::string to_string() {
            return std::to_string(red) + ", " + std::to_string(green) + ", " + std::to_string(blue);
        }
    };
}