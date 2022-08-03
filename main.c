#include <stdio.h>
#include <assert.h>
/* stb library */
#define STB_IMAGE_IMPLEMENTATION
#include "3rd/stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "3rd/stb/stb_image_write.h"
#include <omp.h>


void rgb_read(const char *path, float **out_data, int *out_h, int *out_w, int *out_c) {
    // load image, memory order is HWC (RGB)
    // see stb_image.h: 170
    int h, w, c;
    unsigned char *buffer = stbi_load(path, &w, &h, &c, 0);

    float *data = (float*)malloc(sizeof(float) * h * w * c);
    #pragma omp parallel for
    for (int i = 0; i < h * w * c; ++i)
        data[i] = (float)buffer[i];
    free(buffer);

    *out_data = data, *out_h = h, *out_w = w, *out_c = c;
}

void rgb_write(const char *path, float **data, int *h, int *w, int *c) {
    int n = (*h) * (*w) * (*c);
    unsigned char *buffer = (unsigned char*)malloc(n);

    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        buffer[i] = (unsigned char)((*data)[i]);
    }

    stbi_write_jpg(path, *w, *h, *c, buffer, 0);
    free(buffer);
}

int main() {
    puts("hello ccanny!");

    const char *in = "data/girl.jpg";
    
    float *data;
    int h, w, c;
    rgb_read(in, &data, &h, &w, &c);
    printf("%s, (%d, %d, %d)\n", in, h, w, c);

    const char *out = "data/girl_out.jpg";
    rgb_write(out, &data, &h, &w, &c);

    free(data);
    return 0;
}