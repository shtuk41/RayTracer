#pragma once

#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#include <CL/cl.h>

//message.cpp
void message(char *message);
#define message_info(msg) std::cout << msg << " file: " << __FILE__ << " ,line: " << __LINE__ << std::endl;
void message_pause(char *message);
void message_pause(char *message, char *fileName, int lineNumber);

//error.cpp
void error_translate(cl_int error);

//platfrom.cpp
void	PrintPlatformIds();

//context.cpp
cl_context CreateContextCPU();

//command_queue.cpp
cl_command_queue CreateCommandQueue(cl_context,cl_device_id *);

//crete_program.cpp
cl_program CreateProgram(cl_context context, cl_device_id device, const char *fileName);

//create_memory_objects.cpp
bool CreateMemObjects(cl_context context, cl_mem memObjects[2], int rowSize, unsigned int ARRAY_SIZE);