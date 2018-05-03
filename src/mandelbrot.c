#include "util.h"
#include "color.h"

/*
long double centerx = -1.778103342740640371;
long double centery = 0.00767394242121339392;
long double zoom = 5000.0;

long double centerx = -0.7473354651820724;
long double centery = -0.10030992828748005;
long double zoom = 5000.0;

long double centerx = -1.76651223950935682;
long double centery = 0.04171432271082462;
long double zoom = 1000.0;

long double centerx = 0.37226792743827275;
long double centery = -0.6687312047576894;
long double zoom = 500.0;

long double centerx = 0.3031274851874563;
long double centery = 0.021411693019986702;
long double zoom = 5000.0;

long double centerx = -0.5;
long double centery = 0.0;
long double zoom = 1.5;
*/

long double centerx = 0.3031274851874563;
long double centery = 0.021411693019986702;
long double zoom = 5000.0;

long double escape = 1000.0;
long double zoomrate = 1.0125;
long double rotationrate = 0.0075;

void map(int x, int y, int z, int framenumber, long double *xy)
{
    long double xmin = -2.25 / zoom;
    long double xmax = 2.25 / zoom;
    long double ymin = -2.25 / zoom;
    long double ymax = 2.25 / zoom;
    long double dx = (xmax - xmin) / HEIGHT;
    long double dy = (ymax - ymin) / HEIGHT;
    long double cost = cos(rotationrate * framenumber);
    long double sint = sin(rotationrate * framenumber);
    long double ox = xmin + dx * x + centerx;
    long double oy = ymax - dy * y + centery;
    xy[0] = ox * cost - oy * sint;
    xy[1] = ox * sint + oy * cost;
    if (WIDTH > HEIGHT)
    {
        xy[0] -= dx * (WIDTH - HEIGHT) / 2.0;
    }
    if (AA != 0 && SAMPLES > 1 && z)
    {
        xy[0] = xy[0] + 0.5 * dx * cos(z + 1 * M_PI / SAMPLES - 1);
        xy[1] = xy[1] + 0.5 * dy * sin(z + 1 * M_PI / SAMPLES - 1);
    }
}

void iterate(unsigned int iterationdata[][WIDTH][SAMPLES], double magnitudedata[][WIDTH][SAMPLES], int framenumber)
{
    if (VERBOSE != 0)
    {
        printf("Iterating...\n");
    }
    long double xy[2], zx, zy, zx2, zy2;
    int x, y, z, samples;
    unsigned int i;
    #pragma omp parallel \
    shared(iterationdata, magnitudedata, framenumber) \
    private(xy, zx, zy, zx2, zy2, x, y, z, samples, i)
    {
        #pragma omp for schedule(dynamic, 3)
        for (y = 0; y < HEIGHT; y++)
        {
            for (x = 0; x < WIDTH; x++)
            {
                if (AA != 0)
                {
                    samples = SAMPLES;
                }
                else
                {
                    samples = 1;
                }
                for (z = 0; z < samples; z++)
                {
                    map(x, y, z, framenumber, xy);
                    zx = zy = zx2 = zy2 = 0.0;
                    for (i = 0; i < ITERATIONMAX && (zx2 + zy2 < escape); i++)
                    {
                        zy = 2.0 * zx * zy + xy[1];
                        zx = zx2 - zy2 + xy[0];
                        zx2 = zx * zx;
                        zy2 = zy * zy;
                    }
                    if (i == ITERATIONMAX)
                    {
                        iterationdata[y][x][z] = 0;
                    }
                    else
                    {
                        iterationdata[y][x][z] = i;
                        magnitudedata[y][x][z] = sqrt(zx2 + zy2);
                    }
                }
            }
        }
    }
    if (VERBOSE != 0)
    {    
        printf("Lowest iteration(s): %d, highest iteration(s): %d\n", returnminint(iterationdata) + 1, returnmaxint(iterationdata) + 1);
        printf("Escape radius: %.1Lf, lowest magnitude: %.1f, highest magnitude: %.1f\n", escape, returnmindouble(magnitudedata), returnmaxdouble(magnitudedata));
    }
    zoom *= zoomrate;
}

void render(unsigned int iterationdata[][WIDTH][SAMPLES], double magnitudedata[][WIDTH][SAMPLES], RGB sampledata[][WIDTH][SAMPLES], int framenumber) 
{
    double mu, hue, saturation, value, maxiter;
    int x, y, z;
    if (VERBOSE != 0)
    {
        printf("Rendering...\n");
    }
    if (NORMALIZE != 0)
    {
        maxiter = returnmaxint(iterationdata);
    }
    HSV hsv;
    RGB rgb;
    #pragma omp parallel \
    shared(iterationdata, magnitudedata, sampledata, framenumber, maxiter) \
    private(mu, hue, saturation, value, x, y, z, hsv, rgb)
    {
        #pragma omp for schedule(dynamic, 3)
        for (y = 0; y < HEIGHT; y++)
        {
            for (x = 0; x < WIDTH; x++)
            {
                for (z = 0; z < SAMPLES; z++)
                {
                    if (iterationdata[y][x][z] == 0)
                    {
                        sampledata[y][x][z].r = 0; sampledata[y][x][z].g = 0; sampledata[y][x][z].b = 0;
                    }
                    else
                    {
                        /*
                        http://www.fractalforums.com/programming/please-explain-smooth-coloring/
                        mu = iterationdata[y][x][z] - (log(0.5 * (magnitudedata[y][x][z])) - log(0.5 * log(escape))) / log(2);
                        mu = iterationdata[y][x][z] - (log(0.5 * log((magnitudedata[y][x][z]))) - log(0.5 * log(escape))) / log(2);
                        mu = iterationdata[y][x][z] - (log(log((magnitudedata[y][x][z]))) - log(log(escape))) / log(2);

                        http://iquilezles.org/www/articles/mset_smooth/mset_smooth.htm
                        mu = iterationdata[y][x][z] - log2(log2(magnitudedata[y][x][z])) + 4.0;
                        
                        https://linas.org/art-gallery/escape/smooth.html
                        mu = iterationdata[y][x][z] - (log(log(magnitudedata[y][x][z]))) / log(2.0);
                        
                        mu = ITERATIONMAX / iterationdata[y][x][z];
                        */
                        if (NORMALIZE != 0)
                        {
                            iterationdata[y][x][z] = iterationdata[y][x][z] * ITERATIONMAX / maxiter;
                        }
                        mu = iterationdata[y][x][z] - (log(log((magnitudedata[y][x][z]))) - log(log(escape))) / log(2);
                        mu = 0.5 + 0.5 * cos(3.0 + mu * 0.15);
                        hue = 180.0 + mu * 90.0;
                        hue = fmod(hue + framenumber, 360.0);
                        saturation = 0.75;
                        value = 1.0 - mu * 0.75;
                        hsv.h = hue; hsv.s = saturation; hsv.v = value;
                        hsvtorgb(hsv, &rgb);
                        sampledata[y][x][z].r = rgb.r; sampledata[y][x][z].g = rgb.g; sampledata[y][x][z].b = rgb.b;
                    }
                }    
            }
        }
    }
    zoom *= zoomrate;
}

void average(RGB sampledata[][WIDTH][SAMPLES], RGB imagedata[][WIDTH])
{
    if (VERBOSE != 0)
    {
        printf("Avering samples...\n");
    }
    int x, y, z;
    double r, g, b;
    #pragma omp parallel \
    shared(sampledata, imagedata) \
    private(x, y, z, r, g, b)
    {
        #pragma omp for schedule(dynamic, 3)
        for (y = 0; y < HEIGHT; y++)
        {
            for (x = 0; x < WIDTH; x++)
            {
                r = g = b = 0;
                for (z = 0; z < SAMPLES; z++)
                {
                    r += sampledata[y][x][z].r;
                    g += sampledata[y][x][z].g;
                    b += sampledata[y][x][z].b;
                }
                imagedata[y][x].r = round(r / SAMPLES);
                imagedata[y][x].g = round(g / SAMPLES);
                imagedata[y][x].b = round(b / SAMPLES);
            }
        }
    }
}

void blur(RGB imagedata[][WIDTH], RGB processdata[][WIDTH])
{
    if (VERBOSE != 0)
    {
        printf("Post processing...\n");
    }
    int x, y;
    memcpy(processdata, imagedata, HEIGHT * WIDTH * sizeof(struct RGB));
    #pragma omp parallel \
    shared(imagedata, processdata) \
    private(x, y)
    {
        #pragma omp for schedule(dynamic, 3)
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
    if (VERBOSE != 0)
    {
        if (AA != 0)
        {
            printf("Rendering %d frame(s) @ %d x %d with %d iterations and %d sample(s)\n", LENGTH, WIDTH, HEIGHT, ITERATIONMAX, SAMPLES);
        }
        else
        {
            printf("Rendering %d frame(s) @ %d x %d images at %d iterations and no sample(s)\n", LENGTH, WIDTH, HEIGHT, ITERATIONMAX);
        }
    }
    void *iterationdata = malloc(HEIGHT * WIDTH * SAMPLES * sizeof(unsigned int));
    void *magnitutedata = malloc(HEIGHT * WIDTH * SAMPLES * sizeof(double));
    void *sampledata = malloc(HEIGHT * WIDTH * SAMPLES * sizeof(struct RGB));
    void *imagedata = malloc(HEIGHT * WIDTH * sizeof(struct RGB));
    void *processdata = malloc(HEIGHT * WIDTH * sizeof(struct RGB));
    if (iterationdata == NULL || magnitutedata == NULL || sampledata == NULL || imagedata == NULL || processdata == NULL)
    {
        fprintf(stderr, "Error: Out of memory\n");
        exit(-1);
    }
    for (int i = 0; i < LENGTH; i++)
    {
        iterate(iterationdata, magnitutedata, i);
        render(iterationdata, magnitutedata, sampledata, i);
        average(sampledata, imagedata);
        if (BLUR == 1) 
        {
            blur(imagedata, processdata);
        }
        writefile(imagedata, i);
    }
    printf("\n");
    return 0;
}
