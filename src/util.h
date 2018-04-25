#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define WIDTH 800
#define HEIGHT 800
#define ITERATIONMAX 1024
#define LENGTH 50

extern u_int8_t imagedata[HEIGHT][WIDTH][3];

void writefile(int filenumber);
u_int8_t ui8min(u_int8_t *value, int length);
u_int8_t ui8max(u_int8_t *value, int length);
double dclamp(double x, double lower, double upper);

#endif