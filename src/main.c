#include "function.h"

int main(void)
{
    imagedata = malloc(WIDTH * HEIGHT * 3);
    for (int i = 0; i < LENGTH; i++)
    {
        renderframe(i);
        writefile(i);
    }
}