// RayTracerParallel.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <sstream>

#include "openclfunctions.h"

const unsigned int nx =	640;
const unsigned int ny = 480;

const unsigned int ARRAY_SIZE = nx * ny;


int _tmain(int argc, _TCHAR* argv[])
{
	cl_context context = 0;
	cl_command_queue commandQueue = 0;
	cl_program program = 0;
	cl_device_id device = 0;
	cl_kernel kernel = 0;
	cl_mem memObjects[2] = {0,0};
	cl_int errNum;

	context = CreateContextCPU();

	if (!context)
	{
		message_pause("Unable to create context");
		return 0;
	}


	commandQueue = CreateCommandQueue(context, &device);

	if (!commandQueue)
	{
		message_pause("Unable to create command queue");
		return 0;
	}

	program = CreateProgram(context, device, "color.cl");

	if (program == NULL)
	{
		message_pause("failed to create program");
		return 0;
	}

	kernel = clCreateKernel(program, "color_kernel", NULL);

	if (kernel == NULL)
	{
		message_pause("Failed to create kernel");
		return 0;
	}

	cl_float3	*result = new cl_float3[ARRAY_SIZE];
	cl_int		rowSize = nx;

	if (!CreateMemObjects(context, memObjects, rowSize, ARRAY_SIZE))
	{
		message_pause("Unable to create memory objects");
		return 0;
	}

	errNum = clSetKernelArg(kernel, 0, sizeof(cl_mem), &memObjects[0]);
	errNum |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &memObjects[1]);

	if (errNum != CL_SUCCESS)
	{
		message_pause("Error setting kernel arguments.");
		return 0;
	}

	size_t globalWorkSize[1] = {ARRAY_SIZE};
	size_t localWorkSize[1] = {1};

	errNum = clEnqueueNDRangeKernel(commandQueue,kernel,1,NULL,globalWorkSize,localWorkSize,0,NULL,NULL);

	if (errNum != CL_SUCCESS)
	{
		message_pause("Error queueing kernel for execution.");
		return 0;
	}

	errNum = clEnqueueReadBuffer(commandQueue,memObjects[1],CL_TRUE,0,sizeof(cl_float3) * ARRAY_SIZE,result,0,NULL,NULL);

	std::ofstream frame;
	frame.open("frame_b1_chapter1_640_480.ppm", std::ios::trunc);

	if (frame.is_open())
	{
		frame << "P3\n" << nx << " " << ny << "\n255\n";

		cl_float3* f = result;

		for (int ii = 0; ii < ARRAY_SIZE; ii++, f++)
		{
			int ir = int(255.99 * (*f).x);
			int ig = int(255.99 * (*f).y);
			int ib = int(255.99 * (*f).z);

			frame << ir << " " << ig << " " << ib << "\n";

			//std::cout << ii << " x: " << result[ii].x << std::endl;
			//std::cout << ii << " y: " << result[ii].y << std::endl;
			//std::cout << ii << " z: " << result[ii].z << std::endl;
		}
	}

	delete [] result;

	std::cout << "Success" << std::endl;


	system("pause");

	return 0;
}

