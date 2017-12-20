#include "stdafx.h"
#include <iostream>
#include "openclfunctions.h"

void error_translate(cl_int error)
{
	std::cout << "	error code: " << error << std::endl;

	switch (error)
	{
		case CL_DEVICE_NOT_FOUND:
			message("	CL_DEVICE_NOT_FOUND:	No OpenCL devices fond matching criteria");
			break;
		case CL_DEVICE_NOT_AVAILABLE:
			message("	CL_DEVICE_NOT_AVAILABLE:	OpenCL device is not currently available");
			break;
		default:
			std::cout << "	error not found" << std::endl;
	}
}