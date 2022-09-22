cmake_minimum_required(VERSION 3.9.0)
project(vml_config C)

list(APPEND CMAKE_SHARED_LINKER_FLAGS "--module-id 65")

# Be really pedantic!
add_definitions(-Wall -Wextra -pedantic)

include_directories(./src)
include_directories(./include)

set(SOURCE_FILES
    ./src/vml/config/init.c
    ./src/vml/config/font.c
    )

set(HEADER_FILES
    ./src/vml/config/init.h
    ./src/vml/config/font.h
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
