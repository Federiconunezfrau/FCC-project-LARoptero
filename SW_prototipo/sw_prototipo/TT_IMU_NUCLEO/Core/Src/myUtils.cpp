/*
 * myUtils.cpp
 *
 *  Created on: Feb 15, 2024
 *      Author: fede
 */

#include "myUtils.h"

void my_copy_array_of_uint8(uint8_t *dest, const uint8_t *src, uint32_t n)
{
	uint32_t i;

	for(i = 0; i < n; i++)
	{
		dest[i] = src[i];
	}
}


