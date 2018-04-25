#include "util.h"
#include "color.h"

long double zoom = 1.0;
u_int8_t imagedata[HEIGHT][WIDTH][3];

void renderimage(int framenumber)
{
    int i;
    long double cx, cy, ox, oy, zx, zy, zx2, zy2;
    long double xmin = -2.25 * zoom;
    long double xmax = 2.25 * zoom;
    long double ymin = -2.25 * zoom;
    long double ymax = 2.25 * zoom;
    long double dx = (xmax - xmin) / WIDTH;
    long double dy = (ymax - ymin) / HEIGHT;
    long double escape = 4.0;
    long double zoomrate = 0.9;
    long double time = (long double)framenumber;
    float iteration;

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

    long double centerx = -0.5;
    long double centery = 0.0;

*/

    long double centerx = 0.37226792743827275;
    long double centery = -0.6687312047576894;

    long double cost = cos(0.0075 * time);
    long double sint = sin(0.0075 * time);

    for (int y = 0; y < HEIGHT; y++)
    {
        oy = ymax - y * dy;
        for (int x = 0; x < WIDTH; x++)
        {
            ox = xmin + x * dx;
            cx = ox * cost - oy * sint + centerx;
            cy = ox * sint + oy * cost + centery;

            zx = 0.0;
            zy = 0.0;
            zx2 = zx * zx;
            zy2 = zy * zy;

            iteration = 0.0;

            for (i = 0; i < ITERATIONMAX && ((zx2 + zy2) < escape); i++)
            {
                zy = 2 * zx * zy + cy;
                zx = zx2 - zy2 + cx;
                zx2 = zx * zx;
                zy2 = zy * zy;
                iteration++;
            }
            if (i == ITERATIONMAX)
            {
                imagedata[y][x][0] = 0;
                imagedata[y][x][1] = 0;
                imagedata[y][x][2] = 0;
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

                double modulus = sqrt(zx2 + zy2);
                double mu = iteration - (log(log(modulus))) / log(2.0);
                mu = mu / (double)ITERATIONMAX * 360.0;
                double output = fmod(mu + framenumber, 360.0);

                double hsv[3] = {output, 0.75, 1};
                u_int8_t rgb[3];
                hsvtorgb(hsv, rgb);

                imagedata[y][x][0] = rgb[0];
                imagedata[y][x][1] = rgb[1];
                imagedata[y][x][2] = rgb[2];

            }
        }
    }
    zoom *= zoomrate;
}

int main(void)
{
    for (int i = 0; i < LENGTH; i++)
    {
        renderimage(i);
        writefile(i);
    }
    printf("\n");
    return 0;
}
