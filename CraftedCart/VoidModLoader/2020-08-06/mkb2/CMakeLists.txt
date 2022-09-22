cmake_minimum_required(VERSION 3.9.0)
project(mkb2 C)

set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/include/mkb2/gamecube.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/mkb2/rel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/mkb2/display.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/mkb2/common.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/mkb2/ball.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/mkb2/ape.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/mkb2/camera.h
    )

add_library(${PROJECT_NAME} INTERFACE)
target_sources(${PROJECT_NAME} INTERFACE ${HEADER_FILES})
target_include_directories(${PROJECT_NAME} INTERFACE include/)
