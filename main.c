#include "function.h"

int main()
{
  for (int i = 0; i < 500; i++)
  {
    draw_frame(i);
    output_frame();
  }
}
