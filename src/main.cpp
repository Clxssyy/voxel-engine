#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

#include "Voxel.hpp"

// Window dimensions
const GLint WINDOW_WIDTH = 800;
const GLint WINDOW_HEIGHT = 600;

// Scale
const GLfloat scale = 0.1f;

// Time
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

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
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

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

  // Voxel
  Voxel voxel(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

  Voxel voxel2(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));

  Voxel voxel3(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

  Voxel voxel4(glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f));

  glEnable(GL_DEPTH_TEST);

  GLuint scaleLoc = glGetUniformLocation(shader.ID, "scale");

  // Camera creation
  Camera camera(glm::vec3(0.0f, 0.0f, 2.0f), WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 100.0f);

  // Main loop
  do {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GLfloat currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    shader.Use();

    glUniform1f(scaleLoc, scale);

    camera.Inputs(window, deltaTime);
    camera.UpdateMatrix(45.0f, 0.1f, 100.0f);

    voxel.Draw(shader, camera);
    voxel2.Draw(shader, camera);
    voxel3.Draw(shader, camera);
    voxel4.Draw(shader, camera);

    glfwSwapBuffers(window);

    glfwPollEvents();
  } while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE));

  // Cleanup

  shader.Delete();
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
