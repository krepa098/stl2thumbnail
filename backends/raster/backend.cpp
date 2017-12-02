#include "backend.h"
#include "aabb.h"
#include "zbuffer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

glm::vec3 vec3ToGlm(const Vec3& v)
{
    return { v.x, v.y, v.z };
}

float edgeFunction(glm::vec2 p, glm::vec2 a, glm::vec2 b)
{
    return (p.x - a.x) * (b.y - a.y) - (p.y - a.y) * (b.x - a.x);
}

glm::vec3 glmMatMulVec3(const glm::mat4x4& mat, glm::vec3 v)
{
    return glm::vec3(mat * glm::vec4{ v.x, v.y, v.z, 1.0f });
}

RasterBackend::RasterBackend(unsigned size)
    : m_size(size)
{
}

RasterBackend::~RasterBackend()
{
}

Picture RasterBackend::render(const Mesh& triangles)
{
    // Rasterizer based on
    // https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation

    ZBuffer m_zbuffer(m_size);
    Picture pic(m_size);
    pic.fill(m_backgroundColor.x, m_backgroundColor.y, m_backgroundColor.z);

    // generate AABB and find its center
    auto aabb           = AABBox(triangles);
    float largestStride = aabb.largestStride() * 1.1f;
    auto center         = vec3ToGlm(aabb.center()) / largestStride;

    // create model view projection matrix
    auto projection    = glm::ortho(-0.5f, 0.5f, -0.5f, 0.5f, 0.1f, 1000.0f);
    auto viewPos       = center + glm::vec3{ 1.f, -1.f, 1.f };
    auto view          = glm::lookAt(viewPos, center + glm::vec3{ 0.f, 0.f, 0.f }, { 0.f, 0.f, -1.f });
    auto model         = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f / largestStride));
    auto modelViewProj = projection * view * model;

    for (const auto& t : triangles)
    {
        // project vertices to screen coordinates
        auto v0 = glmMatMulVec3(modelViewProj, vec3ToGlm(t.vertices[0]));
        auto v1 = glmMatMulVec3(modelViewProj, vec3ToGlm(t.vertices[1]));
        auto v2 = glmMatMulVec3(modelViewProj, vec3ToGlm(t.vertices[2]));

        // triangle bounding box
        float minX = std::min(v0.x, std::min(v1.x, v2.x));
        float minY = std::min(v0.y, std::min(v1.y, v2.y));
        float maxX = std::max(v0.x, std::max(v1.x, v2.x));
        float maxY = std::max(v0.y, std::max(v1.y, v2.y));

        // bounding box in screen space
        unsigned sminX = static_cast<unsigned>(std::max(0, static_cast<int>((minX + 1.0f) / 2.0f * m_size)));
        unsigned sminY = static_cast<unsigned>(std::max(0, static_cast<int>((minY + 1.0f) / 2.0f * m_size)));
        unsigned smaxX = static_cast<unsigned>(std::min(int(m_size), static_cast<int>((maxX + 1.0f) / 2.0f * m_size)));
        unsigned smaxY = static_cast<unsigned>(std::min(int(m_size), static_cast<int>((maxY + 1.0f) / 2.0f * m_size)));

        for (unsigned y = sminY; y < smaxY + 1; ++y)
        {
            for (unsigned x = sminX; x < smaxX + 1; ++x)
            {
                // normalize screen coords [-1,1]
                const float nx = 2.f * (x / static_cast<float>(m_size) - 0.5f);
                const float ny = 2.f * (y / static_cast<float>(m_size) - 0.5f);

                //const float nx = x / static_cast<float>(m_size);
                //const float ny = y / static_cast<float>(m_size);

                auto P  = glm::vec2{ nx, ny };
                auto V0 = glm::vec2(v0);
                auto V1 = glm::vec2(v1);
                auto V2 = glm::vec2(v2);

                bool inside = true;
                inside &= edgeFunction(P, V0, V1) < 0.0f;
                inside &= edgeFunction(P, V1, V2) < 0.0f;
                inside &= edgeFunction(P, V2, V0) < 0.0f;

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
                        Vec3 diffColor = std::max(0.0f, dot(t.normal, m_diffuseDir)) * m_diffuseColor;

                        // specular
                        Vec3 fragPos    = { px, py, pz };
                        Vec3 lightDir   = (m_lightPos - fragPos).normalize();
                        Vec3 viewDir    = (Vec3{ viewPos.x, viewPos.y, viewPos.z } - fragPos).normalize();
                        Vec3 reflectDir = reflect(lightDir, t.normal);
                        Vec3 specColor  = std::min(std::pow(std::max(dot(viewDir, reflectDir), 0.0f), 1.0f), 0.5f) * m_specColor;

                        // merge
                        Vec3 color = (m_ambientColor + diffColor + specColor) * m_modelColor;

                        pic.setRGB(x, y, color.x, color.y, color.z);
                    }
                }
            }
        }
    }

    return pic;
}
