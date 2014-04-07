# Copyright: (C) 2014 ISIR
# Author: Guillaume Hamon


# YARP needs CMake 2.6 or greater
cmake_minimum_required(VERSION 2.8.7)

# find YARP
find_package(YARP REQUIRED)

# add YARP include directories
include_directories(${YARP_INCLUDE_DIRS})

# set up our program
add_executable(JTScalibration JTScalibration.cpp)

# link with YARP libraries
target_link_libraries(JTScalibration ${YARP_LIBRARIES})

