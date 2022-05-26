#ifndef OIL_FILTER_FILTER_CUH
#define OIL_FILTER_FILTER_CUH

#define INTENSITY 20
#define RADIUS 4

#include <spdlog/spdlog.h>
#include <cmath>

extern "C" unsigned char *oil_filter(unsigned char *buffer, const int width, const int height);

extern "C" unsigned char *flatten(unsigned char **buffer_, const int width, const int height);

extern "C" unsigned char **unflatten(unsigned char *buffer_, const int width, const int height);

#endif //OIL_FILTER_FILTER_CUH
