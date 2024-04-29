#ifndef CHUNK_HPP
#define CHUNK_HPP

#include "Voxel.hpp"

const int SIZE = 16;
const int HEIGHT = 64;

class Chunk {
public:
  VAO vao;
  VBO vbo;
  EBO ebo;
  glm::vec3 position;

  Voxel voxels[SIZE][HEIGHT][SIZE];

  Chunk(glm::vec3 position);
  void Draw(Shader& shader, Camera& camera);
  void Generate();
  void GenerateGreedy();
  void Greedy(std::string face);
  void GenerateBlocks();
  void Build();
  void Delete();

private:
  std::vector<Vertex> vertices;
  std::vector<GLuint> indices;
};

#endif