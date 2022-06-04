cmake_minimum_required(VERSION 3.0.0)
project(braidley VERSION 0.1.0)

INCLUDE_DIRECTORIES(
    C:/vcpkg/installed/x64-windows/include
)

LINK_DIRECTORIES(
   C:/vcpkg/installed/x64-windows/lib
) 

add_executable(braidley main.cpp)

TARGET_LINK_LIBRARIES(braidley
    raylib
)