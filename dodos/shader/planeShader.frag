#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform float ourColorFactor; // 在OpenGL程序代码中设定这个变量
uniform sampler2D texture1;
uniform vec3 viewPos;

uniform vec3 ambientColor;

uniform sampler2D tex0;

uniform bool isNormalTextureMap;


void main()
{
    FragColor = texture(tex0, TexCoords);
//    FragColor = vec4(1.0f);
}
