#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 tex;

out VS_OUT {
    vec3 normal;
    vec2 texCoords;
} vs_out;

uniform mat4 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    mat3 normalMatrix = mat3(transpose(inverse(view * model)));
    vs_out.normal = vec3(vec4(normalMatrix * aNormal, 0.0));
    vs_out.texCoords = tex;
    gl_Position = transform*projection * view * model * vec4(aPos, 1.0);
}
