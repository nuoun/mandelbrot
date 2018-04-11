.POSIX:
CC = cc -std=c99
CFLAGS = -Wall -Wextra -Ofast -march=native
LDLIBS = -lm
OBJS = main.c function.c
OBJ_NAME = mandelbrot

all: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) $(CFLAGS) -o $(OBJ_NAME) $(LDLIBS)

clean:
	rm -f $(OBJ_NAME)