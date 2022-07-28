#include "image.hpp"
#include <omp.h>
#include <iostream>


ccanny::Image::Image(int h, int w, int c)
: height_(h), width_(w), channels_(c) {
    int n = h * w * c;
    if (n) {
        data_ = new float[h * w * c];
    } else {
        data_ = nullptr;
    }
}

ccanny::Image::Image(int h, int w, int c, float x)
: height_(h), width_(w), channels_(c) {
    int n = h * w * c;
    if (n) {
        data_ = new float[h * w * c];
        #pragma omp parallel for
        for (int i = 0; i < h * w * c; i++)
            data_[i] = x;
    } else {
        data_ = nullptr;
    }
}

ccanny::Image::Image(const Image& other)
: height_(other.height_), width_(other.width_)
, channels_(other.channels_) {
    int n = other.height_ * other.width_ * other.channels_;
    data_ = new float[n];
    memcpy(data_, other.data_, sizeof(float) * n);
}

ccanny::Image::Image(Image&& other)
: height_(other.height_), width_(other.width_)
, channels_(other.channels_), data_(other.data_) {
    other.data_ = nullptr;
}

ccanny::Image::~Image() {
    if (data_) {
        delete[] data_;
    }
}

ccanny::Image& ccanny::Image::operator=(const Image& other) {
    // std::cout << "copy assign operator invoked" << std::endl;
    // TODO: check size
    // TODO: resize data_ ?
    int n = other.height_ * other.width_ * other.channels_;
    memcpy(data_, other.data_, sizeof(float) * n);
    return *this;
}

ccanny::Image& ccanny::Image::operator=(Image&& other) {
    // TODO: RVO?
    // std::cout << "move assign operator invoked" << std::endl;
    // TODO: check size
    data_ = other.data_;
    other.data_ = nullptr;
    return *this;
}

void ccanny::Image::show() const {
    for (int i = 0; i < 20; i++) {
        std::cout << "- ";
    }
    std::cout << "-" << std::endl;

    std::cout << "height: " << height_ << ", "
              << "width: " << width_ << ", "
              << "channels: " << channels_ << std::endl;
    
    std::cout << "data: ";
    int size = height_ * width_ * channels_;
    if (size <= 0) {
        std::cout << "empty image";
    } else if (size <= 8) {
        printf("%.3f", data_[0]);
        for (int i = 1; i < size; i++) {
            printf(" %f", data_[i]);
        }
    } else {
        for (int i = 0; i < 4; i++) {
            printf("%.3f ", data_[i]);
        }
        printf("...");
        for (int i = size - 4; i < size; i++) {
            printf(" %.3f", data_[i]);
        }
    }
    printf("\n");

    for (int i = 0; i < 20; i++) {
        printf("- ");
    }
    printf("-\n");
}

bool ccanny::Image::empty() const {
    return data_ == nullptr;
}