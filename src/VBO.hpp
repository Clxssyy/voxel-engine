#ifndef VBO_HPP
#define VBO_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Color;

    Vertex(glm::vec3 position, glm::vec3 color) {
        Position = position;
        Color = color;
    }
};

class VBO {
public:
    GLuint ID;

    VBO(std::vector<Vertex>& vertices);

    void Bind();
    void Unbind();
    void Delete();
};

#endif
