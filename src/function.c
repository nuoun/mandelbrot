#include "function.h"

long double zoom = 4;
char* imagedata;

void renderframe(int framenumber)
{
    int i;
    int imagebyte = 0;
    long double cx, cy, ox, oy, zx, zy, zx2, zy2;
    long double xmin = -2.25 * zoom;
    long double xmax = 2.25 * zoom;
    long double ymin = -2.25 * zoom;
    long double ymax = 2.25 * zoom;
    long double dx = (xmax - xmin) / WIDTH;
    long double dy = (ymax - ymin) / HEIGHT;
    float time = framenumber;

    /*
    long double centerx = -1.778103342740640371;
    long double centery = 0.00767394242121339392;

    long double centerx = -0.7473354651820724;
    long double centery = -0.10030992828748005;

    long double centerx = -1.256148879595297;
    long double centery = 0.3994106258544796;

    long double centerx = -1.76651223950935682;
    long double centery = 0.04171432271082462;

    long double centerx = 0.29630415552937667;
    long double centery = 0.01716992131736058;

    long double centerx = 0.0;
    long double centery = 0.0;
    */

    long double centerx = 0.37226792743827275;
    long double centery = -0.6687312047576894;

    float cost = cos(0.0075 * time);
    float sint = sin(0.0075 * time);
    zoom *= 0.96125;

    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {

            ox = xmin + x * dx;
            oy = ymax - y * dy;

            cx = ox * cost - oy * sint + centerx;
            cy = ox * sint + oy * cost + centery;

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
                imagedata[imagebyte++] = 0;
                imagedata[imagebyte++] = 0;
                imagedata[imagebyte++] = 0;
            }
            else
            {
                /*
                int output = (i * 5) + framenumber % 360;

                float log_zn = log(zx2 + zy2) / 2;
                float nu = log(log_zn / log(2)) / log(2);
                int output = (int) i + 1 - nu;
                output = 1 + output + framenumber % 359;
                */

                /*
                float l = i + 1 - log2(log2(zx2+zy2)) + 4.0;
                float output = 0.5 + 0.5 * cos(3.0 + l * 0.15);
                output *= 360 + framenumber % 360;
                */

                float modulus = sqrt (zx2 + zy2);
                float mu = i - (log (log (modulus))) / log(2.0);
                float output = mu * 1.5 + framenumber % 360;

                struct HSV data = {output, 0.75, 1.0};
                struct RGB value = HSVToRGB(data);

                imagedata[imagebyte++] = value.R;
                imagedata[imagebyte++] = value.G;
                imagedata[imagebyte++] = value.B;
            }
        }
    }
}

void writefile(int filenumber)
{
    FILE *fp;
    char filename[24];
    sprintf(filename, "./output/img%d.ppm", filenumber);
    fp = fopen(filename, "w");
    fprintf(fp, "P6 %d %d 255\n", WIDTH, HEIGHT);
    fwrite(imagedata, 1, WIDTH * HEIGHT * 3, fp);
    fclose(fp);
    printf("File %s complete\r", filename);
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