#ifndef VOXEL_HPP
#define VOXEL_HPP

#include <glm/glm.hpp>
#include <vector>

#include "VAO.hpp"
#include "EBO.hpp"
#include "Shader.hpp"
#include "Camera.hpp"

class Voxel {
public:
  glm::vec3 position;
  glm::vec3 color;
  VAO vao;

  Voxel(glm::vec3 position, glm::vec3 color);

  void Draw(Shader& shader, Camera& camera);

private:
  std::vector<Vertex> vertices;

  std::vector<GLuint> indices = {
    0, 1, 2, 2, 3, 0,
    1, 5, 6, 6, 2, 1,
    7, 6, 5, 5, 4, 7,
    4, 0, 3, 3, 7, 4,
    4, 5, 1, 1, 0, 4,
    3, 2, 6, 6, 7, 3
  };
};

#endif
