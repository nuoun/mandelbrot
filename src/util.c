#include "util.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../inc/stb_image_write.h"

int returnmaxint(int data[HEIGHT][WIDTH])
{
    int max = data[0][0];
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            if (data[y][x] > max)
            {
                max = data[y][x];
            }
        }
    }
    return max;
}

double returnmaxdouble(double data[HEIGHT][WIDTH])
{
    double max = data[0][0];
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            if (data[y][x] > max)
            {
                max = data[y][x];
            }
        }
    }
    return max;
}

double returnmindouble(double data[HEIGHT][WIDTH])
{
    double min = data[0][0];
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            if (data[y][x] < min)
            {
                min = data[y][x];
            }
        }
    }
    return min;
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

void writefile(RGB imagedata[HEIGHT][WIDTH], int filenumber)
{
    char filename[24];
    sprintf(filename, "./output/img%d.png", filenumber + 1);
    stbi_write_png(filename, WIDTH, HEIGHT, 3, imagedata, WIDTH * 3);
    printf("File %s complete\r", filename);
    fflush(stdout);
}
