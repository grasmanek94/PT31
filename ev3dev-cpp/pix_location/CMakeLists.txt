cmake_minimum_required (VERSION 3.3)
project (pixylocation)

#find_package (libpixyusb REQUIRED)

include_directories (${PROJECT_SOURCE_DIR}/include
				     /usr/include/libusb-1.0)
set (CMAKE_CXX_FLAGS "-Werror -Wall")
set (sources "src/PixyLocationProvider.cxx")
add_library (pixylocation ${sources})

