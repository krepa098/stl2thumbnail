cmake_minimum_required(VERSION 3.1)

project(stl2thumb VERSION 1.0)

set(CMAKE_INCLUDE_CURRENT_DIR true)

find_package(PNG REQUIRED)

include_directories(${PNG_INCLUDE_DIR})

set(SRC_FILES
    "stl/parser.cpp"
    "picture.cpp"
    "aabb.cpp"
    "backends/raster/rasterbackend.cpp"
    "backends/raster/zbuffer.cpp"
)

add_library(${PROJECT_NAME} SHARED ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE ${PNG_LIBRARY})

# Install library
install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# Install header files
install(DIRECTORY "${PROJECT_SOURCE_DIR}"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}"
    FILES_MATCHING
    PATTERN "*.h"
)
