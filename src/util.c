#include "util.h"
#include "color.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../inc/stb_image_write.h"

void writefile(int filenumber)
{
    char filename[24];
    sprintf(filename, "./output/img%d.png", filenumber);
    stbi_write_png(filename, WIDTH, HEIGHT, 3, imagedata, WIDTH * 3);
    printf("File %s complete \r", filename);
    fflush(stdout);
}

u_int8_t ui8min(u_int8_t *value, int length)
{
    u_int8_t min = value[0];
    for (int i = 0; i < length; i++)
    {
        if (value[i] < min){
            min = value[i];
        }
    }
    return min;
}

u_int8_t ui8max(u_int8_t *value, int length)
{
    u_int8_t max = value[0];
    for (int i = 0; i < length; i++)
    {
        if (value[i] > max)
        {
            max = value[i];
        }
    }
    return max;
}

double dclamp(double x, double lower, double upper)
{
    if (x < lower) 
    {
        return lower;
    }
    if (x > upper)
    {
        return upper;
    }
    return x;
}
