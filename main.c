#include "main.h"
 
#define W 800
#define H 800
#define M 255
#define ITERATIONMAX 1024
#define ESCAPE 4
 
uint8_t r[H][W], g[H][W], b[H][W];
long double zoom = 1;

void
draw_frame(int framecount)
{
    int i;
    long double cx, cy, zx, zy, zx2, zy2;
    static unsigned char colour[3];
    long double xmin = -2.25 * zoom;
    long double xmax = 2.25 * zoom;
    long double ymin = -2.25 * zoom;
    long double ymax = 2.25 * zoom;
    long double dx = (xmax - xmin) / W;
    long double dy = (ymax - ymin) / H;

    zoom *= 0.975;

    //long double centerx = 0;
    //long double centery = 0;
    //long double centerx = -1.76651223650935682;
    //long double centery = 0.04171432271082462;
    //long double centerx = -0.7473354651820724;
    //long double centery = -0.10030992828748005;
    //long double centerx = -1.256148879595297;
    //long double centery = 0.3994106258544796;
    //long double centerx = -1.76651223950935682;
    //long double centery = 0.04171432271082462;
    long double centerx = -1.77810334274064037110522326038852639499207961414628307584575173232969154440;
    long double centery = 0.00767394242121339392672671947893471774958985018535019684946671264012302378;
    //long double centerx = 0;
    //long double centery = 0;



    for (int y=0; y<H; y++)
    {
        cy = centery + ymax - y * dy;
        for (int x=0; x<W; x++)
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
                colour[0] = 0;
                colour[1] = 0;
                colour[2] = 0;
            }
            else 
            {
                double z = sqrt(zx * zx + zy * zy);
                int brightness = 256. * log2(1.75 + i - log2(log2(z))) / log2((double)ITERATIONMAX);
                
                
                int d = 256 * sqrt (i / z) * log(z);
                //float d = 0.5*sqrt(dot(z,z)/dot(dz,dz))*log(dot(z,z));
                
                //d = clamp( pow(4.0*d/zoo,0.2), 0.0, 1.0 );
                d = 255 * pow(d,0.2);
                
                
                colour[0] = d; //brightness % 255; //(brightness + framecount) % 255;
                colour[1] = 0; //i % 255;
                colour[2] = 0; //255 + framecount % 255;
            }

            r[y][x] = colour[0];
            g[y][x] = colour[1];
            b[y][x] = colour[2];

        }
    }
}

void
output_frame(void)
{
    fprintf (stdout, "P6 %d %d %d\n", W, H, 255);
    for (int y=0; y<H; y++)
    {
        for (int x=0; x<W; x++)
        {
            fwrite (&r[y][x], sizeof(uint8_t), 1, stdout);
            fwrite (&g[y][x], sizeof(uint8_t), 1, stdout);
            fwrite (&b[y][x], sizeof(uint8_t), 1, stdout);
        }
    }
    fflush(stdout);
}
 
int
main()
{
    for (int i=0; i<15; i++)
    {
        draw_frame(i);
        output_frame();
    }
}