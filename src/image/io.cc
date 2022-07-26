#include "image.hpp"
#include "log.h"

// stb library
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"
#include <omp.h>


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
            res.at<Vec1f>(i / w, i % w)[0] = (float_t)buffer[i];
        }
    } else { // c == 3
        #pragma omp parallel for
        for (size_t i = 0; i < h * w; i++) {
            res.at<Vec3f>(i / w, i % w)[0] = (float_t)buffer[i * 3 + 0];
            res.at<Vec3f>(i / w, i % w)[1] = (float_t)buffer[i * 3 + 1];
            res.at<Vec3f>(i / w, i % w)[2] = (float_t)buffer[i * 3 + 2];
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
