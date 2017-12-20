#include "stdafx.h"
#include <iostream>

#include "openclfunctions.h"

void	PrintPlatformIds()
{
	cl_int	errNum;
	cl_uint numPlatforms;
	cl_platform_id *platformIds;
	cl_context context = NULL;

	errNum = clGetPlatformIDs(0,NULL,&numPlatforms);

	if (errNum != CL_SUCCESS)
	{
		message_info("Failed to find OpenCL platforms");
		return;
	}

	platformIds = (cl_platform_id *)alloca(sizeof(cl_platform_id) * numPlatforms);

	errNum = clGetPlatformIDs(numPlatforms, platformIds, NULL);

	for (cl_uint id = 0; id < numPlatforms; id++)
	{
		//platform name
		size_t size;
		errNum = clGetPlatformInfo(platformIds[id],	CL_PLATFORM_NAME, 0, NULL, &size);

		if (errNum != CL_SUCCESS)
		{
			message_info("Unable to get size of CL_PLATFORM_NAME in clGetPlatformInfo");

		}
		else
		{
			char *name = (char *)alloca(sizeof(char) * size);
			errNum = clGetPlatformInfo(platformIds[id],CL_PLATFORM_NAME,size,name,NULL);
		
			if (errNum != CL_SUCCESS)
			{
				message("Unable to get CL_PLATFORM_NAME from clGetPlatformInfo");
				return;
			}
			else
			{
				std::cout << "count: " << id << std::endl;
				std::cout << "Platform ID: " << platformIds[id] << std::endl;
				std::cout << "Platform Name: " << name << std::endl;
			}
		}

		//platform vendor

		errNum = clGetPlatformInfo(platformIds[id],	CL_PLATFORM_VENDOR, 0, NULL, &size);

		if (errNum != CL_SUCCESS)
		{
			message_info("unable to get a size of CL_PLATFORM_VENDOR / clGetPlatformInfo");
		}
		else
		{
			char *vendor = (char *)alloca(sizeof(char) * size);
			errNum = clGetPlatformInfo(platformIds[id],CL_PLATFORM_VENDOR,size,vendor,NULL);

			if (errNum != CL_SUCCESS)
			{
				message_info("unable to get CL_PLATFORM_VENDOR / clGetPlatformInfo");
			}
			else
			{
				std::cout << "Vendor: " << vendor << std::endl;
			}
		}

		//platform profiel

		errNum = clGetPlatformInfo(platformIds[id],	CL_PLATFORM_PROFILE, 0, NULL, &size);

		if (errNum != CL_SUCCESS)
		{
			message_info("unable to get a size of CL_PLATFORM_PROFILE / clGetPlatformInfo");
		}
		else
		{
			char *profile = (char *)alloca(sizeof(char) * size);
			errNum = clGetPlatformInfo(platformIds[id],CL_PLATFORM_VENDOR,size,profile,NULL);

			if (errNum != CL_SUCCESS)
			{
				message_info("unable to get CL_PLATFORM_PROFILE / clGetPlatformInfo");
			}
			else
			{
				std::cout << "Platform Profile: " << profile << std::endl;
			}
		}

		//platform version

		errNum = clGetPlatformInfo(platformIds[id],	CL_PLATFORM_VERSION, 0, NULL, &size);

		if (errNum != CL_SUCCESS)
		{
			message_info("unable to get a size of CL_PLATFORM_VERSION / clGetPlatformInfo");
		}
		else
		{
			char *version = (char *)alloca(sizeof(char) * size);
			errNum = clGetPlatformInfo(platformIds[id],CL_PLATFORM_VERSION,size,version,NULL);

			if (errNum != CL_SUCCESS)
			{
				message_info("unable to get CL_PLATFORM_VERSION / clGetPlatformInfo");
			}
			else
			{
				std::cout << "Platform Version: " << version << std::endl;
			}
		}

		//platform extension

		errNum = clGetPlatformInfo(platformIds[id],	CL_PLATFORM_EXTENSIONS, 0, NULL, &size);

		if (errNum != CL_SUCCESS)
		{
			message_info("unable to get a size of CL_PLATFORM_EXTENSIONS / clGetPlatformInfo");
		}
		else
		{
			char *extensions = (char *)alloca(sizeof(char) * size);
			errNum = clGetPlatformInfo(platformIds[id],CL_PLATFORM_EXTENSIONS,size,extensions,NULL);

			if (errNum != CL_SUCCESS)
			{
				message_info("unable to get CL_PLATFORM_EXTENSIONS / clGetPlatformInfo");
			}
			else
			{
				std::cout << "Platform extensions: " << extensions << std::endl;
			}
		}

	}
}