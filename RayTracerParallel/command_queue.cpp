#include "stdafx.h"

#include <iostream>

#include "openclfunctions.h"

cl_command_queue CreateCommandQueue(cl_context context, cl_device_id *device)
{
	cl_int errNum;
	cl_device_id *devices;
	cl_command_queue queue = NULL;
	size_t deviceBufferSize = -1;

	errNum = clGetContextInfo(context,CL_CONTEXT_DEVICES,0,NULL,&deviceBufferSize);

	if (errNum != CL_SUCCESS)
	{
		error_translate(errNum);
		message_info("Unable to get context info CL_CONTEXT_DEVICES");

		return queue;
	}

	if (deviceBufferSize > 0)
	{
		devices = new cl_device_id[deviceBufferSize / sizeof(cl_device_id)];
		
		errNum = clGetContextInfo(context,CL_CONTEXT_DEVICES,deviceBufferSize,devices,NULL);

		if (errNum != CL_SUCCESS)
		{
			error_translate(errNum);
			delete [] devices;
			return queue;
		}

		queue = clCreateCommandQueue(context,devices[0],0,&errNum);

		if (errNum != CL_SUCCESS)
		{
			delete [] devices;
			error_translate(errNum);
			return queue;
		}
	}


	*device = devices[0];
	delete [] devices;
	return queue;
}
