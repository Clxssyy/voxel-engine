#include "Chunk.hpp"

Chunk::Chunk(glm::vec3 position) : position(position * glm::vec3(SIZE, HEIGHT, SIZE)) {};

void Chunk::Draw(Shader& shader, Camera& camera) {
  shader.Use();
  vao.Bind();
  camera.Matrix(shader, "camMatrix");
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  vao.Unbind();
};

void Chunk::Generate() {
  for (int x = 0; x < SIZE; x++)
    for (int y = 0; y < HEIGHT; y++)
      for (int z = 0; z < SIZE; z++) {
          Voxel voxel(glm::vec3(x, y, z) + position, glm::vec3(1.0f, 1.0f, 1.0f) * glm::vec3(x, y, z) / glm::vec3(SIZE, HEIGHT, SIZE));
          if (x == 0 || x == SIZE - 1 || y == 0 || y == HEIGHT - 1 || z == 0 || z == SIZE - 1)
            for (const auto &vertex : voxel.vertices)
              vertices.push_back(vertex);
          if (x == 0)
            for (const auto &index : voxel.faces["left"])
              indices.push_back(index + vertices.size() - 8);
          if (x == SIZE - 1)
            for (const auto &index : voxel.faces["right"])
              indices.push_back(index + vertices.size() - 8);
          if (y == 0)
            for (const auto &index : voxel.faces["bottom"])
              indices.push_back(index + vertices.size() - 8);
          if (y == HEIGHT - 1)
            for (const auto &index : voxel.faces["top"])
              indices.push_back(index + vertices.size() - 8);
          if (z == 0)
            for (const auto &index : voxel.faces["back"])
              indices.push_back(index + vertices.size() - 8);
          if (z == SIZE - 1)
            for (const auto &index : voxel.faces["front"])
              indices.push_back(index + vertices.size() - 8);

          voxel.Delete();
          // Render all faces
          // for (const auto &face : voxel.faces)
          //   for (const auto &index : face.second)
          //     indices.push_back(index + vertices.size() - 8);
      }
};

void Chunk::GenerateGreedy() {

}

void Chunk::Build() {
  vao.Bind();

  vbo = VBO(vertices);
  ebo = EBO(indices);

  vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
  vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));

  vao.Unbind();
  vbo.Unbind();
  ebo.Unbind();
};

void Chunk::Delete() {
  vao.Delete();
  vbo.Delete();
  ebo.Delete();
};
