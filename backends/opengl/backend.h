/*
Copyright (C) 2017  Paul Kremer

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "../backend.h"

class OpenGLBackend : public Backend
{
public:
    OpenGLBackend(unsigned size);
    ~OpenGLBackend();

    Picture render(const Mesh& triangles);

protected:
    unsigned loadProgram(const char* vert, const char* frag);

private:
    void* m_eglDisplay = nullptr;
    void* m_eglSurface = nullptr;
    unsigned m_size    = 0;
    unsigned m_program = 0;

    Vec3 m_modelColor      = { 0.2f, 0.2f, 1.0f };
    Vec3 m_ambientColor    = { 0.3f, 0.3f, 0.3f };
    Vec3 m_diffuseColor    = { 0.4f, 0.4f, 0.4f };
    Vec3 m_specColor       = { 1.0f, 1.0f, 1.0f };
    Vec3 m_backgroundColor = { 0.8f, 0.8f, 0.8f };
};
