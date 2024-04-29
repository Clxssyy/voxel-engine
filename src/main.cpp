#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

#include "imgui/imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Chunk.hpp"

// Window dimensions
const GLint WINDOW_WIDTH = 800;
const GLint WINDOW_HEIGHT = 600;

// Scale
GLfloat scale = 0.1f;

// Field of view
GLfloat fov = 45.0f;

// Time
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// Wireframe mode
bool wireframe = false;

// Menu mode
bool menu = false;

// Background color
glm::vec3 background = glm::vec3(0.2f, 0.3f, 0.3f);

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_F && action == GLFW_PRESS)
    wireframe = !wireframe;
  if (key == GLFW_KEY_UP && action == GLFW_PRESS)
    if (scale < 1.0f)
    scale += 0.01f;
  if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
    if (scale > 0.01f)
    scale -= 0.01f;
  if (key == GLFW_KEY_SLASH && action == GLFW_PRESS)
    menu = !menu;
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
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetKeyCallback(window, key_callback);

  // Glad initialization
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    glfwDestroyWindow(window);
    glfwTerminate();
    return 1;
  }

  // ImGui initialization
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
  ImGui_ImplOpenGL3_Init();

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
  for (int x = -4; x < 4; x++) {
    for (int z = -4; z < 4; z++) {
        Chunk chunk(glm::vec3(x, 0.0f, z));
        chunks.push_back(chunk);
        z++;
      }
    x++;
  }

  int time = glfwGetTime();
  for (auto &chunk : chunks) {
    chunk.GenerateBlocks();
    chunk.GenerateGreedy();
    chunk.Build();
  }
  std::cout << "Time: " << glfwGetTime() - time << std::endl;

  std::vector<Chunk> chunks2;
  for (int x = -4; x < 4; x++) {
    x++;
    for (int z = -4; z < 4; z++) {
        z++;
        Chunk chunk(glm::vec3(x, 0.0f, z));
        chunks2.push_back(chunk);
      }
  }

  time = glfwGetTime();
  for (auto &chunk : chunks2) {
    chunk.GenerateBlocks();
    chunk.Generate();
    chunk.Build();
  }
  std::cout << "Time: " << glfwGetTime() - time << std::endl;

  std::vector<Voxel> voxels;

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
  //   // Additional vertices to render lines
  //   // 0, 1, 1, 2, 2, 0, 0, 3, 3, 2, 3,
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
    glfwPollEvents();

    // Menu Creation
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    
    ImGui::Begin("Use '/' to enter Menu Mode");
    ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::SliderFloat("Scale (UP/DOWN)", &scale, 0.01f, 1.0f);
    ImGui::SliderFloat("FOV", &fov, 45.0f, 110.0f);
    ImGui::SliderFloat("Speed", &camera.speed, 0.1f, 10.0f);
    if (ImGui::Button("Wireframe"))
      wireframe = !wireframe;
    ImGui::ColorPicker3("Background", glm::value_ptr(background));
    glm::vec3 chunkPosition;
    ImGui::InputFloat3("Chunk Position", glm::value_ptr(chunkPosition));
    if (ImGui::Button("Add Chunk")) {
      Chunk chunk(chunkPosition);
      chunk.GenerateBlocks();
      chunk.GenerateGreedy();
      chunk.Build();
      chunks.push_back(chunk);
    }
    glm::vec3 voxelPosition;
    ImGui::InputFloat3("Voxel Position", glm::value_ptr(voxelPosition));
    glm::vec3 voxelColor;
    ImGui::ColorPicker3("Voxel Color", glm::value_ptr(voxelColor));
    if (ImGui::Button("Add Voxel")) {
      Voxel voxel(voxelPosition, voxelColor);
      voxels.push_back(voxel);
    }
    ImGui::End();

    // Rendering
    glClearColor(background.x, background.y, background.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);

    // Time
    GLfloat currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    shader.Use();

    // -- Cube Example
    camera.Matrix(shader, "camMatrix");

    glUniform1f(scaleLoc, scale);

    // Menu Mode
    if (!menu) {
      camera.Inputs(window, deltaTime);
    }

    camera.UpdateMatrix(fov, 0.1f, 100.0f);
    camera.UpdateSpeed();

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

    for (auto &chunk : chunks2)
      chunk.Draw(shader, camera);

    for (auto &voxel : voxels)
      voxel.Draw(shader, camera);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
  } while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE));

  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  for (auto &chunk : chunks)
    chunk.Delete();
  shader.Delete();
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
