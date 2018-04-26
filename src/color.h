/*
Based on Agoston, M, 2005, Computer Graphics and Geometric Modeling: Implementation and Algorithms, p. 303, 304
RGB to HSV Input: integer, R, G, B, ∈ [0, 255] / Output: double, H ∈ [0, 360] and S,V ∈ [0, 1]
HSV to RGB Input: double, H ∈ [0, 360] and S,V ∈ [0, 1] / Output: integer, R, G, B, ∈ [0, 255]
*/

#ifndef COLOR_H
#define COLOR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

void rgbtohsv(u_int8_t rgb[3], double *hsv);
void hsvtorgb(double hsv[3], u_int8_t *rgb);

#endif