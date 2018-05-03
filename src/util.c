#include "util.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../inc/stb_image_write.h"

unsigned int returnminint(unsigned int data[][WIDTH][SAMPLES])
{
    unsigned int min = data[0][0][0];
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            for (int z = 0; z < SAMPLES; z++)
            {   
                if (data[y][x][z] < min)
                {
                    min = data[y][x][z];
                }
            }
        }
    }
    return min;
}

unsigned int returnmaxint(unsigned int data[][WIDTH][SAMPLES])
{
    unsigned int max = data[0][0][0];
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            for (int z = 0; z < SAMPLES; z++)
            {   
                if (data[y][x][z] > max)
                {
                    max = data[y][x][z];
                }
            }
        }
    }
    return max;
}

double returnmindouble(double data[][WIDTH][SAMPLES])
{
    double min = data[0][0][0];
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            for (int z = 0; z < SAMPLES; z++)
            {   
                if (data[y][x][z] < min)
                {
                    min = data[y][x][z];
                }
            }
        }
    }
    return min;
}

double returnmaxdouble(double data[][WIDTH][SAMPLES])
{
    double max = data[0][0][0];
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            for (int z = 0; z < SAMPLES; z++)
            {   
                if (data[y][x][z] > max)
                {
                    max = data[y][x][z];
                }
            }
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

void writefile(RGB imagedata[][WIDTH], int filenumber)
{
    char filename[24];
    sprintf(filename, "./output/img%d.png", filenumber + 1);
    stbi_write_png(filename, WIDTH, HEIGHT, 3, imagedata, WIDTH * 3);
    if (VERBOSE != 0)
    {
        printf("File %s complete\n", filename);
    }
    else
    {
        printf("File %s complete\r", filename);
        fflush(stdout);
    }
}
