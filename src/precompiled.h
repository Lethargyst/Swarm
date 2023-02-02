#ifndef PRECOMPILED_H
#define PRECOMPILED_H

#include "Math/Vectors.h"
#include "Math/Matrices.h"

const float epsilon = 0.01;

const unsigned int RENDER_BUFFER_SIZE = 500000;
const unsigned int QUAD_TREE_BUFFER_SIZE = 10000;
const unsigned int SHOUT_LINES_BUFFER_SIZE = 10000000;

const float MAX_FPS = 60.0;
const float MAX_PERIOD = 1.0 / MAX_FPS;

const unsigned int MOVEMENT_SPREAD = 5;
const float SPEED = 0.003f;
const float SOURCE_SIZE = 0.07f;
const float ANT_SIZE = 0.005f;
const float SHOUT_RANGE = 0.02f;
const unsigned int TIME_TO_SHOUT = 1;

const unsigned int MAX_OBJECTS_PER_NODE = 100;
const unsigned int MAX_DEPTH = 6;


#endif

