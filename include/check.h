#ifndef CCANNY_INCLUDE_CHECK_H
#define CCANNY_INCLUDE_CHECK_H

namespace ccanny {

inline bool check_image_range(int x, int y, int w, int h) {
    return (0 < x && x < w && 0 < y && y < h);
}

} // namespace ccanny

#endif // CCANNY_INCLUDE_CHECK_H