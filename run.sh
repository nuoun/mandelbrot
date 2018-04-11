#!/bin/bash
make clean
make
./mandelbrot | ffmpeg -y -r 30 -f image2pipe -vcodec ppm -i /dev/stdin -pix_fmt yuv420p mandelbrot.mp4