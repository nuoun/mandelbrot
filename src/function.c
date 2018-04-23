#include "function.h"

char* image;
long double zoom = 1.0;

void draw_frame(int framecount)
{
    
    int i;
    int imgidx = 0;
    long double cx, cy, zx, zy, zx2, zy2;
    long double xmin = -2.25 * zoom;
    long double xmax = 2.25 * zoom;
    long double ymin = -2.25 * zoom;
    long double ymax = 2.25 * zoom;
    long double dx = (xmax - xmin) / WIDTH;
    long double dy = (ymax - ymin) / HEIGHT;

    zoom *= 0.975;

    /*
    long double centerx = -1.76651223650935682;
    long double centery = 0.04171432271082462;
    long double centerx = -0.7473354651820724;
    long double centery = -0.10030992828748005;
    long double centerx = -1.256148879595297;
    long double centery = 0.3994106258544796;
    long double centerx = -1.77810334274064037110522326038852639499207961414628307584575173232969154440;
    long double centery = 0.00767394242121339392672671947893471774958985018535019684946671264012302378;
    long double centerx = -1.76651223950935682;
    long double centery = 0.04171432271082462;
    */

    long double centerx = 0;
    long double centery = 0;

    for (int y = 0; y < HEIGHT; y++)
    {
        cy = centery + ymax - y * dy;
        for (int x = 0; x < WIDTH; x++)
        {
            cx = centerx + xmin + x * dx;
            zx = 0.0;
            zy = 0.0;
            zx2 = zx * zx;
            zy2 = zy * zy;

            for (i = 0; i < ITERATIONMAX && ((zx2 + zy2) < ESCAPE); i++)
            {
                zy = 2 * zx * zy + cy;
                zx = zx2 - zy2 + cx;
                zx2 = zx * zx;
                zy2 = zy * zy;
            }
            if (i == ITERATIONMAX)
            {
                image[imgidx++] = 0;
                image[imgidx++] = 0;
                image[imgidx++] = 0;
            }
            else
            {
                /*
                double z = zx + zy;
                int smooth = 360 * log2(1.75 + i - log2(log2(z))) / log2((double)ITERATIONMAX);
                smooth = (smooth + framecount) % 360;

                fprintf( stderr,  "%d\n\n", smooth);
                */

                int output = (i * 5) + framecount % 360;

                struct HSV data = {output, 0.75, 0.75};
                struct RGB value = HSVToRGB(data);

                image[imgidx++] = value.R;
                image[imgidx++] = value.G;
                image[imgidx++] = value.B;
            }
        }
    }
}

void output_frame(void)
{
    fprintf(stdout, "P6 %d %d %d\n", WIDTH, HEIGHT, 255);
    fwrite(image, 1, WIDTH * HEIGHT * 3, stdout);
    fflush(stdout);
}

struct RGB HSVToRGB(struct HSV hsv)
{
    double r = 0, g = 0, b = 0;

    if (hsv.S == 0)
    {
        r = hsv.V;
        g = hsv.V;
        b = hsv.V;
    }
    else
    {
        int i;
        double f, p, q, t;

        if (hsv.H == 360)
            hsv.H = 0;
        else
            hsv.H = hsv.H / 60;

        i = (int)trunc(hsv.H);
        f = hsv.H - i;

        p = hsv.V * (1.0 - hsv.S);
        q = hsv.V * (1.0 - (hsv.S * f));
        t = hsv.V * (1.0 - (hsv.S * (1.0 - f)));

        switch (i)
        {
        case 0:
            r = hsv.V;
            g = t;
            b = p;
            break;

        case 1:
            r = q;
            g = hsv.V;
            b = p;
            break;

        case 2:
            r = p;
            g = hsv.V;
            b = t;
            break;

        case 3:
            r = p;
            g = q;
            b = hsv.V;
            break;

        case 4:
            r = t;
            g = p;
            b = hsv.V;
            break;

        default:
            r = hsv.V;
            g = p;
            b = q;
            break;
        }
    }

    struct RGB rgb;

    rgb.R = r * 255;
    rgb.G = g * 255;
    rgb.B = b * 255;

    return rgb;
}