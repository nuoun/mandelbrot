#include "util.h"
#include "color.h"

long double centerx = 0.3031274851874563;
long double centery = 0.021411693019986702;
long double escape = 1000;
long double zoom = 5000.0;
long double zoomrate = 1.25;
long double rotationrate = 0.015;

void map(int x, int y, int a, int framenumber, long double *xy)
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
    if (a == 0)
    {
        ox -= dx / 4;
        oy -= dy / 4;
    }
    else if (a == 1)
    {
        ox += dx / 4;
        oy -= dy / 4;
    }
    else if (a == 2)
    {
        ox -= dx / 4;
        oy += dy / 4;

    }
    else
    {
        ox += dx / 4;
        oy += dy / 4;
    }
    xy[0] = ox * cost - oy * sint + centerx;
    xy[1] = ox * sint + oy * cost + centery;
}

void calculate(unsigned int iterationdata[HEIGHT][WIDTH][AA], double magnitudedata[HEIGHT][WIDTH][AA], int framenumber)
{
    long double xy[2], cx, cy, zx, zy, zx2, zy2;
    int x, y, a;
    unsigned int i;
    #pragma omp parallel \
    shared(iterationdata, magnitudedata, framenumber) \
    private(xy, cx, cy, zx, zy, zx2, zy2, x, y, a, i)
    {
        #pragma omp for schedule(dynamic, 1)
        for (y = 0; y < HEIGHT; y++)
        {
            for (x = 0; x < WIDTH; x++)
            {
                for (a = 0; a < AA; a++)
                {
                    map(x, y, a, framenumber, xy);
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
                        iterationdata[y][x][a] = 0;
                    }
                    else
                    {
                        iterationdata[y][x][a] = i;
                        magnitudedata[y][x][a] = sqrt(zx2 + zy2);
                    }
                }
            }
        }
    }
    zoom *= zoomrate;
}

void render(unsigned int iterationdata[HEIGHT][WIDTH][AA], double magnitudedata[HEIGHT][WIDTH][AA], RGB sampledata[HEIGHT][WIDTH][AA], int framenumber) 
{

    double mu, hue, value;
    int x, y, a;
    unsigned int maxiter;
    HSV hsv;
    RGB rgb;
    maxiter = returnmaxint(iterationdata);
    #pragma omp parallel \
    shared(iterationdata, magnitudedata, sampledata, framenumber, maxiter) \
    private(mu, hue, value, x, y, a, hsv, rgb)
    {
        #pragma omp for schedule(dynamic, 1)
        for (y = 0; y < HEIGHT; y++)
        {
            for (x = 0; x < WIDTH; x++)
            {
                for (a = 0; a < AA; a++)
                {
                    if (iterationdata[y][x][0] == 0 || iterationdata[y][x][1] == 0 || iterationdata[y][x][2] == 0 || iterationdata[y][x][3] == 0)
                    {
                        sampledata[y][x][a].r = 0; sampledata[y][x][a].g = 0; sampledata[y][x][a].b = 0;
                    }
                    else
                    {
                        mu = (double)iterationdata[y][x][a] * ((double)ITERATIONMAX / (double)maxiter);
                        mu = mu - (log(log(magnitudedata[y][x][a]))) / log(2.0);
                        mu = 0.5 + 0.5 * cos(3 + mu * 0.15);
                        hue = 180.0 + mu * 90.0;
                        hue = fmod(hue + framenumber, 360.0);
                        value = 1.0 - mu * 0.75;
                        hsv.h = hue; hsv.s = 0.75; hsv.v = value;
                        hsvtorgb(hsv, &rgb);
                        sampledata[y][x][a].r = rgb.r; sampledata[y][x][a].g = rgb.g; sampledata[y][x][a].b = rgb.b;
                    }
                }    
            }
        }
    }
}

void average(unsigned int iterationdata[HEIGHT][WIDTH][AA], RGB sampledata[HEIGHT][WIDTH][AA], RGB imagedata[HEIGHT][WIDTH])
{
    int x, y, a;
    #pragma omp parallel \
    shared(iterationdata, sampledata, imagedata) \
    private(x, y, a)
    {
        #pragma omp for schedule(dynamic, 1)
        for (y = 0; y < HEIGHT; y++)
        {
            for (x = 0; x < WIDTH; x++)
            {
                for (a = 0; a < AA; a++)
                {
                    if (iterationdata[y][x][0] == 0 || iterationdata[y][x][1] == 0 || iterationdata[y][x][2] == 0 || iterationdata[y][x][3] == 0)
                    {
                        sampledata[y][x][a].r = 0; sampledata[y][x][a].g = 0; sampledata[y][x][a].b = 0;
                    }
                    else
                    {
                        imagedata[y][x].r = floor((sampledata[y][x][0].r + sampledata[y][x][1].r + sampledata[y][x][2].r + sampledata[y][x][3].r) / 4);
                        imagedata[y][x].g = floor((sampledata[y][x][0].g + sampledata[y][x][1].g + sampledata[y][x][2].g + sampledata[y][x][3].g) / 4);
                        imagedata[y][x].b = floor((sampledata[y][x][0].b + sampledata[y][x][1].b + sampledata[y][x][2].b + sampledata[y][x][3].b) / 4);
                    }
                }
            }
        }
    }
}

void process(RGB imagedata[HEIGHT][WIDTH], RGB processdata[HEIGHT][WIDTH])
{
    int x, y;
    memcpy(processdata, imagedata, HEIGHT * WIDTH * sizeof(struct RGB));
    #pragma omp parallel \
    shared(imagedata, processdata) \
    private(x, y)
    {
        #pragma omp for schedule(dynamic, 1)
        for (y = 0; y < HEIGHT; y++)
        {
            for (x = 0; x < WIDTH; x++)
            {
                if (x == 0 || y == 0)
                {
                    imagedata[y][x].r = floor((processdata[y][x].r + processdata[y + 1][x].r + processdata[y][x + 1].r) / 3);
                    imagedata[y][x].g = floor((processdata[y][x].g + processdata[y + 1][x].g + processdata[y][x + 1].g) / 3);
                    imagedata[y][x].b = floor((processdata[y][x].b + processdata[y + 1][x].b + processdata[y][x + 1].b) / 3);
                }
                else
                {
                    imagedata[y][x].r = floor((processdata[y][x].r + processdata[y - 1][x].r + processdata[y][x - 1].r) / 3);
                    imagedata[y][x].g = floor((processdata[y][x].g + processdata[y - 1][x].g + processdata[y][x - 1].g) / 3);
                    imagedata[y][x].b = floor((processdata[y][x].b + processdata[y - 1][x].b + processdata[y][x - 1].b) / 3);
                }
            }
        }
    }
}

int main(void)
{
    void *iterationdata = malloc(HEIGHT * WIDTH * AA * sizeof(unsigned int));
    void *magnitutedata = malloc(HEIGHT * WIDTH * AA * sizeof(double));
    void *sampledata = malloc(HEIGHT * WIDTH * AA * sizeof(struct RGB));
    void *imagedata = malloc(HEIGHT * WIDTH * sizeof(struct RGB));
    void *processdata = malloc(HEIGHT * WIDTH * sizeof(struct RGB));
    if (iterationdata == NULL || magnitutedata == NULL || sampledata == NULL || imagedata == NULL || processdata == NULL)
    {
        fprintf(stderr, "ERROR: Out of memory\n");
        exit(-1);
    }
    for (int i = 0; i < LENGTH; i++)
    {
        calculate(iterationdata, magnitutedata, i);
        render(iterationdata, magnitutedata, sampledata, i);
        average(iterationdata, sampledata, imagedata);
        process(imagedata, processdata);
        writefile(imagedata, i);
    }
    printf("\n");
    return 0;
}
