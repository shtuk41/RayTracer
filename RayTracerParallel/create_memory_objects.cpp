#include "stdafx.h"
#include <iostream>
#include "openclfunctions.h"

bool CreateMemObjects(cl_context context, cl_mem memObjects[2], cl_int rowSize, unsigned int ARRAY_SIZE)
{
	memObjects[0] = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_int), &rowSize, NULL);

	memObjects[1] = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(cl_float3) * ARRAY_SIZE,NULL, NULL);

	if (memObjects[0] == NULL || memObjects[1] == NULL)
	{
		std::cout << "Error creating memory objects." << std::endl;
		return false;
	}
}