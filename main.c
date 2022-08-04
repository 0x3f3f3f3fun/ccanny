#include <stdio.h>
#include <fcntl.h>
/* stb library */
#define STB_IMAGE_IMPLEMENTATION
#include "3rd/stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "3rd/stb/stb_image_write.h"
#include <omp.h>


float SOBEL_KERNEL[2][3][3] = {
    {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}},
    {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}}
};

float DIR[2] = {-1.f, 1.f};
int DX[4] = {0, 0, 1, -1}, DY[4] = {1, -1, 0, 0};

typedef struct {
    int x, y;
} coord_t;

/* Allocate n floats memory. */
float* float_malloc(int n) {
    return (float*)malloc(sizeof(float) * n);
}

void print_data(float *data, int n) {
    for (int i = 0; i < n; i++)
        if (i)
            printf(" %.3f", data[i]);
        else
            printf("%.3f", data[i]);
    puts("");
}

/* Read a R-G-B image from path. */
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

/* Write a R-G-B image to path. */
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

/* Locate a pixel on an image with memory layout H-W-C. */
inline float* at(float *data, int y, int w, int x, int c, int z) {
    return data + y * w * c + x * c + z;
}

/* Check if a coordinate (y, x) is inside a image with shape (h, w). */
inline int check_image_range(int x, int y, int w, int h) {
    return (0 <= x && x < w && 0 <= y && y < h);
}

/* Convert a R-G-B image to a gray image. */
void rgb2gray(float *in, float *out, int h, int w) {
    #pragma omp parallel for
    for (int i = 0; i < h * w; i++) {
        int y = i / w, x = i % w;
        float r = *at(in, y, w, x, 3, 0), g = *at(in, y, w, x, 3, 1),
              b = *at(in, y, w, x, 3, 2);
        // gray = 0.1140 * B  + 0.5870 * G + 0.2989 * R
        float gray = 0.1140 * b  + 0.5870 * g + 0.2989 * r;
        *at(out, y, w, x, 1, 0) = gray;
    }
}

/* Mean blur on gray imamge. */
void mean_blur(float *in, float *out, int h, int w, int k) {
    int half_k = k / 2;

    #pragma omp parallel for
    for (int i = 0; i < h * w; i++) {
        int y = i / w, x = i % w;
        float s = 0, cnt = 0;
        // loop for kernel
        for (int dy = -half_k; dy < k - half_k; dy++) {
            for (int dx = -half_k; dx < k - half_k; dx++) {
                if (!check_image_range(x + dx, y + dy, w, h))
                    continue;
                s += *at(in, y + dy, w, x + dx, 1, 0), cnt++;
            }
        }
        *at(out, y, w, x, 1, 0) = s / cnt;
    }
}

/* Commpute gradient of a gray image. */
void gradient(float *in, float *out_gx, float *out_gy, float *out_mag, int h, int w) {
    int k = 3; int half_k = k / 2;

    #pragma omp parallel for
    for (int i = 0; i < h * w; i++) {
        int y = i / w, x = i % w;
        float gx = 0, gy = 0;
        // loop for kernel
        for (int dy = -half_k; dy < k - half_k; dy++) {
            for (int dx = -half_k; dx < k - half_k; dx++) {
                if (!check_image_range(x + dx, y + dy, w, h))
                    continue;
                gx += *at(in, y + dy, w, x + dx, 1, 0) * SOBEL_KERNEL[0][1 + dy][1 + dx];
                gy += *at(in, y + dy, w, x + dx, 1, 0) * SOBEL_KERNEL[1][1 + dy][1 + dx];
            }
        }
        if (x == 0 || x == w - 1 || y == 0 || y == h - 1)
            gx = 0, gy = 0;
        *at(out_gx, y, w, x, 1, 0) = gx, *at(out_gy, y, w, x, 1, 0) = gy;
        *at(out_mag, y, w, x, 1, 0) = sqrtf(powf(gx, 2) + powf(gy, 2));
    }
}

/* TODO: none max supression is bad */
void none_max_supression(float *in_gx, float *in_gy, float *in_mag, float *out, int h, int w) {
    #pragma omp parallel for
    for (int i = 0; i < h * w; i++) {
        int y = i / w, x = i % w;
        // normalize gradient
        float gx = *at(in_gx, y, w, x, 1, 0), gy = *at(in_gy, y, w, x, 1, 0);
        float mag = *at(in_mag, y, w, x, 1, 0);
        gx /= mag, gy /= mag;

        // compared with neighbors (not precise)
        int is_max = 1;
        for (int t = 0; t < 2; t++) {
            int nx = x + DIR[t] * gx + 0.5, ny = y + DIR[t] * gy + 0.5;
            if (check_image_range(nx, ny, w, h) && mag < *at(in_mag, ny, w, nx, 1, 0))
                is_max = 0;
        }

        if (is_max)
            *at(out, y, w, x, 1, 0) = mag;
        else
            *at(out, y, w, x, 1, 0) = 0.f;
    }
}

float find_kth_min(float *data, int l, int r, int k) {
    if (l == r)
        return data[l];

    float x = data[(l + r) / 2];
    int i = l - 1, j = r + 1;
    while (i < j) {
        while (data[++i] < x);
        while (data[--j] > x);
        if (i < j) {
            float t = data[i];
            data[i] = data[j];
            data[j] = t;
        }
    }

    int sl = j - l + 1;
    if (sl >= k)
        return find_kth_min(data, l, j, k);
    else
        return find_kth_min(data, j + 1, r, k - sl);
}

void sort(float *data, int l, int r) {
    if (l >= r)
        return;

    float x = data[(l + r) / 2];
    int i = l - 1, j = r + 1;
    while (i < j) {
        while (data[++i] < x);
        while (data[--j] > x);
        if (i < j) {
            float t = data[i];
            data[i] = data[j];
            data[j] = t;
        }
    }

    sort(data, l, j); sort(data, j + 1, r);
}

void double_threshold(float *in, int h, int w, float tmin, float tmax) {
    // find boundary
    // tmin = find_kth_min(in, 0, h * w - 1, tmin * h * w);
    // tmax = find_kth_min(in, 0, h * w - 1, tmax * h * w);
    // float *tmp = float_malloc(h * w);
    // memcpy(tmp, in, sizeof(float) * h * w);
    // sort(tmp, 0, h * w - 1);

    // int nz = -1;
    // for (int i = 0; i < h * w; i++)
    //     if (tmp[i] > 0) {
    //         nz = i; break;
    //     }
    // tmin = tmp[nz + (int)((h * w - 1 - nz) * tmin)];
    // tmax = tmp[nz + (int)((h * w - 1 - nz) * tmax)];
    // free(tmp);

    float max_mag = -1.f;
    for (int i = 0; i < h * w; i++)
        max_mag = max_mag < in[i] ? in[i] : max_mag;
    #pragma omp parallel for
    for (int i = 0; i < h * w; i++)
        in[i] /= max_mag;

    // first stage, clear noise (mag < tmin) and enhance weak edges
    coord_t *stk = (coord_t*)malloc(sizeof(coord_t) * h * w);
    char *checked = (char*)malloc(h * w);
    memset(checked, 0, h * w);
    int top = -1;

    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++) {
            float mag = *at(in, y, w, x, 1, 0);
            if (mag <= tmax || checked[y * w + x])
                continue;
            
            top++; stk[top].x = x, stk[top].y = y;
            checked[y * w + x] = 1;

            while (top >= 0) {
                // fetch top
                coord_t u = stk[top]; top--;
                // search for neighbors
                for (int i = 0; i < 4; i++) {
                    int nx = u.x + DX[i], ny = u.y + DY[i];
                    // if this neighbor is weak edge
                    if (check_image_range(nx, ny, w, h)
                        && !checked[ny * w + nx] && *at(in, ny, w, nx, 1, 0) >= tmin) {
                        *at(in, ny, w, nx, 1, 0) = *at(in, u.y, w, u.x, 1, 0);
                        // push to stack
                        top++; stk[top].x = nx, stk[top].y = ny;
                        checked[ny * w + nx] = 1;
                    }
                }
            }
        }
    free(stk); free(checked);
    
    // second stage
    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++) {
            float mag = *at(in, y, w, x, 1, 0);
            if (mag > tmax)
                *at(in, y, w, x, 1, 0) = 255.f;
            else
                *at(in, y, w, x, 1, 0) = 0.f;
        }
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
    mean_blur(gray, blurred, h, w, 7);
    rgb_write("data/girl_blurred.jpg", blurred, h, w, 1);

    /* Get gradient magnitude */
    float *gx = float_malloc(h * w), *gy = float_malloc(h * w), *mag = float_malloc(h * w);
    gradient(blurred, gx, gy, mag, h, w);
    rgb_write("data/girl_mag.jpg", mag, h, w, 1);

    /* None max supression in magnitude */
    float *nms_res = float_malloc(h * w);
    none_max_supression(gx, gy, mag, nms_res, h, w);
    rgb_write("data/girl_nmsed.jpg", nms_res, h, w, 1);

    /* Double threshold */
    double_threshold(nms_res, h, w, 0.1, 0.2);
    rgb_write("data/girl_result.jpg", nms_res, h, w, 1);

    /* Free data */
    free(data);
    free(gray);
    free(blurred);
    free(mag);
    free(gx);
    free(gy);
    free(nms_res);
    return 0;
}