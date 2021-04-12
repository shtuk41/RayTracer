CPPFILES=../../
CFLAGS = -g -Wall -std=c++11
INCLUDES=../../
CC = g++

all: RayTracer

RayTracer:	random perlin hitable_list material hitable
	$(CC) $(CFLAGS) $(CPPFILES)RayTracer.cpp random.o perlin.o hitable_list.o material.o hitable.o -o RayTracer
	
random:
	$(CC) $(CFLAGS) $(CPPFILES)random.cpp -c -o random.o
	
perlin:
	$(CC) $(CFLAGS) $(CPPFILES)perlin.cpp -c -o perlin.o
	
hitable_list:
	$(CC) $(CFLAGS) $(CPPFILES)hitable_list.cpp -c -o hitable_list.o
	
material:
	$(CC) $(CFLAGS) $(CPPFILES)material.cpp -c -o material.o
	
hitable:
	$(CC) $(CFLAGS) $(CPPFILES)hitable.cpp -c -o hitable.o 

clean:
	rm main