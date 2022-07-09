#include <stdio.h>
#include "image.h"

int main() {
    printf("hello ccanny!\n");
    image_t img;
    const char *src = "./data/girl.png";
    if (load_image(src, &img, 3) == 0) {
        printf("load image from %s successfully\n", src);
    }
    print_image(&img);
    write_image(&img, "./data/girl_write.jpg");
    destroy_image(&img);
    return 0;
}