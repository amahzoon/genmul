cmake_minimum_required(VERSION 2.8.12)
project(example)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g3")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR})

subdirs(src)

## this is necessary Emscripten to get js output
if (EMSCRIPTEN)
  SET(CMAKE_EXECUTABLE_SUFFIX ".js")
  set(CMAKE_CXX_FLAGS "--bind -s ALLOW_MEMORY_GROWTH=1")
endif()
