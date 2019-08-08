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

#include "rasterbackend.h"
#include "aabb.h"
#include "zbuffer.h"

#include <array>
#include <glm/gtc/matrix_transform.hpp>
#include <limits>

// helpers
glm::vec3 vec3ToGlm(const Vec3& v)
{
    return { v.x, v.y, v.z };
}

float edgeFunction(glm::vec2 p, glm::vec2 a, glm::vec2 b)
{
    return (p.x - a.x) * (b.y - a.y) - (p.y - a.y) * (b.x - a.x);
}

glm::vec3 glmMat4x4MulVec3(const glm::mat4x4& mat, glm::vec3 v)
{
    return glm::vec3(mat * glm::vec4 { v.x, v.y, v.z, 1.0f });
}

//
RasterBackend::RasterBackend()
{
}

RasterBackend::~RasterBackend()
{
}

Picture RasterBackend::render(unsigned imgWidth, unsigned imgHeight, const Mesh& triangles)
{
    ZBuffer m_zbuffer(imgWidth, imgHeight);
    Picture pic(imgWidth, imgHeight);
    pic.fill(m_backgroundColor.x, m_backgroundColor.y, m_backgroundColor.z, m_backgroundColor.w);

    // generate AABB
    auto aabb = AABBox(triangles);

    // create model view projection matrix (unscaled)
    const float zoom        = 1.0f;
    const float aspectRatio = imgWidth / float(imgHeight);
    auto projection         = glm::ortho(zoom * .5f * aspectRatio, -zoom * .5f * aspectRatio, -zoom * .5f, zoom * .5f, 0.0f, 1.0f);

    auto viewPos       = glm::vec3 { -1.f, -1.f, 1.f };
    auto view          = glm::lookAt(viewPos, glm::vec3 { 0.f, 0.f, 0.f }, { 0.f, 0.f, 1.f });
    auto model         = glm::mat4(1);
    auto modelViewProj = projection * view * model;

    // calculate the new model scale
    auto modelScaleInView = scaleInView(modelViewProj, aabb);

    // recalculate transforms taking the new model scale into account
    model         = glm::scale(glm::mat4(1), glm::vec3 { 1.0f / modelScaleInView }) * glm::translate(glm::mat4(1), -vec3ToGlm(aabb.center()));
    modelViewProj = projection * view * model;

    Vec3 eyeNormal = Vec3 { viewPos.x, viewPos.y, viewPos.z }.normalize();

    for (const auto& t : triangles)
    {
        // backface culling
        if (dot(eyeNormal, t.normal) > 0.0f)
            continue;

        // project vertices to screen coordinates
        auto v0 = glmMat4x4MulVec3(modelViewProj, vec3ToGlm(t.vertices[0]));
        auto v1 = glmMat4x4MulVec3(modelViewProj, vec3ToGlm(t.vertices[1]));
        auto v2 = glmMat4x4MulVec3(modelViewProj, vec3ToGlm(t.vertices[2]));

        // triangle bounding box
        float minX = std::min(v0.x, std::min(v1.x, v2.x));
        float minY = std::min(v0.y, std::min(v1.y, v2.y));
        float maxX = std::max(v0.x, std::max(v1.x, v2.x));
        float maxY = std::max(v0.y, std::max(v1.y, v2.y));

        // bounding box in screen space
        unsigned sminX = static_cast<unsigned>(std::max(0, static_cast<int>((minX + 1.0f) / 2.0f * imgWidth)));
        unsigned sminY = static_cast<unsigned>(std::max(0, static_cast<int>((minY + 1.0f) / 2.0f * imgHeight)));
        unsigned smaxX = static_cast<unsigned>(std::max(0, std::min(int(imgWidth), static_cast<int>((maxX + 1.0f) / 2.0f * imgWidth))));
        unsigned smaxY = static_cast<unsigned>(std::max(0, std::min(int(imgHeight), static_cast<int>((maxY + 1.0f) / 2.0f * imgHeight))));

        for (unsigned y = sminY; y < smaxY + 1; ++y)
        {
            for (unsigned x = sminX; x < smaxX + 1; ++x)
            {
                // normalize screen coords [-1,1]
                const float nx = 2.f * (x / static_cast<float>(imgWidth) - 0.5f);
                const float ny = 2.f * (y / static_cast<float>(imgHeight) - 0.5f);

                auto P  = glm::vec2 { nx, ny };
                auto V0 = glm::vec2(v0);
                auto V1 = glm::vec2(v1);
                auto V2 = glm::vec2(v2);

                bool inside = true;
                inside &= edgeFunction(P, V0, V1) <= 0.0f;
                inside &= edgeFunction(P, V1, V2) <= 0.0f;
                inside &= edgeFunction(P, V2, V0) <= 0.0f;

                if (inside)
                {
                    // calculate baricentric coords
                    float area = edgeFunction(V0, V1, V2);
                    float w0   = edgeFunction(V1, V2, P) / area;
                    float w1   = edgeFunction(V2, V0, P) / area;
                    float w2   = edgeFunction(V0, V1, P) / area;

                    // the z position at point p by interpolating the z position of all 3 vertices
                    float pz = w0 * v0.z + w1 * v1.z + w2 * v2.z;
                    float px = w0 * v0.x + w1 * v1.x + w2 * v2.x;
                    float py = w0 * v0.y + w1 * v1.y + w2 * v2.y;

                    if (m_zbuffer.testAndSet(x, y, pz))
                    {
                        // calculate lightning
                        // diffuse
                        Vec3 s2l       = (m_lightPos - Vec3 { px, py, pz }).normalize();
                        Vec3 diffColor = std::max(0.0f, dot(t.normal, s2l)) * m_diffuseColor;

                        // specular
                        Vec3 fragPos    = { px, py, pz };
                        Vec3 lightDir   = (m_lightPos - fragPos).normalize();
                        Vec3 viewDir    = (Vec3 { viewPos.x, viewPos.y, viewPos.z } - fragPos).normalize();
                        Vec3 reflectDir = reflect(-lightDir, t.normal);
                        Vec3 specColor  = std::pow(std::max(dot(viewDir, reflectDir), 0.0f), 4.0f) * m_specColor * 0.7f;

                        // merge
                        Vec3 color = (m_ambientColor + diffColor + specColor) * m_modelColor;

                        // output pixel color
                        pic.setRGB(x, y, color.x, color.y, color.z);
                    }
                }
            }
        }
    }

    return pic;
}

float RasterBackend::scaleInView(const glm::mat4& modelViewProj, const AABBox& aabb)
{
    // Project the 8 edges of the AABB in screen space
    std::array<glm::vec3, 8> edges;
    edges[0] = glmMat4x4MulVec3(modelViewProj, { aabb.lower.x, aabb.lower.y, aabb.lower.z });
    edges[1] = glmMat4x4MulVec3(modelViewProj, { aabb.upper.x, aabb.lower.y, aabb.lower.z });
    edges[2] = glmMat4x4MulVec3(modelViewProj, { aabb.lower.x, aabb.upper.y, aabb.lower.z });
    edges[3] = glmMat4x4MulVec3(modelViewProj, { aabb.upper.x, aabb.upper.y, aabb.lower.z });
    edges[4] = glmMat4x4MulVec3(modelViewProj, { aabb.lower.x, aabb.lower.y, aabb.upper.z });
    edges[5] = glmMat4x4MulVec3(modelViewProj, { aabb.upper.x, aabb.lower.y, aabb.upper.z });
    edges[6] = glmMat4x4MulVec3(modelViewProj, { aabb.lower.x, aabb.upper.y, aabb.upper.z });
    edges[7] = glmMat4x4MulVec3(modelViewProj, { aabb.upper.x, aabb.upper.y, aabb.upper.z });

    // Calculate a model scaling factor that fits the entire model into the view
    glm::vec3 emin = glm::vec3 { std::numeric_limits<float>::max() };
    glm::vec3 emax = glm::vec3 { std::numeric_limits<float>::min() };
    for (const auto e : edges)
    {
        emin.x = std::min(emin.x, e.x);
        emin.y = std::min(emin.y, e.y);

        emax.x = std::max(emax.x, e.x);
        emax.y = std::max(emax.y, e.y);
    }

    return std::max(std::abs(emax.x - emin.x), std::abs(emax.y - emin.y)) * 0.5f;
}
