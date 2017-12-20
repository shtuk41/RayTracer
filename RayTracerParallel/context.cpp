#include <stdafx.h>
#include <iostream>
#include "openclfunctions.h"

cl_context CreateContextCPU()
{
	cl_int errNum;
	cl_uint numPlatform;
	cl_context context = NULL;
	cl_platform_id* platformIds;

	errNum = clGetPlatformIDs(0, NULL, &numPlatform);

	if (errNum != CL_SUCCESS || numPlatform < 1)
	{
		message("Unable to query for number of platforms with clGetPlatformIDs");
		return 0;
	}

	platformIds = (cl_platform_id *)alloca(sizeof(cl_platform_id) * numPlatform);

	errNum = clGetPlatformIDs(numPlatform, platformIds, NULL);

	for (cl_uint id = 0; id < numPlatform; id++)
	{

		cl_uint numDevices;
		errNum = clGetDeviceIDs(platformIds[id],CL_DEVICE_TYPE_GPU,0,NULL,&numDevices);

		if (errNum != CL_SUCCESS || numDevices < 1)
			continue;

		cl_device_id* devices = (cl_device_id*)alloca(numDevices * sizeof(cl_device_id));
		
		errNum = clGetDeviceIDs(platformIds[id],CL_DEVICE_TYPE_GPU,numDevices,devices,NULL);

		if (errNum != CL_SUCCESS)
		{
			message("unable to get device ids");
			continue;
		}

		cl_context_properties properties [] = {CL_CONTEXT_PLATFORM, (cl_context_properties)platformIds[id],0};

		context = clCreateContext(properties,numDevices, devices,NULL,NULL,&errNum);

		if (errNum != CL_SUCCESS)
		{
			message_info("Unable to create context");
			error_translate(errNum);
			continue;
		}

		size_t param_value_size_ret;
		errNum =  clGetContextInfo(context,CL_CONTEXT_NUM_DEVICES,0,NULL,&param_value_size_ret);

		if (errNum != CL_SUCCESS)
		{
			message("Unable to get context info, clGetContextInfo");
			continue;
		}

		cl_uint number_of_devices;

		errNum =  clGetContextInfo(context,CL_CONTEXT_NUM_DEVICES,param_value_size_ret,&number_of_devices,NULL);

		if (errNum != CL_SUCCESS)
		{
			error_translate(errNum);
			continue;
		}
		else
		{
			std::cout << "Number of devices in context: " << number_of_devices << std::endl;
			system("pause");
		}

		return context;

	}

	return context;
}

