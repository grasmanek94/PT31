cmake_minimum_required (VERSION 3.3)
project (pixylocation)

#find_package (libpixyusb REQUIRED)

option (TARGET_RPI
        "Compile for raspberry pi" OFF)

set (LIBRARY_OUTPUT_PATH "build/x86")
set (CMAKE_FILES_DIRECTORY "build/CMakeFiles")

if (TARGET_RPI)
	set (CMAKE_C_COMPILER "arm-linux-gnueabihf-gcc")
	set (CMAKE_CXX_COMPILER "arm-linux-gnueabihf-g++")
	set (LIBRARY_OUTPUT_PATH "build/rpi")
endif (TARGET_RPI)

include_directories (include
				     /usr/include/libusb-1.0)
set (CMAKE_CXX_FLAGS "-Werror -Wall")
set (sources "src/PixyLocationProvider.cxx")
add_library (pixylocation ${sources})

