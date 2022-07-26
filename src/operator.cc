#include "operator.h"


ccanny::Image ccanny::mean_blur(const Image& src, const size_t k) {
    size_t h = src.height(), w = src.width(), c = src.channels();
    Image res(h, w, c);

    if (c == 3) {
        for (size_t y = 0; y < h; y++) {
            for (size_t x = 0; x < w; x++) {
                Vec3f s(0.0f);
                // TODO:
            }
        }
    } else {

    }
}
