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