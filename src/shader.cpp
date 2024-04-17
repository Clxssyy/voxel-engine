#include "shader.hpp"

std::string ReadFile(const char* filePath) {
    std::ifstream in(filePath, std::ios::binary);
    if (!in) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return "";
    }

    std::string content;
    in.seekg(0, std::ios::end);
    content.resize(in.tellg());
    in.seekg(0, std::ios::beg);
    in.read(&content[0], content.size());
    in.close();

    return content;
}

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    std::string vertexContent = ReadFile(vertexPath);
    std::string fragmentContent = ReadFile(fragmentPath);

    const char* vertexSource = vertexContent.c_str();
    const char* fragmentSource = fragmentContent.c_str();

    // Vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, nullptr);
    glCompileShader(vertexShader);

    // Fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
    glCompileShader(fragmentShader);

    // Shader program
    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);

    // Clean up
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

}

void Shader::Use() {
    glUseProgram(ID);
}

void Shader::Delete() {
    glDeleteProgram(ID);
}
