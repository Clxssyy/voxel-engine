#version 330 core

in vec4 lightColor;
out vec4 FragColor;

void main()
{
    FragColor = lightColor;
}