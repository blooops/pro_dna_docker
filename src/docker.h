/*
 * 	Author : Rohit Roy
 */
#pragma once
#include "structure.h"

// Non parallel function definitions:

void generate_surface(SpaceMatrix* space, int thickness, int core_value = 1);

// Parallel function definitions:

void p_generate_surface(SpaceMatrix* space, int thickness);
