CC = g++
CFLAGS = -Wall -Wextra -pedantic -std=c++11

SRCS = raytracer.cc math.cc math.tcc geometry.cc geometry.tcc
OBJS = $(SRCS:.cc=.o)

all: raytracer

raytracer: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.cc
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) raytracer
