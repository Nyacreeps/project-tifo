#ifndef OIL_FILTER_FILTER_CUH
#define OIL_FILTER_FILTER_CUH

#define INTENSITY 20
#define RADIUS 5

#include <spdlog/spdlog.h>
#include <cmath>

extern "C" unsigned char **oil_filter(unsigned char **buffer_, const int width, const int height);

#endif //OIL_FILTER_FILTER_CUH