cmake_minimum_required(VERSION 3.25)
project(gimpsep-pmig)

set(CMAKE_CXX_STANDARD 23)
set(ENV{OpenCV_dir} "C:/Users/User/opencv/build")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(OpenCV REQUIRED)
include_directories(${OPENCV_INCLUDE_DIRS})

add_executable(gimpsep-pmig Image.cpp)

set(OpenCV_LIBS opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs)

target_link_libraries(gimpsep-pmig ${OpenCV_LIBS})
