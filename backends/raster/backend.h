#pragma once

#include "../backend.h"

class RasterBackend : public Backend
{
public:
    RasterBackend(unsigned size);
    ~RasterBackend();

    Picture render(const Mesh& triangles);

protected:
private:
    unsigned m_size        = 0;
    Vec3 m_diffuseDir      = { 1.0f, 1.0f, -1.0f };
    Vec3 m_modelColor      = { 0.2f, 0.2f, 1.0f };
    Vec3 m_ambientColor    = { 0.3f, 0.3f, 0.3f };
    Vec3 m_diffuseColor    = { 0.4f, 0.4f, 0.4f };
    Vec3 m_specColor       = { 1.0f, 1.0f, 1.0f };
    Vec3 m_backgroundColor = { 0.8f, 0.8f, 0.8f };
};
