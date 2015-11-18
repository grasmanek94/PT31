cmake_minimum_required (VERSION 3.0)
project (tmd)

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake" )
set(BOOST_LIBS thread system chrono)

#TODO: require libpixyusb
#find_package (libpixyusb REQUIRED)
find_package (libusb-1.0 REQUIRED)
find_package (Boost 1.49 COMPONENTS ${BOOST_LIBS} REQUIRED)

#option (TARGET_RPI
#        "Compile for raspberry pi" OFF)

set (LIBRARY_OUTPUT_PATH "build/x86")
set (CMAKE_FILES_DIRECTORY "build/CMakeFiles")

if (TARGET_RPI)
	set (CMAKE_C_COMPILER "arm-linux-gnueabihf-gcc")
	set (CMAKE_CXX_COMPILER "arm-linux-gnueabihf-g++")
	set (LIBRARY_OUTPUT_PATH "build/rpi")
endif (TARGET_RPI)

include_directories ("src"
                     "include"
				     "/usr/include/libusb-1.0")
set (CMAKE_CXX_FLAGS "-Werror -Wall")
set (sources "src/PixyLocationProvider.cxx" 
             "src/main.cxx")

add_executable (tmd ${sources})

target_link_libraries(tmd pixyusb)
target_link_libraries(tmd usb-1.0)
target_link_libraries(tmd ${Boost_LIBRARIES})

