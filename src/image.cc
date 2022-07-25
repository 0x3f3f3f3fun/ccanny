#include "image.hpp"
#include "log.h"
// stb library
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"
#include <omp.h>
#include <iostream>

ccanny::Image::Image(size_t h, size_t w, size_t c)
: height_(h), width_(w), channels_(c) {
    size_t n = h * w * c;
    if (n) {
        data_ = new float_t[h * w * c];
    } else {
        data_ = nullptr;
    }
}

ccanny::Image::Image(size_t h, size_t w, size_t c, float_t x)
: height_(h), width_(w), channels_(c) {
    size_t n = h * w * c;
    if (n) {
        data_ = new float_t[h * w * c];
        #pragma omp parallel for
        for (size_t i = 0; i < h * w * c; i++)
            data_[i] = x;
    } else {
        data_ = nullptr;
    }
}

ccanny::Image::Image(const Image& other)
: height_(other.height_), width_(other.width_)
, channels_(other.channels_) {
    size_t n = other.height_ * other.width_ * other.channels_;
    data_ = new float_t[n];
    memcpy(data_, other.data_, sizeof(float_t) * n);
    // #pragma omp parallel for
    // for (size_t i = 0; i < n; i++)
    //     data_[i] = other.data_[i];
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
    size_t n = other.height_ * other.width_ * other.channels_;
    memcpy(data_, other.data_, sizeof(float_t) * n);
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
    printf("data: ");
    size_t size = height_ * width_ * channels_;
    if (size <= 0) {
        std::cout << "empty image";
    } else if (size <= 8) {
        printf("%.3f", data_[0]);
        for (size_t i = 1; i < size; i++) {
            printf(" %f", data_[i]);
        }
    } else {
        for (size_t i = 0; i < 4; i++) {
            printf("%.3f ", data_[i]);
        }
        printf("...");
        for (size_t i = size - 4; i < size; i++) {
            printf(" %.3f", data_[i]);
        }
    }
    printf("\n");

    for (size_t i = 0; i < 20; i++) {
        printf("- ");
    }
    printf("-\n");
}

ccanny::Image ccanny::imread(const std::string& path) {
    // load image, memory order is HWC (RGB)
    // see stb_image.h: 170
    int h, w, c;
    uchar_t* buffer = stbi_load(path.c_str(), &w, &h, &c, 0);

    if (!buffer) {
        ERROR("file not exist <%s>", path.c_str());
        return Image();
    }
    // c == 1: GRAY c == 3: RGB
    if (!(c == 1 || c == 3)) {
        ERROR("unsupported image type");
        return Image();
    }

    Image res(h, w, c);
    if (c == 1) {
        #pragma omp parallel for
        for (size_t i = 0; i < h * w; i++) {
            res.at<GrayPixel>(i / w, i % w) = (float_t)buffer[i];
        }
    } else { // c == 3
        #pragma omp parallel for
        for (size_t i = 0; i < h * w; i++) {
            res.at<Pixel>(i / w, i % w)[0] = (float_t)buffer[i * 3 + 0];
            res.at<Pixel>(i / w, i % w)[1] = (float_t)buffer[i * 3 + 1];
            res.at<Pixel>(i / w, i % w)[2] = (float_t)buffer[i * 3 + 2];
        }
    }
    free(buffer);

    return res;
}

bool ccanny::imwrite(const Image& src, const std::string& path) {
    size_t h = src.height(), w = src.width(), c = src.channels();
    size_t n = h * w * c;
    float_t* data = src.data();

    uchar_t* buffer = new uchar_t[n];
    #pragma omp parallel for
    for (size_t i = 0; i < n; i++) {
        buffer[i] = (uchar_t)data[i];
    }

    int success = stbi_write_jpg(path.c_str(), w, h, c, buffer, 0);
    free(buffer);

    if (!success) {
        ERROR("failed to write image to <%s>", path.c_str());
        return false;
    } else {
        return true;
    }
}