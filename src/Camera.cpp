#include "Camera.hpp"

Camera::Camera(glm::vec3 positionN, int windowWidth, int windowHeight, float speed, float sensitivity) {
    Camera::position = positionN;
    Camera::windowWidth = windowWidth;
    Camera::windowHeight = windowHeight;
    Camera::orientation = glm::vec3(0.0f, 0.0f, -1.0f);
    Camera::up = glm::vec3(0.0f, 1.0f, 0.0f);
    Camera::firstClick = true;
    Camera::speed = speed;
    Camera::sensitivity = sensitivity;
    Camera::maxSpeed = speed * 2;
}

void Camera::Inputs(GLFWwindow* window, float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        position += speed * orientation * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        position -= speed * orientation * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        position -= glm::normalize(glm::cross(orientation, up)) * speed * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        position += glm::normalize(glm::cross(orientation, up)) * speed * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        position += glm::normalize(up) * speed * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        position -= glm::normalize(up) * speed * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        if (speed < maxSpeed) speed = speed * 2;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
        if (speed > maxSpeed / 2) speed = speed / 2;
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        if (firstClick) {
            glfwSetCursorPos(window, windowWidth / 2, windowHeight / 2);
            firstClick = false;
        }

        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        float rotX = sensitivity * (float)(mouseY - (windowHeight / 2)) / windowHeight;
        float rotY = sensitivity * (float)(mouseX - (windowWidth / 2)) / windowWidth;

        glm::vec3 newOrientation = glm::rotate(orientation, glm::radians(-rotX), glm::normalize(glm::cross(orientation, up)));

        if (abs(glm::angle(newOrientation, up) - glm::radians(90.0f)) <= glm::radians(85.0f)) {
            orientation = newOrientation;
        }

        orientation = glm::rotate(orientation, glm::radians(-rotY), up);

        glfwSetCursorPos(window, windowWidth / 2, windowHeight / 2);
    } else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        firstClick = true;
    }
}

void Camera::UpdateMatrix(float FOVdeg, float nearPlane, float farPlane, const char* uniform, Shader& shader) {
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    view = glm::lookAt(position, position + orientation, up);
    projection = glm::perspective(glm::radians(FOVdeg), (float)windowWidth / (float)windowHeight, nearPlane, farPlane);

    int uniformLoc = glGetUniformLocation(shader.ID, uniform);
    glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(projection * view));
}