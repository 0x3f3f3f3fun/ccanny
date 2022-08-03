#include <stdio.h>
#include <assert.h>
/* stb library */
#define STB_IMAGE_IMPLEMENTATION
#include "3rd/stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "3rd/stb/stb_image_write.h"
#include <omp.h>


const float SOBEL_KERNEL[2][3][3] = {
    {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}},
    {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}}
};

float* float_malloc(int n) {
    return (float*)malloc(sizeof(float) * n);
}

void rgb_read(char *path, float **out_data, int *out_h, int *out_w, int *out_c) {
    // load image, memory order is HWC (RGB)
    // see stb_image.h: 170
    int h, w, c;
    unsigned char *buffer = stbi_load(path, &w, &h, &c, 0);

    float *data = float_malloc(h * w * c);
    #pragma omp parallel for
    for (int i = 0; i < h * w * c; ++i)
        data[i] = (float)buffer[i];
    free(buffer);

    *out_data = data, *out_h = h, *out_w = w, *out_c = c;
}

void rgb_write(char *path, float *data, int h, int w, int c) {
    int n = h * w * c;
    unsigned char *buffer = (unsigned char*)malloc(n);

    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        buffer[i] = (unsigned char)data[i];
    }

    stbi_write_jpg(path, w, h, c, buffer, 0);
    free(buffer);
}

inline
float* at_hwc(float *data, int y, int w, int x, int c, int z) {
    return data + y * w * c + x * c + z;
}

inline
float* at_chw(float *data, int z, int h, int y, int w, int x) {
    return data + z * h * w + y * w + x;
}

void hwc2chw(float *in, float *out, int h, int w, int c) {
    #pragma omp parallel for
    for (int i = 0; i < h * w * c; i++) {
        int y = i / (w * c), x = (i % (w * c)) / c, z = i % c;
        *at_chw(out, z, h, y, w, x) = *at_hwc(in, y, w, x, c, z);
    }
}

void chw2hwc(float *in, float *out, int h, int w, int c) {
    #pragma omp parallel for
    for (int i = 0; i < h * w * c; i++) {
        int z = i / (h * w), y = (i % (h * w)) / w, x = i % w;
        *at_hwc(out, y, w, x, c, z) = *at_chw(in, z, h, y, w, x);
    }
}

inline
int check_image_range(int x, int y, int w, int h) {
    return (0 <= x && x < w && 0 <= y && y < h);
}

void rgb2gray(float *rgb, float *out, int h, int w) {
    // #pragma omp parallel for
    for (int i = 0; i < h * w; i++) {
        int y = i / w, x = i % w;
        float r = *at_hwc(rgb, y, w, x, 3, 0), g = *at_hwc(rgb, y, w, x, 3, 1),
              b = *at_hwc(rgb, y, w, x, 3, 2);
        // gray = 0.1140 * B  + 0.5870 * G + 0.2989 * R
        float gray = 0.1140 * b  + 0.5870 * g + 0.2989 * r;
        *at_hwc(out, y, w, x, 1, 0) = gray;
    }
}

void mean_blur(float *in, float *out, int h, int w, int c, int k) {
    /* Input memory order should be C-H-W if c != 1 */
    int half_k = k / 2;

    for (int z = 0; z < c; z++)
        #pragma omp parallel for
        for (int i = 0; i < h * w; i++) {
            int y = i / w, x = i % w;
            float s = 0, cnt = 0;
            // loop for kernel
            for (int dy = -half_k; dy < k - half_k; dy++) {
                for (int dx = -half_k; dx < k - half_k; dx++) {
                    if (!check_image_range(x + dx, y + dy, w, h))
                        continue;
                    s += *at_chw(in, z, h, y + dy, w, x + dx), cnt++;
                }
            }
            *at_chw(out, z, h, y, w, x) = s / cnt;
        }
}

void gradient_magnitude(float *gray, float *out, int h, int w) {
    /* Size of sobel is 3 */
    int k = 3; int half_k = k / 2;

    #pragma omp parallel for
    for (int i = 0; i < h * w; i++) {
        int y = i / w, x = i % w;
        float sx = 0, sy = 0, cnt = 0;
        // loop for kernel
        for (int dy = -half_k; dy < k - half_k; dy++) {
            for (int dx = -half_k; dx < k - half_k; dx++) {
                if (!check_image_range(x + dx, y + dy, w, h))
                    continue;
                sx += *at_hwc(gray, y + dy, w, x + dx, 1, 0) * SOBEL_KERNEL[0][1 + dy][1 + dx];
                sy += *at_hwc(gray, y + dy, w, x + dx, 1, 0) * SOBEL_KERNEL[1][1 + dy][1 + dx];
                cnt++;
            }
        }
        *at_hwc(out, y, w, x, 1, 0) = sqrtf(powf(sx / cnt, 2) + powf(sy / cnt, 2));
    }
}

void print_data(float *data, int n) {
    for (int i = 0; i < n; i++)
        if (i)
            printf(" %.3f", data[i]);
        else
            printf("%.3f", data[i]);
    puts("");
}

int main() {
    puts("hello ccanny!");
    float *data;
    int h, w, c;

    rgb_read("data/girl.jpg", &data, &h, &w, &c);
    printf("%s, (%d, %d, %d)\n", "read image data/girl.jpg", h, w, c);

    /* Convert to gray */
    float *gray = float_malloc(h * w);
    rgb2gray(data, gray, h, w);
    rgb_write("data/girl_gray.jpg", gray, h, w, 1);

    /* Blur input image */
    float *blurred = float_malloc(h * w);
    mean_blur(gray, blurred, h, w, 1, 5);
    rgb_write("data/girl_blurred.jpg", blurred, h, w, 1);

    /* Get gradient magnitude */
    float *mag = float_malloc(h * w * 1);
    gradient_magnitude(blurred, mag, h, w);
    rgb_write("data/girl_mag.jpg", mag, h, w, 1);

    /* Free data */
    free(data);
    free(gray);
    free(blurred);
    free(mag);
    return 0;
}