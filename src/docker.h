#pragma once

#include "structure.h"

// Non parallel function definitions:

void generateSurface(SpaceMatrix* space, float thickness);

// Parallel function definitions:

void p_generateSurface(SpaceMatrix* space, float thickness);
