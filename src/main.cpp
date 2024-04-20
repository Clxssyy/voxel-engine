#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

#include "Chunk.hpp"

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

  // -- Voxel Example
  // Voxel voxel(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));

  // -- Chunk Example
  // Chunk chunk(glm::vec3(0.0f, 0.0f, 0.0f));
  // chunk.Generate();
  // chunk.Build();

  // -- Multiple Chunks Example
  std::vector<Chunk> chunks;
  for (int x = 0; x < 4; x++)
    for (int z = 0; z < 4; z++) {
        Chunk chunk(glm::vec3(x, 0.0f, z));
        chunks.push_back(chunk);
      }

  for (auto &chunk : chunks) {
    chunk.Generate();
    chunk.Build();
  }

  // -- Cube Example using VAO, VBO, EBO
  // std::vector<Vertex> vertices = {
  //   // Position                           // Color
  //   Vertex{glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(1.0f, 0.0f, 0.0f)}, // Front Bottom left
  //   Vertex{glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f)},  // Front Bottom right
  //   Vertex{glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f)},   // Front Top right
  //   Vertex{glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 0.0f)},  // Front Top left

  //   Vertex{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f)}, // Back Bottom left
  //   Vertex{glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f)},  // Back Bottom right
  //   Vertex{glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.0f, 0.0f, 1.0f)},   // Back Top right
  //   Vertex{glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(1.0f, 1.0f, 0.0f)}   // Back Top left
  // };

  // std::vector<GLuint> indices = {
  //   // Front
  //   0, 1, 2,
  //   0, 2, 3,
  //   // Right
  //   1, 5, 6,
  //   1, 6, 2,
  //   // Back
  //   5, 4, 7,
  //   5, 7, 6,
  //   // Left
  //   4, 0, 3,
  //   4, 3, 7,
  //   // Bottom
  //   4, 5, 1,
  //   4, 1, 0,
  //   // Top
  //   3, 2, 6,
  //   3, 6, 7
  // };

  // shader.Use();

  // VAO vao;
  // vao.Bind();
  // VBO vbo(vertices);
  // EBO ebo(indices);

  // vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
  // vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));

  // vao.Unbind();

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  GLuint scaleLoc = glGetUniformLocation(shader.ID, "scale");

  // Camera creation
  Camera camera(glm::vec3(0.0f, 0.0f, 2.0f), WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 100.0f);

  // Main loop
  do {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Time
    GLfloat currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    shader.Use();

    // -- Cube Example
    // camera.Matrix(shader, "camMatrix");

    glUniform1f(scaleLoc, scale);

    camera.Inputs(window, deltaTime);
    camera.UpdateMatrix(45.0f, 0.1f, 100.0f);

    // -- Cube Render
    // vao.Bind();
    // glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    // vao.Unbind();

    // -- Voxel Render
    // voxel.Draw(shader, camera);

    // -- Chunk Render
    // chunk.Draw(shader, camera);

    // -- Multiple Chunks Render
    for (auto &chunk : chunks)
      chunk.Draw(shader, camera);

    glfwSwapBuffers(window);

    glfwPollEvents();
  } while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE));

  // Cleanup
  shader.Delete();
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
