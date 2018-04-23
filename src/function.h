/* include guard */
#ifndef FUNCTION_H
#define FUNCTION_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define WIDTH 800
#define HEIGHT 800
#define ITERATIONMAX 1024
#define ESCAPE 4
#define LENGTH 100

extern char* image;

struct RGB
{
    int R;
    int G;
    int B;
};

struct HSV
{
    double H;
    double S;
    double V;
};

struct RGB HSVToRGB(struct HSV hsv);
void draw_frame(int);
void output_frame(void);

/* FUNCTION_H */
#endif