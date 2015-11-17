cmake_minimum_required (VERSION 3.3)
project (pixylocation)

include_directories ("${PROJECT_SOURCE_DIR}/include")
set(CMAKE_CXX_FLAGS "-Werror -Wall")
set(sources "PixyLocationProvider.cxx")
add_library(pixylocation ${sources})

