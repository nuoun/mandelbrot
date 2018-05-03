#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>

#define WIDTH 1920
#define HEIGHT 1080
#define LENGTH 1
#define ITERATIONMAX 65535
#define AA 64
#define SAMPLES 4
#define NORMALIZE 0
#define BLUR 0
#define VERBOSE 1

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

unsigned int returnminint(unsigned int data[][WIDTH][SAMPLES]);
unsigned int returnmaxint(unsigned int data[][WIDTH][SAMPLES]);
double returnmindouble(double data[][WIDTH][SAMPLES]);
double returnmaxdouble(double data[][WIDTH][SAMPLES]);
double dclamp(double x, double lower, double upper);
void writefile(RGB imagedata[][WIDTH], int filenumber);

#endif /* UTIL_H */