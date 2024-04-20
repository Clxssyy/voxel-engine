cmake_minimum_required(VERSION 3.28 FATAL_ERROR)
project(VoxelEngine)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE src/main.cpp src/Shader.cpp src/VAO.cpp src/VBO.cpp src/EBO.cpp src/Camera.cpp src/Voxel.cpp src/Chunk.cpp)

# GLFW
add_subdirectory(include/glfw)

# GLAD
add_subdirectory(include/glad)

# GLM
add_subdirectory(include/glm)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw glad glm)
