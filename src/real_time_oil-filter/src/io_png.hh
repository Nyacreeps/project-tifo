#pragma once
#include <spdlog/spdlog.h>
#include <memory>
#include <png.h>

png_bytep *read_png(const char* filename, int *width, int *height);
void write_png(const char *filename, png_bytep *row_pointers, int width, int height);
