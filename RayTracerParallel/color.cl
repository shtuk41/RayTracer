__kernel void color_kernel(__global int *rowsize, __global float3 *result)
{
	int gid = get_global_id(0);

	int x = gid % *rowsize;
	int y = 480 - gid / *rowsize;
	result[gid].x = (float)x / 640 ;
	result[gid].y = (float)y / 480 ;
	result[gid].z = 0.2f;

}