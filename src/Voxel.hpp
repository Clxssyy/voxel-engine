#ifndef VOXEL_HPP
#define VOXEL_HPP

#include <glm/glm.hpp>
#include <vector>
#include <map>

#include "VAO.hpp"
#include "EBO.hpp"
#include "Shader.hpp"
#include "Camera.hpp"

class Voxel {
public:
  glm::vec3 position;
  glm::vec3 color;
  VAO vao;
  std::vector<Vertex> vertices;
  std::map<std::string, std::vector<GLuint>> faces = {
    {"front",{
      0, 1, 2,
      0, 2, 3
    }},
    {"back",{
      5, 4, 7,
      5, 7, 6
    }},
    {"right",{
      1, 5, 6,
      1, 6, 2
    }},
    {"left",{
      4, 0, 3,
      4, 3, 7
    }},
    {"top",{
      3, 2, 6,
      3, 6, 7
    }},
    {"bottom",{
      4, 5, 1,
      4, 1, 0
    }}
  };

  Voxel() = default;
  Voxel(glm::vec3 position, glm::vec3 color);

  void Draw(Shader& shader, Camera& camera);
};

#endif
