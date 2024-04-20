#include "Voxel.hpp"

Voxel::Voxel(glm::vec3 position, glm::vec3 color) : position(position), color(color) {
  vertices = {
    Vertex(glm::vec3(0.5f, 0.5f, 0.5f) + position, color),
    Vertex(glm::vec3(0.5f, -0.5f, 0.5f) + position, color),
    Vertex(glm::vec3(-0.5f, -0.5f, 0.5f) + position, color),
    Vertex(glm::vec3(-0.5f, 0.5f, 0.5f) + position, color),
    Vertex(glm::vec3(0.5f, 0.5f, -0.5f) + position, color),
    Vertex(glm::vec3(0.5f, -0.5f, -0.5f) + position, color),
    Vertex(glm::vec3(-0.5f, -0.5f, -0.5f) + position, color),
    Vertex(glm::vec3(-0.5f, 0.5f, -0.5f) + position, color)
  };

  vao.Bind();
  VBO vbo(vertices);
  EBO ebo(indices);

  vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
  vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));

  vao.Unbind();
  vbo.Unbind();
  ebo.Unbind();
};

void Voxel::Draw(Shader& shader, Camera& camera) {
  shader.Use();
  vao.Bind();
  camera.Matrix(shader, "camMatrix");
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  vao.Unbind();
};