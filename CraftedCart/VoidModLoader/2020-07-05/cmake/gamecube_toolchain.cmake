set(CMAKE_SYSTEM_NAME GameCube)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_SYSTEM_PROCESSOR powerpc-eabi)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

set(target powerpc-none-eabi)
set(CMAKE_C_COMPILER_TARGET ${target})
set(CMAKE_CXX_COMPILER_TARGET ${target})

set(CMAKE_LINKER /home/alice/Developer/cube_code/target/debug/cube_code)

# We don't really support .dol executables yet, so this here all won't work, but it makes CMake happy
set(CMAKE_C_LINK_EXECUTABLE "<CMAKE_LINKER> link <FLAGS> <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET> --symbol-map /home/alice/SMBHacking/VoidModLoader/smb2_symbol_map.json --emit-symbol-map <TARGET>.json <LINK_LIBRARIES>")
set(CMAKE_CXX_LINK_EXECUTABLE "<CMAKE_LINKER> link <FLAGS> <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET> --symbol-map /home/alice/SMBHacking/VoidModLoader/smb2_symbol_map.json --emit-symbol-map <TARGET>.json <LINK_LIBRARIES>")

# For .rel libraries
set(CMAKE_C_CREATE_SHARED_LIBRARY "${CMAKE_C_LINK_EXECUTABLE}")
set(CMAKE_CXX_CREATE_SHARED_LIBRARY "${CMAKE_CXX_LINK_EXECUTABLE}")

# Name our libraries name.rel, not libname.so
set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_SHARED_LIBRARY_PREFIX_C "")
set(CMAKE_SHARED_LIBRARY_PREFIX_CXX "")
set(CMAKE_SHARED_LIBRARY_SUFFIX ".rel")
set(CMAKE_SHARED_LIBRARY_SUFFIX_C ".rel")
set(CMAKE_SHARED_LIBRARY_SUFFIX_CXX ".rel")

set(CMAKE_LINK_LIBRARY_FILE_FLAG "--symbol-map ")
set(CMAKE_C_LINK_LIBRARY_FILE_FLAG "--symbol-map ")
set(CMAKE_CXX_LINK_LIBRARY_FILE_FLAG "--symbol-map ")

set(CMAKE_IMPORT_LIBRARY_SUFFIX ".rel.json")
set(CMAKE_IMPORT_LIBRARY_SUFFIX_C ".rel.json")
set(CMAKE_IMPORT_LIBRARY_SUFFIX_CXX ".rel.json")

# Don't use -fPIC, that breaks linking
set(CMAKE_POSITION_INDEPENDENT_CODE OFF)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# The CMake compiler check fails because `--module-id` isn't passed to `cube_code` when linking. I'm not really sure of
# any good way to give a module ID to the test run while requiring it to be overridden in modules sooo we just assume it
# works.
set(CMAKE_C_COMPILER_WORKS TRUE)
set(CMAKE_CXX_COMPILER_WORKS TRUE)
