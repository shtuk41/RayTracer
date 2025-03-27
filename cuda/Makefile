CUDA_PATH=/usr/local/cuda/bin
HOST_COMPILER=g++ -std=c++17
NVCC=$(CUDA_PATH)/nvcc $(NVCCFLAGS) -ccbin $(HOST_COMPILER)
GENCODE_FLAGS=-arch=compute_61 -code=sm_61
CFLAGS=-g -G
OPENCV_INCLUDE=/usr/include/opencv4
CUDA_INCLUDE=/usr/local/cuda/include
CUDA_LIB=/usr/local/cuda/lib64
OPENCV_LIB=/usr/lib/x86_64-linux-gnu

all:
	$(NVCC) $(GENCODE_FLAGS) -I$(OPENCV_INCLUDE) -I$(CUDA_INCLUDE) -L$(CUDA_LIB) \
	 -lcudart -lopencv_core -lopencv_highgui -lopencv_imgcodecs \
	 -lopencv_imgproc -o raytracer main.cu


debug:
	$(NVCC) $(GENCODE_FLAGS) $(CFLAGS) -I$(OPENCV_INCLUDE) -I$(CUDA_INCLUDE) -L$(CUDA_LIB) \
	 -lcudart -lopencv_core -lopencv_highgui -lopencv_imgcodecs \
	 -lopencv_imgproc -o raytracer main.cu

clean:
	rm raytracer imageFile.ppm
