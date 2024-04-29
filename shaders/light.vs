#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec4 lightColor;

uniform mat4 camMatrix;
uniform float scale;

void main()
{
    gl_Position = camMatrix * vec4(aPos * scale, 1.0f);
    lightColor = vec4(aColor, 1.0f);
}