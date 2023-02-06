#include "Algebra.hpp"

float rad2deg(float x) { return (x) * 57.295754f; }
float deg2rad(float x) { return (x) * 0.0174533f; }

float clamp(float num, float min, float max)
{
    return (num < min) ? min : (num > max) ? max : num;
}
