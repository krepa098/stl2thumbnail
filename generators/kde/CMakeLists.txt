cmake_minimum_required(VERSION 3.1)

project(stl2thumbnail-kde VERSION 1.0)

find_package(PNG REQUIRED)
find_package(KF5KIO REQUIRED)

include_directories(${PNG_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/lib)

set(SRC_FILES
    "thumbcreator.cpp"
)

set(LIBS
    stl2thumb
    KF5::KIOCore
    KF5::KIOFileWidgets
    KF5::KIOWidgets
    KF5::KIONTLM
)

add_library(${PROJECT_NAME} MODULE ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} ${LIBS})

