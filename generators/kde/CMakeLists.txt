cmake_minimum_required(VERSION 3.1)

project(stl2thumbnail-kde VERSION 1.0)

find_package(ECM NO_MODULE REQUIRED)
find_package(KF5KIO REQUIRED)

set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

include(KDEInstallDirs) # provides SERVICES_INSTALL_DIR

include_directories(${CMAKE_SOURCE_DIR}/lib)

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

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION "lib"
)

install(FILES "${CMAKE_SOURCE_DIR}/dist/linux/stl-thumbnailer.desktop" DESTINATION ${SERVICES_INSTALL_DIR})
