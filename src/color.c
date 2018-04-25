#include "color.h"

void hsvtorgb(double hsv[3], u_int8_t *rgb)
{
    hsv[0] = dclamp(hsv[0], 0.0, 360.0);
    hsv[1] = dclamp(hsv[1], 0.0, 1.0);
    hsv[2] = dclamp(hsv[2], 0.0, 1.0);
    u_int8_t i;
    double p, q, t, fract;

    if (hsv[1] == 0.0)
    {
        rgb[0] = rgb[1] = rgb[2] = (u_int8_t)floor(hsv[2] * 255);
        return;
    }
    if (hsv[0] == 360.0)
    {
        hsv[0] = 0.0;
    }
    else
    {
        hsv[0] /= 60.0;
    }

    i = (u_int8_t)floor(hsv[0]);
    fract = hsv[0] - floor(hsv[0]);
    p = hsv[2] * (1.0 - hsv[1]);
    q = hsv[2] * (1.0 - (hsv[1] * fract));
    t = hsv[2] * (1.0 - (hsv[1] * (1.0 - fract)));

    switch (i)
    {
        case 0:
            rgb[0] = (u_int8_t)floor(hsv[2] * 255);
            rgb[1] = (u_int8_t)floor(t * 255);
            rgb[2] = (u_int8_t)floor(p * 255);
            break;
        case 1:
            rgb[0] = (u_int8_t)floor(q * 255);
            rgb[1] = (u_int8_t)floor(hsv[2] * 255);
            rgb[2] = (u_int8_t)floor(p * 255);
            break;
        case 2:
            rgb[0] = (u_int8_t)floor(p * 255);
            rgb[1] = (u_int8_t)floor(hsv[2] * 255);
            rgb[2] = (u_int8_t)floor(t * 255);
            break;
        case 3:
            rgb[0] = (u_int8_t)floor(p * 255);
            rgb[1] = (u_int8_t)floor(q * 255);
            rgb[2] = (u_int8_t)floor(hsv[2] * 255);
            break;
        case 4:
            rgb[0] = (u_int8_t)floor(t * 255);
            rgb[1] = (u_int8_t)floor(p * 255);
            rgb[2] = (u_int8_t)floor(hsv[2] * 255);
            break;
        default:
            rgb[0] = (u_int8_t)floor(hsv[2] * 255);
            rgb[1] = (u_int8_t)floor(p * 255);
            rgb[2] = (u_int8_t)floor(q * 255);
            break;
    }
}

void rgbtohsv(u_int8_t rgb[3], double *hsv)
{
    u_int8_t int_min = ui8min(rgb, 3);
    u_int8_t int_max = ui8max(rgb, 3);
    double delta;
    double min = (double)int_min / 255.0;
    double max = (double)int_max / 255.0;
    double r = (double)rgb[0] / 255.0;
    double g = (double)rgb[1] / 255.0;
    double b = (double)rgb[2] / 255.0;
    hsv[2] = max;

    if (int_max == 0)
    {
        hsv[0] = 0.0;
        hsv[1] = 0.0; 
        return;        
    }
    else
    {
        hsv[1] = (max - min) / max;
    }
    if (hsv[1] == 0)
    {
        hsv[0] = 0.0;
        return;
    }

    delta = max - min;

    if (rgb[0] == int_max) {
        hsv[0] = (g - b) / delta;
    }
    else if (rgb[1] == int_max) {
        hsv[0] = 2.0 + (b - r) / delta;
    }
    else if (rgb[2] == int_max) {
        hsv[0] = 4.0 + (r - g) / delta;
    }
    hsv[0] *= 60.0;
}
