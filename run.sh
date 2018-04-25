#!/bin/bash
make clean && make
rm -f ./output/*.png
date +"%T"
SECONDS=0
./mandelbrot
duration=$SECONDS
printf "$(($duration / 3600)) hours $(($duration % 3600 / 60)) minutes and $(($duration % 60)) seconds elapsed.\n"
ffmpeg -y -r 30 -i "output/img%d.png" -pix_fmt yuv420p mandelbrot.mp4