#pragma once

#include "../picture.h"
#include "triangle.h"

class Backend
{
public:
    virtual ~Backend() {}
    virtual Picture render(const Mesh& triangles) = 0;
};
