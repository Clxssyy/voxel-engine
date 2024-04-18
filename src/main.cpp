#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

#include "Shader.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"
#include "Camera.hpp"

// Window dimensions
const GLint WINDOW_WIDTH = 800;
const GLint WINDOW_HEIGHT = 600;

// Cube vertices (position, color)
GLfloat cube[] = {
  // Front face
  -0.8f, -0.8f,  0.8f, 0.0f, 0.0f, 0.2f,
    0.8f, -0.8f,  0.8f, 0.0f, 0.0f, 0.2f,
    0.8f,  0.8f,  0.8f, 0.0f, 0.0f, 0.2f,
  -0.8f,  0.8f,  0.8f, 0.0f, 0.0f, 0.2f,
  // Back face
  -0.8f, -0.8f, -0.8f, 0.0f, 0.0f, 0.5f,
    0.8f, -0.8f, -0.8f, 0.0f, 0.0f, 0.5f,
    0.8f,  0.8f, -0.8f, 0.0f, 0.0f, 0.5f,
  -0.8f,  0.8f, -0.8f, 0.0f, 0.0f, 0.5f
};

// Cube indices
GLuint cubeIndices[] = {
  // Front face
  0, 1, 2,
  2, 3, 0,
  // Top face
  3, 2, 6,
  6, 7, 3,
  // Back face
  7, 6, 5,
  5, 4, 7,
  // Bottom face
  4, 5, 1,
  1, 0, 4,
  // Left face
  4, 0, 3,
  3, 7, 4,
  // Right face
  1, 5, 6,
  6, 2, 1
};

int main() {
  // Glfw initialization
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW" << std::endl;
    return 1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);   
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Window creation
  GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hello, world!", nullptr, nullptr);
  if (!window) {
    std::cerr << "Failed to create window" << std::endl;
    glfwTerminate();
    return 1;
  }

  glfwMakeContextCurrent(window);

  // Glad initialization
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    glfwDestroyWindow(window);
    glfwTerminate();
    return 1;
  }

  #ifdef _WIN32
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
  #elif __APPLE__
    glViewport(0, 0, WINDOW_WIDTH * 2, WINDOW_HEIGHT * 2);
  #endif

  // Shader initialization
  Shader shader("../shaders/vertex.vs", "../shaders/fragment.fs");

  // VAO, VBO, EBO initialization
  VAO VAO1;
  VAO1.Bind();

  VBO VBO1(cube, sizeof(cube));
  EBO EBO1(cubeIndices, sizeof(cubeIndices));
  VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(GLfloat), (void*)0);
  VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

  VAO1.Unbind();
  VBO1.Unbind();
  EBO1.Unbind();

  glEnable(GL_DEPTH_TEST);

  // Camera creation
  Camera camera(glm::vec3(0.0f, 0.0f, 2.0f), WINDOW_WIDTH, WINDOW_HEIGHT);

  // Main loop
  do {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.Use();

    camera.Inputs(window);
    camera.UpdateMatrix(45.0f, 0.1f, 100.0f, "camMatrix", shader);

    VAO1.Bind();
    glDrawElements(GL_TRIANGLES, sizeof(cubeIndices) / sizeof(int), GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);

    glfwPollEvents();
  } while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE));

  // Cleanup
  VAO1.Delete();
  VBO1.Delete();
  EBO1.Delete();
  shader.Delete();
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
