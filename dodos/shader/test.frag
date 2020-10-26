#version 330 core
out vec4 FragColor;
in vec3 vertexColor;
in vec2 TexCoord;
in vec3 Normal;
in float light;

uniform float ourColorFactor; // 在OpenGL程序代码中设定这个变量
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
//    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.8);
    FragColor = texture(texture1,TexCoord);
//    FragColor = vec4(vec3(dot(Normal,vec3(0.0,1.0,0.0))),1.0);
////    FragColor = vec4(vec3(Normal.x*10.0),1.0);
//    FragColor = vec4(vec3(light),1.0);
}
