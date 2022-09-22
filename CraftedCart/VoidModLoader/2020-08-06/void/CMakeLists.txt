cmake_minimum_required(VERSION 3.9.0)
project(void C)

list(APPEND CMAKE_SHARED_LINKER_FLAGS "--module-id 86797368") # VOID in ASCII is 86 79 73 68 :)

# Be really pedantic!
add_definitions(-Wall -Wextra -pedantic)

include_directories(./src)
include_directories(./include)

set(SOURCE_FILES
    ./src/void/init.c
    ./src/void/font.c
    ./src/void/draw.c
    ./src/void/stage_info.c
    )

set(HEADER_FILES
    ./src/void/init.h
    ./src/void/font.h
    ./include/void/draw.h
    ./include/void/stage_info.h
    )

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(${PROJECT_NAME} mkb2 vml_init)

# Export headers
target_include_directories(
    ${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:include>
    PRIVATE src
    )

install(TARGETS ${PROJECT_NAME} DESTINATION vml)
