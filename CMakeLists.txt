cmake_minimum_required(VERSION 3.28 FATAL_ERROR)
project(VoxelEngine)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE src/main.cpp)

