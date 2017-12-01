cmake_minimum_required(VERSION 3.0)

project(stl2thumbnail)

add_subdirectory(libstl)

include_directories(libstl)

set(CMAKE_INCLUDE_CURRENT_DIR true)

add_executable (
    ${PROJECT_NAME}
    "main.cpp"
    "picture.cpp"
    "picture.h"
    "aabb.cpp"
    "aabb.h"
    "vec3.h"
    "args.hxx"
    "triangle.cpp"
    "triangle.h"
    "backends/backend.h"
    "backends/opengl/backend.cpp"
    "backends/opengl/backend.h"
    "backends/opengl/glad.c"
    "backends/opengl/glad.h"
)

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/dist/linux/stl.thumbnailer
                ${CMAKE_CURRENT_BINARY_DIR}/stl.thumbnailer
)

target_link_libraries(${PROJECT_NAME} libstl png EGL GL dl)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION "bin")
install(FILES "stl.thumbnailer" DESTINATION "share/thumbnailers")
