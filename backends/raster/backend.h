#pragma once

#include "../backend.h"
#include "vec4.h"

class RasterBackend : public Backend
{
public:
    RasterBackend(unsigned size);
    ~RasterBackend();

    Picture render(const Mesh& triangles);

protected:
private:
    unsigned m_size        = 0;
    Vec3 m_modelColor      = { 1.0f, 0.925f, 0.208f };
    Vec3 m_ambientColor    = { 0.3f, 0.3f, 0.3f };
    Vec3 m_diffuseColor    = { 0.5f, 0.5f, 0.5f };
    Vec3 m_specColor       = { 1.0f, 1.0f, 1.0f };
    Vec4 m_backgroundColor = { 1.0f, 1.0f, 1.0f, 0.0f };
    Vec3 m_lightPos        = { 4, -2, 2 };
};
