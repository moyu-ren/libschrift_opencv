cmake_minimum_required(VERSION 3.17)
set(TARGET_NAME test2)

project(test2)

set (OpenCV_DIR D:/software/opencv)

include_directories( "C:/Program Files (x86)/Eigen3/include/eigen3" )
find_package(Eigen3 REQUIRED)
INCLUDE_DIRECTORIES(${EIGEN3_INCLUDE_DIR})

include_directories( "./util" )

set (OpenCV_DIR D:/software/opencv)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
set(OpenCV_LIBS opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs)

add_executable (${TARGET_NAME} main.cpp schrift.h schrift.c)
target_link_libraries(${TARGET_NAME} PRIVATE Eigen3::Eigen ${OpenCV_LIBS})

