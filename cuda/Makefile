
CUDA_PATH=/usr/local/cuda-9.1/bin
HOST_COMPILER=g++ -std=c++11
NVCC=$(CUDA_PATH)/nvcc -ccbin $(HOST_COMPILER)
GENCODE_FLAGS  = -gencode arch=compute_60,code=sm_60
CFLAGS=-g -G
OPENCV_INCLUDE=/usr/local/include
CUDA_INCLUDE=/usr/local/cuda-9.1/include
CUDA_LIB=/usr/local/cuda-9.1/lib64
OPENCV_LIB=/usr/local/lib

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