cmake_minimum_required(VERSION 3.9.0)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(CMAKE_TOOLCHAIN_FILE ./cmake/gamecube_toolchain.cmake)

project(vml)

# Export compile commands for editor autocomplete
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories("/home/alice/SMBHacking/NINTENDO GameCube SDK 1.0/include")

add_subdirectory(./mkb2)
add_subdirectory(./vml_init)
add_subdirectory(./vml_config)
add_subdirectory(./void)
