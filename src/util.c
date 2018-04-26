#include "util.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../inc/stb_image_write.h"

void
writefile(int filenumber)
{
    char filename[24];
    sprintf(filename, "./output/img%d.png", filenumber + 1);
    stbi_write_png(filename, WIDTH, HEIGHT, 3, imagedata, WIDTH * 3);
    printf("File %s complete \r", filename);
    fflush(stdout);
}
