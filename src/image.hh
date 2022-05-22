#pragma once

#include "matrix2D.hh"

Matrix2D<int> computeIntensities(const Matrix2D<color::RGB>& image, float intensityLevels);

Matrix2D<color::RGB> oilFilter(const Matrix2D<color::RGB>& image, float intensityLevels, int radius);