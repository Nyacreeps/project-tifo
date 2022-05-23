#pragma once

#include <vector>
#include <iostream>
#include <string>

#include "color.hh"

template<typename T>
class Matrix2D {
public:
    class Row2D {
    public:
        friend class Matrix2D<T>;
        T& operator[](int col) {
            return parent.data_[row * parent.cols_ + col];
        }
    private:
        Row2D(Matrix2D<T>& parent, int row):
            parent(parent), row(row) {};
        Matrix2D& parent;
        int row;
    };

    class Row2DConst {
    public:
        friend class Matrix2D<T>;
        T operator[](int col) {
            return parent.data_[row * parent.cols_ + col];
        }
    private:
        Row2DConst(const Matrix2D<T>& parent, int row):
            parent(parent), row(row) {};
        const Matrix2D& parent;
        int row;
    };

    Row2D operator[](int row) {
        return Row2D(*this, row);
    }

    Row2DConst operator[](int row) const {
        return Row2DConst(*this, row);
    }
    
    Matrix2D(int rows, int cols) :
        rows_(rows), cols_(cols), data_(std::vector<T>(rows * cols)) {};

    static Matrix2D<T> loadFromPNG(const std::string& path);
    void saveAsPNG(const std::string& path);
    static Matrix2D<T> loadFromYUV420Frame(char* frame, int width, int height, int bytes);

    int rows_, cols_;
private:
    std::vector<T> data_;
};