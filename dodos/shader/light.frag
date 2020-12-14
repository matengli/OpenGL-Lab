#version 330 core
out vec4 FragColor;
in vec3 vertexColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 worldPos;
in float light;

uniform vec3 lightColor;

void main()
{
    FragColor = vec4(1.0f);
}
