#ifndef CHUNK_HPP
#define CHUNK_HPP

#include "Voxel.hpp"

const int SIZE = 8;
const int HEIGHT = 8;

class Chunk {
public:
  VAO vao;
  VBO vbo;
  EBO ebo;
  glm::vec3 position;

  Chunk(glm::vec3 position);
  void Draw(Shader& shader, Camera& camera);
  void Generate();
  void GenerateGreedy();
  void Build();
  void Delete();

private:
  std::vector<Vertex> vertices;
  std::vector<GLuint> indices;
};

#endif