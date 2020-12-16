#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 aTexCoord;
out vec3 vertexColor;
out float light;
out VS_OUT {
    vec3 normal;
} vs_out;

uniform mat4 transform;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

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
    vs_out.normal = mat3(transpose(inverse(model))) * normal;
}
