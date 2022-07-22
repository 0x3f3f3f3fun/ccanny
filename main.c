#include <stdio.h>
#include "image.h"

int main() {
    printf("hello ccanny!\n");
    image_t img;
    
    const char *src = "./data/girl.jpg";
    if (load_image(src, &img, RGB)) {
        printf("load image from %s\n", src);
    }
    
    print_image(&img);

    const char *dst = "./data/girl_write.jpg";
    if (write_image(&img, dst)) {
        printf("write image to %s\n", dst);
    }

    destroy_image(&img);
    return 0;
}