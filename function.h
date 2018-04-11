#ifndef MAIN_H // include guard
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define W 800
#define HEIGHT 800
#define M 255
#define ITERATIONMAX 1024
#define ESCAPE 4

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

#endif // MAIN_H