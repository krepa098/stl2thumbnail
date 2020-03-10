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

namespace stl2thumb
{

// helpers
inline float edgeFunction(glm::vec2 p, glm::vec2 a, glm::vec2 b)
{
    return (p.x - a.x) * (b.y - a.y) - (p.y - a.y) * (b.x - a.x);
}

inline glm::vec3 matMul(const glm::mat4x4& mat, glm::vec3 v)
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

Picture RasterBackend::render(unsigned imgWidth, unsigned imgHeight, const Mesh& mesh)
{
    ZBuffer m_zbuffer(imgWidth, imgHeight);
    Picture pic(imgWidth, imgHeight);
    pic.fill({ backgroundColor.x, backgroundColor.y, backgroundColor.z, backgroundColor.w });

    // generate AABB
    auto aabb = AABBox(mesh);

    // create model view projection matrix (unscaled)
    const float zoom        = 1.0f;
    const float aspectRatio = imgWidth / float(imgHeight);
    auto projection         = glm::ortho(zoom * .5f * aspectRatio, -zoom * .5f * aspectRatio, -zoom * .5f, zoom * .5f, 0.0f, 1.0f);

    auto view          = glm::lookAt(viewPos, { 0.f, 0.f, 0.f }, { 0.f, 0.f, -1.f });
    auto model         = glm::mat4(1);
    auto modelViewProj = projection * view * model;
    auto viewProj      = projection * view;

    // calculate the new model scale
    auto modelScaleInView = scaleInView(modelViewProj, aabb);

    // recalculate transforms taking the new model scale into account
    model         = glm::scale(glm::mat4(1), Vec3 { 1.0f / modelScaleInView }) * glm::translate(glm::mat4(1), -aabb.center());
    modelViewProj = projection * view * model;

    Vec3 eyeNormal  = glm::normalize(-Vec3 { viewPos.x, viewPos.y, viewPos.z });
    Vec3 lightPosCS = matMul(viewProj, lightPos); // in camera space

    for (const auto& t : mesh)
    {
        // backface culling
        if (dot(eyeNormal, t.normal) > 0.0f)
            continue;

        // project vertices to camera space
        auto v0 = matMul(modelViewProj, t.vertices[0]);
        auto v1 = matMul(modelViewProj, t.vertices[1]);
        auto v2 = matMul(modelViewProj, t.vertices[2]);
        auto n  = glm::normalize(matMul(viewProj, t.normal));

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

        for (unsigned y = sminY; y <= smaxY; ++y)
        {
            for (unsigned x = sminX; x <= smaxX; ++x)
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

                    // fragment position in screen space
                    Vec3 fragPos = {
                        (w0 * v0.x + w1 * v1.x + w2 * v2.x),
                        (w0 * v0.y + w1 * v1.y + w2 * v2.y),
                        (w0 * v0.z + w1 * v1.z + w2 * v2.z)
                    };

                    if (m_zbuffer.testAndSet(x, y, fragPos.z))
                    {
                        // calculate lightning
                        // diffuse
                        Vec3 lightDir  = glm::normalize(fragPos - lightPosCS);
                        Vec3 diffColor = std::max(0.0f, dot(t.normal, lightDir)) * diffuseColor;

                        // specular
                        Vec3 viewDir    = glm::normalize(fragPos);
                        Vec3 reflectDir = glm::reflect(lightDir, n);
                        Vec3 specColor  = glm::pow(std::max(dot(viewDir, reflectDir), 0.0f), 3.0f) * specularColor;

                        // merge
                        Vec3 color = (ambientColor + diffColor + specColor) * modelColor;

                        // output pixel color
                        pic.setPixel(x, y, { color.r, color.g, color.b, 1.0f });
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
    std::array<Vec3, 8> edges = {
        matMul(modelViewProj, { aabb.lower.x, aabb.lower.y, aabb.lower.z }),
        matMul(modelViewProj, { aabb.upper.x, aabb.lower.y, aabb.lower.z }),
        matMul(modelViewProj, { aabb.lower.x, aabb.upper.y, aabb.lower.z }),
        matMul(modelViewProj, { aabb.upper.x, aabb.upper.y, aabb.lower.z }),
        matMul(modelViewProj, { aabb.lower.x, aabb.lower.y, aabb.upper.z }),
        matMul(modelViewProj, { aabb.upper.x, aabb.lower.y, aabb.upper.z }),
        matMul(modelViewProj, { aabb.lower.x, aabb.upper.y, aabb.upper.z }),
        matMul(modelViewProj, { aabb.upper.x, aabb.upper.y, aabb.upper.z }),
    };

    // Calculate a model scaling factor that fits the entire model into the view
    auto emin = Vec3 { std::numeric_limits<float>::max() };
    auto emax = Vec3 { std::numeric_limits<float>::lowest() };
    for (const auto e : edges)
    {
        emin.x = std::min(emin.x, e.x);
        emin.y = std::min(emin.y, e.y);

        emax.x = std::max(emax.x, e.x);
        emax.y = std::max(emax.y, e.y);
    }

    return std::max(std::abs(emax.x - emin.x), std::abs(emax.y - emin.y)) * 0.5f;
}

} // namespace
