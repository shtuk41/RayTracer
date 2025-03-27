CC=g++ -std=c++17
CFLAGS=-g
OPENCV_INCLUDE=/usr/include/opencv4
OPENCV_LIB=/usr/lib/x86_64-linux-gnu

all:    cornell_box final perlin_noise RayTracer

cornell_box:
	$(CC) -o $@ $@.cpp random.cpp perlin.cpp hitable.cpp \
	material.cpp texture.cpp hitable_list.cpp
	
final:
	$(CC) -o final final.cpp random.cpp perlin.cpp hitable.cpp \
	material.cpp texture.cpp hitable_list.cpp
	
perlin_noise:
	$(CC) -o $@ PerlinNoiseRayTracer.cpp random.cpp perlin.cpp hitable.cpp \
	material.cpp texture.cpp hitable_list.cpp
	
RayTracer:
	$(CC) -o $@ RayTracer.cpp random.cpp perlin.cpp hitable.cpp \
	material.cpp texture.cpp hitable_list.cpp			

debug_cornell_box:
	$(CC) $(CFLAGS) -o $@ cornell_box.cpp random.cpp perlin.cpp hitable.cpp \
	material.cpp texture.cpp hitable_list.cpp

clean:
	@rm -rf cornell_box final perlin_noise RayTracer
