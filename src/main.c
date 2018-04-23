#include "function.h"

int main(void)
{
    image = malloc(WIDTH * HEIGHT * 3);
    for (int i = 0; i < LENGTH; i++)
    {
        draw_frame(i);
        output_frame();
    }
}