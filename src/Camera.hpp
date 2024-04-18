#ifndef CAMERA_HPP
#define CAMERA_HPP

#define GLM_ENABLE_EXPERIMENTAL
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <GLFW/glfw3.h>

#include "Shader.hpp"

class Camera {
public:
    glm::vec3 position;
    glm::vec3 orientation;
    glm::vec3 up;

    int windowWidth;
    int windowHeight;

    bool firstClick;

    float speed;
    float sensitivity;

    Camera(glm::vec3 position, int windowWidth, int windowHeight);

    void Inputs(GLFWwindow* window, float deltaTime);

    void UpdateMatrix(float FOVdeg, float nearPlane, float farPlane, const char* uniform, Shader& shader);
};

#endif
