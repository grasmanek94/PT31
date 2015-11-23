cmake_minimum_required(VERSION 2.8)
project(tmd)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake" )
set(BOOST_LIBS thread system chrono)

set(CMAKE_CXX_FLAGS "-Werror -Wall")

#TODO: require libpixyusb
#find_package (libpixyusb REQUIRED)
find_package(libusb-1.0 REQUIRED)
find_package(Boost 1.49 COMPONENTS ${BOOST_LIBS} REQUIRED)

#option (TARGET_RPI
#        "Compile for raspberry pi" OFF)

if (TARGET_RPI)
        set(CMAKE_C_COMPILER "arm-linux-gnueabihf-gcc")
        set(CMAKE_CXX_COMPILER "arm-linux-gnueabihf-g++")
        set(LIBRARY_OUTPUT_PATH "build/rpi")
endif (TARGET_RPI)

include_directories("src"
                    "include"
                    "/usr/include/libusb-1.0")

aux_source_directory("src" SRC_LIST)

add_executable(${PROJECT_NAME} ${SRC_LIST})

target_link_libraries(tmd pixyusb)
target_link_libraries(tmd usb-1.0)
target_link_libraries(tmd ${Boost_LIBRARIES})

