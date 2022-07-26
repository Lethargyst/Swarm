#ifndef AGENTS_H
#define AGENTS_H

#include "objects.h"

class Source : Object
{
public:
    Source(vec2 pos, vec3 color) : Object(pos, color) {}

private:
    static int amount_;
    int ID_;
};


class Ant : Object
{
public:
    Ant(vec2 pos, vec3 color) : Object(pos, color) {}
    Ant(Source* source);

private:
    static int amount_;
    float speed_, direction_;
};

#endif