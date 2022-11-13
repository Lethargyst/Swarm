#ifndef PRECOMPILED_H
#define PRECOMPILED_H

#include "Math/Vectors.h"
#include "Math/Matrices.h"

const unsigned int RENDER_BUFFER_SIZE = 500000;
const unsigned int QUAD_TREE_BUFFER_SIZE = 10000;
const unsigned int SHOUT_LINES_BUFFER_SIZE = 10000;

const float MAX_FPS = 60.0;
const float MAX_PERIOD = 1.0 / MAX_FPS;

const unsigned int MOVEMENT_SPREAD = 3;
const float SOURCE_SIZE = 0.07f;
const float ANT_SIZE = 0.005f;

const unsigned int MAX_OBJECTS_PER_NODE = 200;
const unsigned int MAX_DEPTH = 6;


#endif

