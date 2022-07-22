#include "image.h"
#include "log.h"
// stb library
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"
#include <memory.h>

const image_type_t channel_to_type[] = {0x3f3f3f3f, GRAY, GRAYA, RGB, RGBA};

int load_image(const char *path, image_t *img, image_type_t img_type) {
    int h, w, c;
    // load image, memory order is HWC (RGB)
    // see stb_image.h: 170
    unsigned char *buffer = stbi_load(path, &w, &h, &c, 0);
    if (!buffer) {
        ERROR("file not exist <%s>", path);
        return 0;
    }
    // validate image type
    image_type_t type = channel_to_type[c];
    if (type != img_type) {
        ERROR("error image type");
        free(buffer);
        return 0;
    }
    // allocate memory
    float *data = (float*)malloc(sizeof(float) * h * w * c);
    // copy each pixel and convert from unsigned char to float
    for (int i = 0; i < h * w * c; i++) {
        data[i] = (float)buffer[i];
    }
    free(buffer);
    img->h = h; img->w = w; img->c = c;
    img->data = data;
    return 1;
}

int write_image(const image_t *img, const char *path) {
    int h = img->h, w = img->w, c = img->c;
    float *data = img->data;

    unsigned char *buffer = (unsigned char*)malloc(sizeof(unsigned char) * h * w * c);
    for (int i = 0; i < h * w * c; i++) {
        buffer[i] = (unsigned char)data[i];
    }
    int success = stbi_write_jpg(path, w, h, c, buffer, 0);
    free(buffer);

    if (!success) {
        ERROR("failed to write image to %s", path);
        return 0;
    } else {
        return 1;
    }
}

void destroy_image(image_t *img) {
    free(img->data);
}

void print_image(const image_t *img) {
    for (int i = 0; i < 20; i++) {
        printf("- ");
    }
    printf("-\n");

    printf("height: %d, width: %d, channel: %d\n", img->h, img->w, img->c);
    printf("data: ");
    int size = img->h * img->w * img->c;
    if (size <= 0) {
        printf("empty image");
    } else if (size <= 8) {
        printf("%.3f", img->data[0]);
        for (int i = 1; i < size; i++) {
            printf(" %f", img->data[i]);
        }
    } else {
        for (int i = 0; i < 4; i++) {
            printf("%.3f ", img->data[i]);
        }
        printf("...");
        for (int i = size - 4; i < size; i++) {
            printf(" %.3f", img->data[i]);
        }
    }
    printf("\n");

    for (int i = 0; i < 20; i++) {
        printf("- ");
    }
    printf("-\n");
}