#ifndef CCANNY_INCLUDE_IAMGE_H
#define CCANNY_INCLUDE_IAMGE_H

typedef struct {
    float *data;
    int h; // height
    int w; // wigth
    int c; // number of channels
} image_t;

int load_image(const char *path, image_t *img, int req_comp);
int write_image(image_t *img, const char *path);
void destroy_image(image_t *img);
void print_image(image_t *img);

#endif // CCANNY_INCLUDE_IAMGE_H