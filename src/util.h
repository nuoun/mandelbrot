#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>

#define WIDTH 1920
#define HEIGHT 1080
#define ITERATIONMAX 65535
#define LENGTH 1

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

int returnmaxint(int data[HEIGHT][WIDTH]);
double returnmaxdouble(double data[HEIGHT][WIDTH]);
double returnmindouble(double data[HEIGHT][WIDTH]);
double dclamp(double x, double lower, double upper);
void writefile(RGB imagedata[HEIGHT][WIDTH], int filenumber);

#endif /* UTIL_H */