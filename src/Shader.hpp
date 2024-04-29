#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstring>

std::string ReadFile(const char* filePath);

class Shader {
public:
    GLuint ID;

    Shader(const char* vertexPath, const char* fragmentPath);

    void Use();
    void Delete();

    void compileErrors(unsigned int shader, const char* type);
};

#endif