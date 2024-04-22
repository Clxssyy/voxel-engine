#include "Voxel.hpp"

Voxel::Voxel(glm::vec3 position, glm::vec3 color) : position(position), color(color) {
  vertices = {
    Vertex{glm::vec3(-0.5f, -0.5f, 0.5f) + position, color},   // Front Bottom left
    Vertex{glm::vec3(0.5f, -0.5f, 0.5f) + position, color},    // Front Bottom right
    Vertex{glm::vec3(0.5f, 0.5f, 0.5f) + position, color},     // Front Top right
    Vertex{glm::vec3(-0.5f, 0.5f, 0.5f) + position, color},    // Front Top left

    Vertex{glm::vec3(-0.5f, -0.5f, -0.5f) + position, color},  // Back Bottom left
    Vertex{glm::vec3(0.5f, -0.5f, -0.5f) + position, color},   // Back Bottom right
    Vertex{glm::vec3(0.5f, 0.5f, -0.5f) + position, color},    // Back Top right
    Vertex{glm::vec3(-0.5f, 0.5f, -0.5f) + position, color}    // Back Top left
  };

  vao.Bind();
  VBO vbo(vertices);

  std::vector<GLuint> indices;
  for (const auto &face : faces)
   for (const auto &index : face.second)
    indices.push_back(index);

  EBO ebo(indices);
  vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
  vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));

  vao.Unbind();
  vbo.Unbind();
  ebo.Unbind();
};

void Voxel::Draw(Shader& shader, Camera& camera) {
  std::size_t size = faces["front"].size() * faces.size();

  shader.Use();
  vao.Bind();
  camera.Matrix(shader, "camMatrix");
  glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
  vao.Unbind();
};

void Voxel::Delete() {
  vao.Delete();
};
