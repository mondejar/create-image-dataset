cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

find_package( OpenCV REQUIRED )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2")

add_executable (create_dataset transformer_patch.cpp main.cpp)
target_link_libraries (create_dataset ${OpenCV_LIBS})

