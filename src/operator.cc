#include "operator.h"
#include "check.h"
#include <omp.h>


ccanny::Image ccanny::mean_blur(const Image& src, const int k) {
    int h = src.height(), w = src.width(), c = src.channels();
    Image res(h, w, c);

    if (c == 3) {
        #pragma omp parallel for
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                Vec3f s(0.0f);
                int cnt = 0;
                for (int dy = -k / 2; dy < k - k / 2; dy++)
                    for (int dx = -k / 2; dx < k - k / 2; dx++) {
                        if (!check_image_range(x + dx, y + dy, w, h))
                            continue;
                        s += src.at<Vec3f>(y + dy, x + dx), cnt++;
                    }
                res.at<Vec3f>(y, x) = s / cnt;
            }
        }
    } else {
        #pragma omp parallel for
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                float s = 0;
                int cnt = 0;
                for (int dy = -k / 2; dy < k - k / 2; dy++)
                    for (int dx = -k / 2; dx < k - k / 2; dx++) {
                        if (!check_image_range(x + dx, y + dy, w, h))
                            continue;
                        s += src.at<Vec1f>(y + dy, x + dx)[0], cnt++;
                    }
                res.at<Vec1f>(y, x)[0] = s / cnt;
            }
        }
    }
    return res;
}
