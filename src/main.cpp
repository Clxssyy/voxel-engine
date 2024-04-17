#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>
#include <vector>

#include "Shader.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"

int main() {
  const int WINDOW_WIDTH = 800;
  const int WINDOW_HEIGHT = 600;

  // Triangle vertices (position, color)
  GLfloat triangle[] = {
    -0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.0f,
     0.5f, -0.5f, 0.0f, 0.5f, 0.2f, 0.0f,
     0.0f,  0.5f, 0.0f, 0.5f, 0.8f, 0.0f
  };

  GLfloat square[] = {
    -0.2f, -0.2f, 0.0f, 0.2f, 0.2f, 0.0f,
     0.2f, -0.2f, 0.0f, 0.2f, 0.2f, 0.0f,
     0.2f,  0.2f, 0.0f, 0.2f, 0.8f, 0.0f,
    -0.2f,  0.2f, 0.0f, 0.2f, 0.2f, 0.0f
  };

  GLuint indices[] = {
    0, 1, 2,
    2, 3, 0
  };

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

  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

  // Shader initialization
  Shader shader("../shaders/vertex.vs", "../shaders/fragment.fs");

  VAO VAO1;
  VAO1.Bind();

  VBO VBO1(triangle, sizeof(triangle));
  VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(GLfloat), (void*)0);
  VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

  VAO1.Unbind();
  VBO1.Unbind();

  VAO VAO2;
  VAO2.Bind();

  VBO VBO2(square, sizeof(square));
  EBO EBO1(indices, sizeof(indices));
  VAO2.LinkAttrib(VBO2, 0, 3, GL_FLOAT, 6 * sizeof(GLfloat), (void*)0);
  VAO2.LinkAttrib(VBO2, 1, 3, GL_FLOAT, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

  VAO1.Unbind();
  VBO2.Unbind();
  EBO1.Unbind();

  // Main loop
  do {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader.Use();

    VAO1.Bind();
    glDrawArrays(GL_TRIANGLES, 0, 3);

    VAO2.Bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);

    glfwPollEvents();
  } while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE));

  // Cleanup
  VAO1.Delete();
  VBO1.Delete();
  VAO2.Delete();
  VBO2.Delete();
  EBO1.Delete();
  shader.Delete();
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
