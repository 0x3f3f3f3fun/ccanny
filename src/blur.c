#include "blur.h"

void mean_blur(image_t *in, image_t *out, int k) {
    int h = in->h, w = in->w, c = in->c;
    
    for (int z = 0; z < c; z++) {
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                float sum = 0;
                // accumulate kernel
                int l = k / 2, r = k - k / 2 - 1;
                for (int dy = -l; dy <= r; dy++) {
                    for (int dx = -l; dx <= r; dx++) {
                        int nx = (x + dx + w) % w,
                            ny = (y + dy + h) % h;
                        // tired ...
                        // it's not the time for coding ...
                    }
                }
            }
        }
    }
}