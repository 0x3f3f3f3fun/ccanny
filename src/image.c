#include "image.h"
#include "log.h"
// stb library
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"
#include <memory.h>

int load_image(const char *path, image_t *img, int req_comp) {
    int h, w, c, parsed_c;
    // load image, memory order is HWC (RGB)
    // see stb_image.h: 170
    unsigned char *buffer = stbi_load(path, &w, &h, &parsed_c, 0);
    if (!buffer) {
        ERROR("failed to load image from %s, %s\n", path, stbi_failure_reason());
        img->h = 0; img->w = 0; img->c = 0;
        img->data = NULL;
        return -1;
    }
    // validate number of channels
    if (parsed_c < req_comp) {
        WARNING("number of channels of image from %s is %d, less than %d", 
                path, parsed_c, req_comp);
        c = parsed_c;
    } else if (req_comp < parsed_c) {
        WARNING("number of channels of image from %s is %d, greater than %d", 
                path, parsed_c, req_comp);
        c = req_comp;
    } else { // parsed_c = req_comp
        c = parsed_c;
    }
    // allocate memory
    float *data = (float*)malloc(sizeof(float) * h * w * c);
    // copy each pixel and convert from unsigned char to float
    for (int i = 0; i < h * w; i++) {
        for (int j = 0; j < c; j++) {
            data[i * c + j] = (float)buffer[i * parsed_c + j];
        }
    }
    // free buffer
    stbi_image_free(buffer);
    // initialize image
    img->h = h; img->w = w; img->c = c;
    img->data = data;
    return 0;
}

int write_image(image_t *img, const char *path) {
    int h = img->h, w = img->w, c = img->c;
    float *data = img->data;

    unsigned char *buffer = (unsigned char*)malloc(sizeof(unsigned char) * h * w * c);
    for (int i = 0; i < h * w * c; i++) {
        buffer[i] = (unsigned char)data[i];
    }
    int ret = stbi_write_jpg(path, w, h, c, buffer, 0);
    free(buffer);

    if (!ret) {
        ERROR("failed to write image to %s", path);
        return -1;
    } else {
        return 0;
    }
}

void destroy_image(image_t *img) {
    free(img->data);
}

void print_image(image_t *img) {
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