#version 330 core
out vec4 FragColor;  
in vec3 color;
in vec3 crntPos;

uniform vec3 lightPos;
uniform vec4 lightColor;

void main()
{
    float ambient = 0.2f;
    float diff = max(dot(normalize(lightPos - crntPos), normalize(color)), 0.0f);
    vec3 diffuse = diff * lightColor.rgb;
    vec3 result = (ambient + diffuse) * color;
    FragColor = vec4(result, 1.0f);
}