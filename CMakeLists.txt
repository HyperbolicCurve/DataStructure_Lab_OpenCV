cmake_minimum_required(VERSION 3.26)
project(OpenCV)

set(CMAKE_CXX_STANDARD 17)

add_executable(OpenCV
        poisson.cpp
        poisson.h
        Watershed.cpp
        4_Color.cpp
        4_Color.h
)

set(OpenCV_DIR E:/OpenCV/opencv/MinGW-build/install)
find_package(OpenCV REQUIRED)
target_link_libraries(OpenCV ${OpenCV_LIBS})


