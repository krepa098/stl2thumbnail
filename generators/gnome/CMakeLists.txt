cmake_minimum_required(VERSION 3.16)

project(stl2thumbnail VERSION 1.0)

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

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(FILES "stl.thumbnailer" DESTINATION "share/thumbnailers")
