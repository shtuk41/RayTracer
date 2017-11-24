#pragma once

#include <random>

float get_random()
{
	return (float)rand() / RAND_MAX;
}