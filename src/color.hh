#pragma once

#include <algorithm>
#include <string>

struct Color {
    double red;
    double green;
    double blue;
    Color(double red, double green, double blue) :
        red(red), green(green), blue(blue) {};
    Color() :
        red(0), green(0), blue(0) {};

    void clamp();
    std::string to_string();
};