#include "utils.h"

/**
 *
 * Utility Functions
 *
 * */ 

int clamp(int value, int min, int max) 
{
	if (value > max)
		return max;
	if (value < min)
		return min;
	return value;
}
