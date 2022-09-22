cmake_minimum_required(VERSION 3.9.0)
project(vml_init C)

list(APPEND CMAKE_SHARED_LINKER_FLAGS "--module-id 64")

# Be really pedantic!
add_definitions(-Wall -Wextra -pedantic)

include_directories(./src)
include_directories(./include)

set(SOURCE_FILES
    ./src/vml/init.c
    ./src/vml/rel.c
    ./src/vml/log.c
    ./src/vml/patch.c
    ./src/vml/alloc.c

    ./src/v/container/vec.c
    )

set(HEADER_FILES
    ./src/vml/init.h
    ./include/vml/rel.h
    ./include/vml/log.h
    ./include/vml/patch.h
    ./include/vml/alloc.h

    ./include/v/container/vec.h
    )

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(${PROJECT_NAME} mkb2)

# Export headers
target_include_directories(
    ${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:include>
    PRIVATE src
    )

install(TARGETS ${PROJECT_NAME} DESTINATION vml)
