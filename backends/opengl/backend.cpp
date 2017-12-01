#include "backend.h"

#include "aabb.h"
#include "glad.h"

#include <EGL/egl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

OpenGLBackend::OpenGLBackend(unsigned size)
    : m_size(size)
{
    // EGL config attributes
    const EGLint configAttribs[] = {
        EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
        EGL_BLUE_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_RED_SIZE, 8,
        EGL_DEPTH_SIZE, 8,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
        EGL_NONE
    };

    // EGL context attributes
    EGLint eglContextAttributes[] = {
        EGL_CONTEXT_MAJOR_VERSION,
        3,
        EGL_CONTEXT_MINOR_VERSION,
        2,
        EGL_NONE
    };

    // EGL pBuffer attributes
    const EGLint pbufferAttribs[] = {
        EGL_WIDTH,
        static_cast<EGLint>(size),
        EGL_HEIGHT,
        static_cast<EGLint>(size),
        EGL_NONE,
    };

    // https://devblogs.nvidia.com/parallelforall/egl-eye-opengl-visualization-without-x-server/
    // init egl
    EGLDisplay m_eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    EGLint major, minor;
    eglInitialize(m_eglDisplay, &major, &minor);

    // select config
    EGLint numConfigs;
    EGLConfig eglCfg;
    eglChooseConfig(m_eglDisplay, configAttribs, &eglCfg, 1, &numConfigs);

    // create surface
    EGLSurface eglSurf = eglCreatePbufferSurface(m_eglDisplay, eglCfg, pbufferAttribs);
    m_eglSurface       = eglSurf;

    // create context and make it current
    EGLContext eglCtx = eglCreateContext(m_eglDisplay, eglCfg, EGL_NO_CONTEXT, eglContextAttributes);
    eglMakeCurrent(m_eglDisplay, eglSurf, eglSurf, eglCtx);

    // openGL setup
    if (!gladLoadGL())
    {
        std::cout << "Failed to init opengl" << std::endl;
        exit(1); // no point to go on
    }

    std::cout << "OpenGL " << GLVersion.major << "." << GLVersion.minor << std::endl;

    //https://learnopengl.com/#!Lighting/Basic-Lighting
    const std::string fragShader = R"(
    #version 330 core
    out vec3 color;
    in vec3 normal;
    in vec3 fragPos;
    uniform vec3 modelColor;
    uniform vec3 diffuseLightDir;
    uniform vec3 diffuseLightColor;
    uniform vec3 ambientLightColor;
    uniform vec3 specLightColor;
    uniform vec3 lightPos;
    uniform vec3 viewPos;
    void main()
    {
        vec3 lightDir = normalize(lightPos - fragPos);
        vec3 viewDir = normalize(viewPos - fragPos);
        vec3 reflectDir = reflect(-lightDir, normal);
        vec3 specColor = pow(max(dot(viewDir, reflectDir), 0.0), 0.5) * specLightColor * 0.5;
        vec3 diffuseColor = max(0.0, dot(diffuseLightDir, normal)) * diffuseLightColor;

        color = (ambientLightColor + diffuseColor + specColor) * modelColor;
    }
    )";

    const std::string vertShader = R"(
    #version 330 core
    layout(location = 0) in vec3 vPos;
    layout (location = 1) in vec3 vNormal;
    uniform mat4 viewproj;
    uniform mat4 model;

    out vec3 normal;
    out vec3 fragPos;

    void main() {
        normal = normalize(viewproj * vec4(vNormal,1)).xyz;
        gl_Position = viewproj * model * vec4(vPos,1);
        fragPos = (model * vec4(vPos,1)).xyz;
    }
    )";

    // load the shader
    m_program = loadProgram(vertShader.c_str(), fragShader.c_str());

    // enable backface culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // stl files use counter clockwise faces
    glFrontFace(GL_CCW);

    // enable depth test
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}

OpenGLBackend::~OpenGLBackend()
{
    eglTerminate(m_eglDisplay);
}

Picture OpenGLBackend::render(const Mesh& triangles)
{
    Picture pic(m_size);

    // calculate bounding box
    auto aabb           = AABBox(triangles);
    float largestStride = aabb.largestStride() * 1.3f;
    auto center         = glm::vec3{ aabb.center().x, aabb.center().y, aabb.center().z } / largestStride;

    // pack the triangles for later use in openGL
    std::vector<Vec3> vertices;
    vertices.reserve(triangles.size() * 3 * 3);

    for (const auto& t : triangles)
    {
        vertices.push_back(t.vertices[0]);
        vertices.push_back(t.normal);
        vertices.push_back(t.vertices[1]);
        vertices.push_back(t.normal);
        vertices.push_back(t.vertices[2]);
        vertices.push_back(t.normal);
    }

    // setup camera
    auto projection = glm::ortho(-0.5f, 0.5f, -0.5f, 0.5f, 0.1f, 1000.0f);

    // the view centers the object
    auto viewPos = center + glm::vec3{ 1.f, -1.f, 1.f };
    auto view    = glm::lookAt(viewPos, center + glm::vec3{ 0.f, 0.f, 0.f }, { 0.f, 0.f, -1.f });

    auto model = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f / largestStride));

    auto viewproj = projection * view;

    glClearColor(m_backgroundColor.x, m_backgroundColor.y, m_backgroundColor.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(m_program);

    // setup program
    glUniformMatrix4fv(glGetUniformLocation(m_program, "viewproj"), 1, GL_FALSE, glm::value_ptr(viewproj));
    glUniformMatrix4fv(glGetUniformLocation(m_program, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniform3fv(glGetUniformLocation(m_program, "diffuseLightDir"), 1, glm::value_ptr(glm::vec3{ 1.0f, 1.0f, -1.0f }));
    glUniform3fv(glGetUniformLocation(m_program, "diffuseLightColor"), 1, glm::value_ptr(glm::vec3{ m_diffuseColor.x, m_diffuseColor.y, m_diffuseColor.z }));
    glUniform3fv(glGetUniformLocation(m_program, "ambientLightColor"), 1, glm::value_ptr(glm::vec3{ m_ambientColor.x, m_ambientColor.y, m_ambientColor.z }));
    glUniform3fv(glGetUniformLocation(m_program, "specLightColor"), 1, glm::value_ptr(glm::vec3{ m_specColor.x, m_specColor.y, m_specColor.z }));
    glUniform3fv(glGetUniformLocation(m_program, "modelColor"), 1, glm::value_ptr(glm::vec3{ m_modelColor.x, m_modelColor.y, m_modelColor.z }));
    glUniform3fv(glGetUniformLocation(m_program, "lightPos"), 1, glm::value_ptr(glm::vec3{ 1.0f, 1.0f, -1.0f }));
    glUniform3fv(glGetUniformLocation(m_program, "viewPos"), 1, glm::value_ptr(viewPos));

    // vertex array
    GLuint va;
    glGenVertexArrays(1, &va);
    glBindVertexArray(va);

    // vertex buffer
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(Vec3)), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // vertex attributes
    // positions
    glVertexAttribPointer(
        0, // attribute
        3, // size
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        6 * sizeof(float), // stride
        nullptr // array buffer offset
    );
    glEnableVertexAttribArray(0);
    // normals
    glVertexAttribPointer(
        1, // attribute
        3, // size
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        6 * sizeof(float), // stride
        (void*)(3 * sizeof(float)) // array buffer offset
    );
    glEnableVertexAttribArray(1);
    // Draw the triangle !
    glUseProgram(m_program);
    glBindVertexArray(va);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawArrays(GL_TRIANGLES, 0, triangles.size() * 3);
    glDisableVertexAttribArray(0);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glReadPixels(0, 0, m_size, m_size, GL_RGB, GL_UNSIGNED_BYTE, pic.data());

    return pic;
}

unsigned OpenGLBackend::loadProgram(const char* vert, const char* frag)
{
    GLint Result = GL_FALSE;
    int InfoLogLength;

    auto vertShader = glCreateShader(GL_VERTEX_SHADER);
    auto fragShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertShader, 1, &vert, nullptr);
    glShaderSource(fragShader, 1, &frag, nullptr);

    glCompileShader(vertShader);
    glCompileShader(fragShader);

    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &InfoLogLength);

    if (InfoLogLength > 0)
    {
        std::vector<char> shaderErrorMessage(static_cast<size_t>(InfoLogLength + 1));
        glGetShaderInfoLog(fragShader, InfoLogLength, nullptr, &shaderErrorMessage[0]);
        std::cout << "Fragment Shader\n"
                  << &shaderErrorMessage[0] << std::endl;
    }

    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &InfoLogLength);

    if (InfoLogLength > 0)
    {
        std::vector<char> shaderErrorMessage(static_cast<size_t>(InfoLogLength + 1));
        glGetShaderInfoLog(vertShader, InfoLogLength, nullptr, &shaderErrorMessage[0]);
        std::cout << "Vertex Shader\n"
                  << &shaderErrorMessage[0] << std::endl;
    }

    auto program = glCreateProgram();
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);

    glGetShaderiv(program, GL_LINK_STATUS, &Result);
    glGetShaderiv(program, GL_INFO_LOG_LENGTH, &InfoLogLength);

    if (InfoLogLength > 0)
    {
        std::vector<char> shaderErrorMessage(static_cast<size_t>(InfoLogLength + 1));
        glGetShaderInfoLog(program, InfoLogLength, nullptr, &shaderErrorMessage[0]);
        std::cout << "Program\n"
                  << &shaderErrorMessage[0] << std::endl;
    }

    return program;
}
