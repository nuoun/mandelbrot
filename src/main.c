#include "function.h"

char* image;

int main()
{
    image = malloc(WIDTH * HEIGHT * 3);
    for (int i = 0; i < LENGTH; i++)
    {
        draw_frame(i);
        output_frame();
    }
}