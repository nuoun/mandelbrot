#include "util.h"
#include "color.h"

long double centerx = 0.3031274851874563;
long double centery = 0.021411693019986702;
long double escape = 1000;
long double zoom = 50000.0;
long double zoomrate = 1.25;
long double rotationrate = 0.015;

void map(int x, int y, int framenumber, long double *xy)
{
    long double ox, oy;
    long double xmin = -2.25l / zoom;
    long double xmax = 2.25l / zoom;
    long double ymin = -2.25l / zoom;
    long double ymax = 2.25l / zoom;
    long double dx = (xmax - xmin) / (long double)HEIGHT;
    long double dy = (ymax - ymin) / (long double)HEIGHT;
    long double cost = cos(rotationrate * (long double)framenumber);
    long double sint = sin(rotationrate * (long double)framenumber);
    ox = xmin + (long double)x * dx;
    oy = ymax - (long double)y * dy;
    xy[0] = ox * cost - oy * sint + centerx;
    xy[1] = ox * sint + oy * cost + centery;
}

void calculate(int iterationdata[HEIGHT][WIDTH], double magnitudedata[HEIGHT][WIDTH], int framenumber)
{
    long double xy[2], cx, cy, zx, zy, zx2, zy2;
    int x, y, i;
    #pragma omp parallel \
    shared(iterationdata, magnitudedata, framenumber) \
    private(xy, cx, cy, zx, zy, zx2, zy2, x, y, i)
    {
        #pragma omp for schedule(dynamic, 1)
        for (y = 0; y < HEIGHT; y++)
        {
            for (x = 0; x < WIDTH; x++)
            {
                map(x, y, framenumber, xy);
                cx = xy[0];
                cy = xy[1];
                zx = 0.0l;
                zy = 0.0l;
                zx2 = zx * zx;
                zy2 = zy * zy;
                for (i = 0; i < ITERATIONMAX && (zx2 + zy2 < escape); i++)
                {
                    zy = 2.0l * zx * zy + cy;
                    zx = zx2 - zy2 + cx;
                    zx2 = zx * zx;
                    zy2 = zy * zy;
                }
                if (i == ITERATIONMAX)
                {
                    iterationdata[y][x] = 0;
                }
                else
                {
                    iterationdata[y][x] = i;
                    magnitudedata[y][x] = sqrt(zx2 + zy2);
                }
            }
        }
    }
    zoom *= zoomrate;
}

void renderimage(int iterationdata[HEIGHT][WIDTH], double magnitudedata[HEIGHT][WIDTH], RGB imagedata[HEIGHT][WIDTH], int framenumber) 
{
    double iteration, hue, value;
    int x, y, maxiter;
    HSV hsv;
    RGB rgb;
    maxiter = returnmaxint(iterationdata);
    #pragma omp parallel \
    shared(iterationdata, magnitudedata, imagedata, framenumber, maxiter) \
    private(iteration, hue, value, x, y, hsv, rgb)
    {
        #pragma omp for schedule(dynamic, 1)
        for (y = 0; y < HEIGHT; y++)
        {
            for (x = 0; x < WIDTH; x++)
            {
                if (iterationdata[y][x] == 0)
                {
                    imagedata[y][x].r = 0; imagedata[y][x].g = 0; imagedata[y][x].b = 0;
                }
                else
                {
                    iteration = (double)iterationdata[y][x] * ((double)ITERATIONMAX / (double)maxiter);
                    magnitudedata[y][x] = iteration - (log(log(magnitudedata[y][x]))) / log (2.0);
                    magnitudedata[y][x] = 0.5 + 0.5 * cos(3 + magnitudedata[y][x] * 0.15);
                    hue = 180.0 + magnitudedata[y][x] * 90.0;
                    hue = fmod(hue + framenumber, 360.0);
                    value = 1.0 - magnitudedata[y][x] * 0.75;
                    saturation = 1.0 - magnitudedata[y][x] * 0.25;
                    hsv.h = hue; hsv.s = saturation; hsv.v = value;
                    hsvtorgb(hsv, &rgb);
                    imagedata[y][x].r = rgb.r; imagedata[y][x].g = rgb.g; imagedata[y][x].b = rgb.b;
                }    
            }
        }
    }
}

void procesimage(RGB imagedata[HEIGHT][WIDTH])
{
    int x, y;
    RGB (*imagecopy)[HEIGHT][WIDTH] = malloc(HEIGHT * WIDTH * sizeof(struct RGB));
    memcpy(imagecopy, imagedata, HEIGHT * WIDTH * sizeof(struct RGB));
    #pragma omp parallel \
    shared(imagedata, imagecopy) \
    private(x, y)
    {
        #pragma omp for schedule(dynamic, 1)
        for (y = 0; y < HEIGHT; y++)
        {
            for (x = 0; x < WIDTH; x++)
            {
                if (x == 0 && y == 0)
                {
                    imagedata[y][x].r = floor(((*imagecopy)[y][x].r + (*imagecopy)[y + 1][x].r + (*imagecopy)[y][x + 1].r) / 3);
                    imagedata[y][x].g = floor(((*imagecopy)[y][x].g + (*imagecopy)[y + 1][x].g + (*imagecopy)[y][x + 1].g) / 3);
                    imagedata[y][x].b = floor(((*imagecopy)[y][x].b + (*imagecopy)[y + 1][x].b + (*imagecopy)[y][x + 1].b) / 3);
                }
                else if (x == 0)
                {
                    imagedata[y][x].r = floor(((*imagecopy)[y][x].r + (*imagecopy)[y - 1][x].r + (*imagecopy)[y][x + 1].r) / 3);
                    imagedata[y][x].g = floor(((*imagecopy)[y][x].g + (*imagecopy)[y - 1][x].g + (*imagecopy)[y][x + 1].g) / 3);
                    imagedata[y][x].b = floor(((*imagecopy)[y][x].b + (*imagecopy)[y - 1][x].b + (*imagecopy)[y][x + 1].b) / 3);
                }
                else if (y == 0)
                {
                    imagedata[y][x].r = floor(((*imagecopy)[y][x].r + (*imagecopy)[y + 1][x].r + (*imagecopy)[y][x - 1].r) / 3);
                    imagedata[y][x].g = floor(((*imagecopy)[y][x].g + (*imagecopy)[y + 1][x].g + (*imagecopy)[y][x - 1].g) / 3);
                    imagedata[y][x].b = floor(((*imagecopy)[y][x].b + (*imagecopy)[y + 1][x].b + (*imagecopy)[y][x - 1].b) / 3);
                }
                else
                {
                    imagedata[y][x].r = floor(((*imagecopy)[y][x].r + (*imagecopy)[y - 1][x].r + (*imagecopy)[y][x - 1].r) / 3);
                    imagedata[y][x].g = floor(((*imagecopy)[y][x].g + (*imagecopy)[y - 1][x].g + (*imagecopy)[y][x - 1].g) / 3);
                    imagedata[y][x].b = floor(((*imagecopy)[y][x].b + (*imagecopy)[y - 1][x].b + (*imagecopy)[y][x - 1].b) / 3);
                }
            }
        }
    }
}

int main(void)
{
    void *iterationdata = malloc(HEIGHT * WIDTH * sizeof(unsigned int));
    void *magnitutedata = malloc(HEIGHT * WIDTH * sizeof(double));
    void *imagedata = malloc(HEIGHT * WIDTH * sizeof(struct RGB));
    if (imagedata == NULL)
    {
        fprintf(stderr, "ERROR: Out of memory\n");
        exit(-1);
    }
    for (int i = 0; i < LENGTH; i++)
    {
        calculate(iterationdata, magnitutedata, i);
        renderimage(iterationdata, magnitutedata, imagedata, i);
        procesimage(imagedata);
        writefile(imagedata, i);
    }
    printf("\n");
    return 0;
}
