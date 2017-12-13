/*
 * 	Author : Rohit Roy
 */

#include "docker.h"
#define INDEX(x, y, z, size) ((x) + (y) * (size) + (z) * (size) * (size))

// Check the neighbours of an element to see if the element is a surface element
bool check_neighbours(int thickness, int* matrix, int size, int x, int y, int z,
		int core_value) {
	if (matrix[INDEX(x, y, z, size)] != 0) {
		int neighbourhood = 1;
		for (int p = -neighbourhood; p <= neighbourhood; p++) {
			for (int q = -neighbourhood; q <= neighbourhood; q++) {
				for (int r = -neighbourhood; r <= neighbourhood; r++) {
					if (matrix[INDEX(x + p, y + q, z + r, size)] != core_value)
						return true;
				}
			}
		}
		return false;
	}
	return false;
}

// Set the surface molecules to the value 1
void generate_surface(SpaceMatrix* space, int thickness, int core_value = 1) {
	// checking the inner elements
	for (int x = 1; x < space->size-1; x++) {
		for (int y = 1; y < space->size-1; y++) {
			for (int z = 1; z < space->size-1; z++) {
				int element = space->matrix[INDEX(x, y, z, space->size)];
				if (element != 0
						&& check_neighbours(thickness, space->matrix,
								space->size, x, y, z, core_value))
					space->matrix[INDEX(x, y, z, space->size)] = 1;
			}
		}
	}

	//checking the outer elements
}
