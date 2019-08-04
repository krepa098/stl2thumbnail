cmake_minimum_required(VERSION 3.1)

project(stl2thumbnail-gnome VERSION 1.0)

find_package(PNG REQUIRED)

include_directories(${PNG_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/lib)

set(SRC_FILES
    "main.cpp"
)

set(LIBS
    stl2thumb
)

add_executable (
    ${PROJECT_NAME}
    ${SRC_FILES}
    "main.cpp"
)

target_link_libraries(${PROJECT_NAME} ${LIBS})

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/dist/linux/stl.thumbnailer
                ${CMAKE_CURRENT_BINARY_DIR}/stl.thumbnailer
)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION "lib"
    RUNTIME DESTINATION "bin"
)

install(FILES "${CMAKE_SOURCE_DIR}/dist/linux/stl.thumbnailer" DESTINATION "share/thumbnailers")
