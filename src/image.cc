#include "image.hh"

#include <algorithm>
#include <iostream>

#include "morpho.hh"

Matrix2D<int> computeIntensities(const Matrix2D<color::RGB>& image, float intensityLevels) {
    auto result = Matrix2D<int>(image.rows_, image.cols_);
    for (int i = 0; i < image.rows_; i++) {
        for (int j = 0; j < image.cols_; j++) {
            auto color = image[i][j];
            int intensity = (int)((float)(color.red + color.green + color.blue) / 3 * intensityLevels / 255);
            std::clamp(intensity, 0, 255);
            result[i][j] = intensity;
        }
    }
    return result;
}

Matrix2D<color::RGB> oilFilter(const Matrix2D<color::RGB>& image, float intensityLevels, int radius) {
    auto mask = morphoCircle(radius);
    auto result = Matrix2D<color::RGB>(image.rows_, image.cols_);
    auto intensityCache = computeIntensities(image, intensityLevels);
    int progress = 0;
    for (int i = 0; i < image.rows_; i++) {
        for (int j = 0; j < image.cols_; j++) {
            if ((progress % (int)(image.rows_ * image.cols_ / 20)) == 0) {
                double percentage = (double)progress / (double)(image.rows_ * image.cols_);
                std::cout << "[";
                int pos = (int)(70. * percentage);
                for (int i = 0; i < 70; ++i) {
                    if (i < pos) std::cout << "=";
                    else if (i == pos) std::cout << ">";
                    else std::cout << " ";
                }
                std::cout << "] " << int(percentage * 100.0) + 1 << " %\r";
                std::cout.flush();
                progress++;
            }
            int intensityBins[256] = {0};
            int averageR[256] = {0};
            int averageG[256] = {0};
            int averageB[256] = {0};
            for (int n = 0; n < 2 * radius + 1; n++) {
                for (int m = 0; m < 2 * radius + 1; m++) {
                    if (!mask[n][m])
                        continue;
                    auto indexh = i + (n - radius);
                    auto indexw = j + (m - radius);
                    if (indexh < 0 or indexh >= image.rows_ or indexw < 0 or indexw >= image.cols_)
                        continue;
                    auto intensity  = intensityCache[indexh][indexw];
                    intensityBins[intensity]++;
                    auto color = image[indexh][indexw];
                    averageR[intensity] += color.red;
                    averageG[intensity] += color.green;
                    averageB[intensity] += color.blue;
                }
            }
            int maxIndex = 0;
            int curMax = 0;
            for (int k = 0; k < 256; k++) {
                if (intensityBins[k] > curMax) {
                    curMax = intensityBins[k];
                    maxIndex = k;
                }
            }
            color::RGB& resultColor = result[i][j];
            resultColor.red = (int)((float)averageR[maxIndex] / (float)intensityBins[maxIndex]);
            resultColor.green = (int)((float)averageG[maxIndex] / (float)intensityBins[maxIndex]);
            resultColor.blue = (int)((float)averageB[maxIndex] / (float)intensityBins[maxIndex]);
            progress++;
        }
    }
    std::cout << '\n';
    return result;
}