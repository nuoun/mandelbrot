#!/bin/bash
make clean && make
rm -f ./output/*.ppm
SECONDS=0
date +"%T"
./mandelbrot
duration=$SECONDS
printf "\n$(($duration / 3600)) hours $(($duration % 3600 / 60)) minutes and $(($duration % 60)) seconds elapsed.\n"
ffmpeg -y -r 30 -i "output/img%d.ppm" -pix_fmt yuv420p mandelbrot.mp4