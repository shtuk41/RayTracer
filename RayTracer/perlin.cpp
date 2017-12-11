#include "stdafx.h"
#include "perlin.h"

void permute(int *p, int n)
{
	for (int i = n-1; i > 0; i--)
	{
		int target = int(get_random() * (i + 1));
		int tmp = p[i];
		p[i] = p[target];
		p[target] = tmp;
	}
	return;
}

vec3 *perlin::ranvec = perlin_generate();
int *perlin::perm_x = perlin_generate_perm();
int *perlin::perm_y = perlin_generate_perm();
int *perlin::perm_z = perlin_generate_perm();