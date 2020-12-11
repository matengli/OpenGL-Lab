#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 aTexCoord;
out vec3 vertexColor;
out vec2 TexCoords;
out vec3 Normal;
out float light;
out vec3 FragPos;

uniform mat4 transform;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = transform*projection * view * model * vec4(aPos, 1.0);
    vertexColor = vec3(0.0);
    FragPos = ( model * vec4(aPos, 1.0)).xyz;
    TexCoords = aTexCoord;
    Normal = mat3(transpose(inverse(model))) * normal;
}
