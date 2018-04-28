#include "util.h"
#include "color.h"

long double centerx = -0.7892360339246801;
long double centery = 0.1506276728392605;
long double escape = 4.0;
long double zoom = 1;
long double zoomrate = 1.075;
long double rotationspeed = 0.015;

long double saturation = 0.75;
long double value = 1.0;
long double paletteshift = 0.0;
long double paletteexpand = 5.0;
long double palettemod = 360.0;

void map(int x, int y, int framenumber, long double *xy)
{
    long double ox, oy;
    long double xmin = -2.25 / zoom;
    long double xmax = 2.25 / zoom;
    long double ymin = -2.25 / zoom;
    long double ymax = 2.25 / zoom;
    long double dx = (xmax - xmin) / (long double)WIDTH;
    long double dy = (ymax - ymin) / (long double)HEIGHT;
    long double cost = cos(rotationspeed * (long double)framenumber);
    long double sint = sin(rotationspeed * (long double)framenumber);
    ox = xmin + (long double)x * dx;
    oy = ymax - (long double)y * dy;
    xy[0] = ox * cost - oy * sint + centerx;
    xy[1] = ox * sint + oy * cost + centery;
}

void renderimage(RGB imagedata[HEIGHT][WIDTH], int framenumber)
{
    long double xy[2], cx, cy, zx, zy, zx2, zy2, z, mu;
    int x, y, i, iteration;
    HSV hsv;
    RGB rgb;
    #pragma omp parallel \
    shared(imagedata, framenumber) \
    private(xy, cx, cy, zx, zy, zx2, zy2, x, y, i, iteration, z, mu, hsv, rgb)
    {
        #pragma omp for schedule(dynamic, 1)
        for (y = 0; y < HEIGHT; y++)
        {
            for (x = 0; x < WIDTH; x++)
            {
                map(x, y, framenumber, xy);
                cx = xy[0];
                cy = xy[1];
                zx = 0.0;
                zy = 0.0;
                zx2 = zx * zx;
                zy2 = zy * zy;
                iteration = 0;
                for (i = 0; i < ITERATIONMAX && ((zx2 + zy2) < escape); i++)
                {
                    zy = 2 * zx * zy + cy;
                    zx = zx2 - zy2 + cx;
                    zx2 = zx * zx;
                    zy2 = zy * zy;
                    iteration++;
                }
                if (iteration == ITERATIONMAX)
                {
                    imagedata[y][x].r = 0;
                    imagedata[y][x].g = 0;
                    imagedata[y][x].b = 0;
                }
                else
                {
                    z = sqrt(zx2 + zy2);
                    mu = (double)iteration - (log(log(z))) / log(2.0);
                    mu = 360.0 * mu / (double)ITERATIONMAX;
                    mu = paletteshift + fmod((mu * paletteexpand) + (double)framenumber, palettemod);

                    hsv.h = mu; hsv.s = saturation; hsv.v = value;
                    hsvtorgb(hsv, &rgb);
                    imagedata[y][x].r = rgb.r;
                    imagedata[y][x].g = rgb.g;
                    imagedata[y][x].b = rgb.b;
                }
            }
        }
    }
    zoom *= zoomrate;
}

int main(void)
{
    void *imagedata = malloc(HEIGHT * WIDTH * sizeof(struct RGB));
    if (imagedata == NULL)
    {
        fprintf(stderr, "ERROR: Out of memory\n");
        exit(-1);
    }
    for (int i = 0; i < LENGTH; i++)
    {
        renderimage(imagedata, i);
        writefile(imagedata, i);
    }
    printf("\n");
    return 0;
}
