#include "stdafx.h"
#include "random.h"

float get_random()
{
	return (float)rand() / RAND_MAX;
}