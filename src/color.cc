#include "color.hh"

std::string Color::to_string() {
    return std::to_string(red) + ", " + std::to_string(green) + ", " + std::to_string(blue);
}

void Color::clamp() {
    std::clamp(red, 0.0, 1.0);
    std::clamp(green, 0.0, 1.0);
    std::clamp(blue, 0.0, 1.0);
}