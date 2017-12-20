#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "openclfunctions.h"

cl_program CreateProgram(cl_context context, cl_device_id device, const char *fileName)
{
	cl_int errNum;

	cl_program	program;

	std::ifstream kernelFile(fileName,std::ios::in);

	if (!kernelFile.is_open())
	{
		std::cout << "Filed to open file for reading: " << fileName << std::endl;
		return NULL;
	}

	std::ostringstream oss;
	oss << kernelFile.rdbuf();

	std::string srcStdStr = oss.str();

	const char *srcStr = srcStdStr.c_str();

	program = clCreateProgramWithSource(context, 1, (const char**)&srcStr, NULL, NULL);

	if (program == NULL)
	{
		std::cout << "Failed to create CL Program from source. " << std::endl;
		return NULL;
	}

	errNum = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);

	if (errNum != CL_SUCCESS)
	{
		char buildLog[16384];

		clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, sizeof(buildLog), buildLog, NULL);

		std::cout << "Error in kernel: " << std::endl;
		std::cout << buildLog << std::endl;
		clReleaseProgram(program);
		return NULL;
	}

	return program;

}

