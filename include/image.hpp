#ifndef CCANNY_INCLUDE_IAMGE_H
#define CCANNY_INCLUDE_IAMGE_H

#include "vector.hpp"
#include <string>

namespace ccanny {

typedef Vector<3> Pixel;
typedef float_t GrayPixel;

class Image {
private:
    float_t* data_;
    size_t height_, width_, channels_;

public:
    Image() = default;
    Image(size_t h, size_t w, size_t c);
    Image(size_t h, size_t w, size_t c, float_t x);

    Image(const Image& other);
    Image(Image&& other);

    Image& operator=(const Image& other);
    Image& operator=(Image&& other);

    ~Image();

    void show() const;
    bool empty() const;
    float_t* const data() const { return data_; };
    size_t height() const { return height_; }
    size_t width() const { return width_; }
    size_t channels() const { return channels_; }
    size_t size() const { return height_ * width_ * channels_; }
    
    template<typename ReturnType>
    ReturnType& at(size_t h, size_t w) const;
};

template<typename ReturnType>
ReturnType& Image::at(size_t i, size_t j) const {
    return *((ReturnType*)(data_ + i * width_ * channels_ + j * channels_));
}

Image imread(const std::string& path);
bool imwrite(const Image& src, const std::string& path);

} // namespace ccanny

#endif // CCANNY_INCLUDE_IAMGE_H