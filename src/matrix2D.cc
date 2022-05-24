#include "matrix2D.hh"

#include "lodepng.hh"

template<>
Matrix2D<color::RGB> Matrix2D<color::RGB>::loadFromPNG(const std::string& path) {
    std::vector<unsigned char> image;
    unsigned cols, rows;
    size_t error = lodepng::decode(image, cols, rows, path);
    if (error) {
        std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << '\n';
        throw std::exception();
    }
    auto result = Matrix2D<color::RGB>(rows, cols);
    for (int i = 0; i < result.rows_; i++) {
        for (int j = 0; j < result.cols_; j++) {
            int index = i * (4 * result.cols_) + (j * 4);
            result[i][j] = color::RGB(image[index], image[index + 1], image[index + 2]);
        }
    }
    return result;
}

template<>
Matrix2D<color::RGB> Matrix2D<color::RGB>::loadFromCVBuffer(unsigned char* buf, int rows, int cols) {
    auto result = Matrix2D<color::RGB>(rows, cols);
    for (int i = 0; i < result.rows_; i++) {
        for (int j = 0; j < result.cols_; j++) {
            int index = i * (3 * result.cols_) + (j * 3);
            result[i][j] = color::RGB(buf[index + 2], buf[index + 1], buf[index]);
        }
    }
    return result;
}

template<>
void Matrix2D<color::RGB>::saveAsPNG(const std::string& path) {
    std::vector<unsigned char> image;
    for (int i = 0; i < this->rows_; i++) {
        for (int j = 0; j < this->cols_; j++) {
            image.push_back((*this)[i][j].red);
            image.push_back((*this)[i][j].green);
            image.push_back((*this)[i][j].blue);
            image.push_back(255);
        }
    }
    size_t error = lodepng::encode(path, image, this->cols_, this->rows_);
    if (error) {
        std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << '\n';
        throw std::exception();
    }
}

template<>
void Matrix2D<color::RGB>::saveAsCVBuffer(unsigned char* buf) {
    for (int i = 0; i < this->rows_; i++) {
        for (int j = 0; j < this->cols_; j++) {
            int index = i * this->cols_ * 3 + j * 3;
            buf[index] = (*this)[i][j].blue;
            buf[index + 1] = (*this)[i][j].green;
            buf[index + 2] = (*this)[i][j].red;
        }
    }
}

template<>
Matrix2D<color::RGB> Matrix2D<color::RGB>::loadFromYUV420Frame(unsigned char* frame, int width, int height) {
    auto output = Matrix2D<color::RGB>(height, width);

    const int size = width * height;

    const size_t CbBase = size;

    const size_t CrBase = size + width*height/4;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int index = i * width + j;
            int Y  = frame[index] - 16;
            int Cr = frame[CrBase + (i * width / 4) + (j / 2)]  - 128;
            int Cb = frame[CbBase + (i * width / 4) + (j / 2)]  - 128;

            double R = 1.164*Y+1.596*Cr;
            double G = 1.164*Y-0.392*Cb-0.813*Cr;
            double B = 1.164*Y+2.017*Cb;

            output[i][j].red = (R > 255) ? 255 : ((R < 0) ? 0 : R);
            output[i][j].green = (G > 255) ? 255 : ((G < 0) ? 0 : G);
            output[i][j].blue = (B > 255) ? 255 : ((B < 0) ? 0 : B);
        }
    }
    return output;
}