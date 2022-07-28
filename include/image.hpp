#ifndef CCANNY_INCLUDE_IAMGE_H
#define CCANNY_INCLUDE_IAMGE_H

#include "vector.hpp"
#include <string>

namespace ccanny {

typedef Vector<3> Vec3f;
typedef Vector<1> Vec1f;

class Image {
private:
    int height_;
    int width_;
    int channels_;
    float* data_;

public:
    Image() = default;
    Image(int h, int w, int c);
    Image(int h, int w, int c, float x);

    Image(const Image& other);
    Image(Image&& other);

    Image& operator=(const Image& other);
    Image& operator=(Image&& other);

    ~Image();

    void show() const;
    bool empty() const;
    float* const data() const { return data_; };
    int height() const { return height_; }
    int width() const { return width_; }
    int channels() const { return channels_; }
    int size() const { return height_ * width_ * channels_; }
    
    template<typename ReturnType>
    ReturnType& at(int h, int w) const;
};

template<typename ReturnType>
ReturnType& Image::at(int i, int j) const {
    return *((ReturnType*)(data_ + i * width_ * channels_ + j * channels_));
}

Image imread(const std::string& path);
bool imwrite(const Image& src, const std::string& path);

} // namespace ccanny

#endif // CCANNY_INCLUDE_IAMGE_H