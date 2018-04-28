#include "color.h"

static uint8_t pui8min(uint8_t *value, int length)
{
    uint8_t min = value[0];
    for (int i = 0; i < length; i++)
    {
        if (value[i] < min)
        {
            min = value[i];
        }
    }
    return min;
}

static uint8_t pui8max(uint8_t *value, int length)
{
    uint8_t max = value[0];
    for (int i = 0; i < length; i++)
    {
        if (value[i] > max)
        {
            max = value[i];
        }
    }
    return max;
}

static double dclamp(double x, double lower, double upper)
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

void hsvtorgb(HSV hsv, RGB *rgb)
{
    hsv.h = dclamp(hsv.h, 0.0, 360.0);
    hsv.s = dclamp(hsv.s, 0.0, 1.0);
    hsv.v = dclamp(hsv.v, 0.0, 1.0);
    uint8_t i;
    double p, q, t, fract;

    if (hsv.s == 0.0)
    {
        rgb->r = rgb->g = rgb->b = (uint8_t)floor(hsv.v * 255);
        return;
    }
    if (hsv.h == 360.0)
    {
        hsv.h = 0.0;
    }
    else
    {
        hsv.h /= 60.0;
    }

    i = (uint8_t)floor(hsv.h);
    fract = hsv.h - floor(hsv.h);
    p = hsv.v * (1.0 - hsv.s);
    q = hsv.v * (1.0 - (hsv.s * fract));
    t = hsv.v * (1.0 - (hsv.s * (1.0 - fract)));

    if (i == 0)
    {
        rgb->r = (uint8_t)floor(hsv.v * 255);
        rgb->g = (uint8_t)floor(t * 255);
        rgb->b = (uint8_t)floor(p * 255);
    }
    else if (i == 1)
    {
        rgb->r = (uint8_t)floor(q * 255);
        rgb->g = (uint8_t)floor(hsv.v * 255);
        rgb->b = (uint8_t)floor(p * 255);
    }
    else if (i == 2)
    {
        rgb->r = (uint8_t)floor(p * 255);
        rgb->g = (uint8_t)floor(hsv.v * 255);
        rgb->b = (uint8_t)floor(t * 255);
    }
    else if (i == 3)
    {
        rgb->r = (uint8_t)floor(p * 255);
        rgb->g = (uint8_t)floor(q * 255);
        rgb->b = (uint8_t)floor(hsv.v * 255);
    }
    else if (i == 4)
    {
        rgb->r = (uint8_t)floor(t * 255);
        rgb->g = (uint8_t)floor(p * 255);
        rgb->b = (uint8_t)floor(hsv.v * 255);
    }
    else
    {
        rgb->r = (uint8_t)floor(hsv.v * 255);
        rgb->g = (uint8_t)floor(p * 255);
        rgb->b = (uint8_t)floor(q * 255);
    }
}

void rgbtohsv(RGB rgb, HSV *hsv)
{
    void *rgbptr = &rgb;
    uint8_t int_min = pui8min(rgbptr, 3);
    uint8_t int_max = pui8max(rgbptr, 3);
    double delta;
    double min = (double)int_min / 255.0;
    double max = (double)int_max / 255.0;
    double r = (double)rgb.r / 255.0;
    double g = (double)rgb.g / 255.0;
    double b = (double)rgb.b / 255.0;
    hsv->v = max;

    if (int_max == 0.0)
    {
        hsv->h = 0.0;
        hsv->s = 0.0;
        return;
    }
    else
    {
        hsv->s = (max - min) / max;
    }
    if (hsv->s == 0.0)
    {
        hsv->h = 0.0;
        return;
    }

    delta = max - min;

    if (rgb.r == int_max)
    {
        hsv->h = (g - b) / delta;
    }
    else if (rgb.g == int_max)
    {
        hsv->h = 2.0 + (b - r) / delta;
    }
    else if (rgb.b == int_max)
    {
        hsv->h = 4.0 + (r - g) / delta;
    }
    hsv->h *= 60.0;
}
