#include <stdio.h>
#include <getopt.h>
#include <sys/time.h>
#include <omp.h>

/* stb library */
#define STB_IMAGE_IMPLEMENTATION
#include "3rd/stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "3rd/stb/stb_image_write.h"

#define tic(msg) \
do \
{ \
    gettimeofday(&end, NULL); \
    long cost = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec; \
    printf("[%s] cost %.3f ms\n", msg, cost / 1000.f); \
    gettimeofday(&start, NULL); \
} \
while (0);

/* Below are some global variables */
float SOBEL_KERNEL_3x3[2][3][3] = {
    {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}},
    {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}}
};

float SOBEL_KERNEL_5x5[2][5][5] = {
    {
        {-1, -2, 0, 2, 1},
        {-4, -8, 0, 8, 4},
        {-6, -12, 0, 12, 6},
        {-4, -8, 0, 8, 4},
        {-1, -2, 0, 2, 1}
    },
    {
        {-1, -4, -6, -4, -1},
        {-2, -8, -12, -8, -2},
        {0, 0, 0, 0, 0},
        {2, 8, 12, 8, 2},
        {1, 4, 6, 4, 1}
    }
};

typedef struct {
    int x, y;
} coord_t;

/* Simple options parser for this program */
typedef struct {
    char path[100];         /* Path to input image */
    int k_blur;             /* Size of blur kernel */
    int k_sobel;            /* Size of sobel kernel */
    int tmin;               /* Low threshold */
    int tmax;               /* High threshold */
    int write_internal_res; /* Decide whether to save internal results */
} options_t;

options_t parse_options(int argc, char *argv[]) {
    int opt;
    options_t res = {{'\0'}, 3, 5, 20, 40, 0}; /* Default values */

    while ((opt = getopt(argc, argv, "f:b:s:l:h:i")) != -1) {
        switch (opt) {
            case 'f': { strcpy(res.path, optarg); break; }
            case 'b': { res.k_blur = atoi(optarg); break; }
            case 's': { res.k_sobel = atoi(optarg); break; }
            case 'l': { res.tmin = atoi(optarg); break; }
            case 'h': { res.tmax = atoi(optarg); break; }
            case 'i': { res.write_internal_res = 1; break; }
            default: {
                fprintf(stderr, "Usage: %s [-b] [-s] [-l] [-h] -i \n", argv[0]);
                exit(EXIT_FAILURE);
            }
        }
    }

    return res;
}

void print_options(options_t opt) {
    printf("\noptions: \n");
    printf("---> input                 = %s\n", opt.path);
    printf("---> blur kernel size      = %d\n", opt.k_blur);
    printf("---> sobel kernel size     = %d\n", opt.k_sobel);
    printf("---> min threshold         = %d\n", opt.tmin);
    printf("---> max threshold         = %d\n", opt.tmax);
    printf("---> write internal result = %d\n", opt.write_internal_res);
    printf("\n");
}

/* Allocate n floats memory. */
float* float_malloc(int n) {
    return (float*)malloc(sizeof(float) * n);
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
        if (data[i] < 0.f)
            buffer[i] = 0;
        else if (data[i] > 255.f)
            buffer[i] = 255.f;
        else
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
void gradient(float *in, float *out_gx, float *out_gy, float *out_mag, int h, int w, int k) {
    int half_k = k / 2;
    float *sobel_kernel = k == 3
                          ? (float*)SOBEL_KERNEL_3x3
                          : (float*)SOBEL_KERNEL_5x5;

    #pragma omp parallel for
    for (int i = 0; i < h * w; i++) {
        int y = i / w, x = i % w;
        float gx = 0, gy = 0;

        // loop for kernel
        for (int dy = -half_k; dy < k - half_k; dy++) {
            for (int dx = -half_k; dx < k - half_k; dx++) {
                if (!check_image_range(x + dx, y + dy, w, h))
                    continue;
                gx += *at(in, y + dy, w, x + dx, 1, 0) * sobel_kernel[(half_k + dy) * k + half_k + dx];
                gy += *at(in, y + dy, w, x + dx, 1, 0) * sobel_kernel[k * k + (half_k + dy) * k + half_k + dx];
            }
        }

        if (x < half_k || x >= w - half_k || y < half_k || y >= h - half_k)
            gx = 0, gy = 0;
        *at(out_gx, y, w, x, 1, 0) = gx, *at(out_gy, y, w, x, 1, 0) = gy;
        *at(out_mag, y, w, x, 1, 0) = sqrtf(powf(gx, 2) + powf(gy, 2));
    }
}

void none_max_supression(float *in_gx, float *in_gy, float *in_mag, float *out, int h, int w) {
    float step[2] = {-1, 1};

    #pragma omp parallel for
    for (int i = 0; i < h * w; i++) {
        int y = i / w, x = i % w;
        float gx = *at(in_gx, y, w, x, 1, 0), gy = *at(in_gy, y, w, x, 1, 0);
        float mag = *at(in_mag, y, w, x, 1, 0);

        if (mag == 0) {
            *at(out, y, w, x, 1, 0) = 0.f;
            continue;
        }

        // normalize gradient
        gx /= mag, gy /= mag;

        // compared with neighbors along gradient (not precise)
        int is_max = 1;
        for (int t = 0; t < 2; t++) {
            int nx = x + step[t] * gx + 0.5, ny = y + step[t] * gy + 0.5;
            if (check_image_range(nx, ny, w, h) && mag < *at(in_mag, ny, w, nx, 1, 0))
                is_max = 0;
        }

        if (is_max)
            *at(out, y, w, x, 1, 0) = mag;
        else
            *at(out, y, w, x, 1, 0) = 0.f;
    }
}

void double_threshold(float *in, int h, int w, float tmin, float tmax) {
    int dx[8] = {1, 1, 1, 0, 0, -1, -1, -1},
        dy[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

    // normalize gradient magnitude to [0, 255]
    float max_mag = -1.f;
    for (int i = 0; i < h * w; i++)
        max_mag = max_mag < in[i] ? in[i] : max_mag;
    
    #pragma omp parallel for
    for (int i = 0; i < h * w; i++)
        in[i] = in[i] / max_mag * 255.f;

    // enhance weak edges
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
                for (int i = 0; i < 8; i++) {
                    int nx = u.x + dx[i], ny = u.y + dy[i];
                    // if this neighbor is weak edge
                    if (check_image_range(nx, ny, w, h) && !checked[ny * w + nx]
                        && *at(in, ny, w, nx, 1, 0) >= tmin)
                    {
                        *at(in, ny, w, nx, 1, 0) = *at(in, u.y, w, u.x, 1, 0);
                        // push to stack
                        top++; stk[top].x = nx, stk[top].y = ny;
                        checked[ny * w + nx] = 1;
                    }
                }
            }
        }
    free(stk); free(checked);
    
    // binarize
    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++) {
            float mag = *at(in, y, w, x, 1, 0);
            if (mag > tmax)
                *at(in, y, w, x, 1, 0) = 255.f;
            else
                *at(in, y, w, x, 1, 0) = 0.f;
        }
}

int main(int argc, char *argv[]) {
    printf("hello ccanny!\n");
    options_t opt = parse_options(argc, argv);
    print_options(opt);

    float *data;
    int h, w, c;
    rgb_read(opt.path, &data, &h, &w, &c);
    printf("read image %s, (%d, %d, %d)\n\n", opt.path, h, w, c);

    struct timeval start, end;
    gettimeofday(&start, NULL);

    float *gray = float_malloc(h * w), *blurred = float_malloc(h * w),
          *gx = float_malloc(h * w),   *gy = float_malloc(h * w),
          *mag = float_malloc(h * w),  *nms_res = float_malloc(h * w);
    tic("allocate memory")

    /* Convert to gray */
    rgb2gray(data, gray, h, w);
    /* Blur input image */
    mean_blur(gray, blurred, h, w, opt.k_blur);
    /* Get gradient magnitude */
    gradient(blurred, gx, gy, mag, h, w, opt.k_sobel);
    /* None max supression in magnitude */
    none_max_supression(gx, gy, mag, nms_res, h, w);
    tic("preprocess")

    if (opt.write_internal_res) {
        rgb_write("internal_results/gray.jpg", gray, h, w, 1);
        rgb_write("internal_results/blurred.jpg", blurred, h, w, 1);
        rgb_write("internal_results/mag.jpg", mag, h, w, 1);
        rgb_write("internal_results/gx.jpg", gx, h, w, 1);
        rgb_write("internal_results/gy.jpg", gy, h, w, 1);
        rgb_write("internal_results/nmsed.jpg", nms_res, h, w, 1);
    }
    tic("write internal result")

    /* Double threshold */
    double_threshold(nms_res, h, w, opt.tmin, opt.tmax);
    tic("binarize")

    rgb_write("result.jpg", nms_res, h, w, 1);

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