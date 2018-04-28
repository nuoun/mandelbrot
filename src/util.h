#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define WIDTH 800
#define HEIGHT 800
#define ITERATIONMAX 1024
#define LENGTH 100

typedef struct RGB {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} RGB;

typedef struct HSV {
    double h;
    double s;
    double v;
} HSV;

void writefile(RGB imagedata[HEIGHT][WIDTH], int filenumber);

#endif /* UTIL_H */