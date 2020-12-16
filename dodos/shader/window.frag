#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float bloomFactor;

float getColor(vec4 color){
    return  0.2125 * color.r + 0.7154 * color.g + 0.0721 * color.b;
}

vec3 getKernel(){
    const float offset = 1.0 / 300.0f;

    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // 左上
        vec2( 0.0f,    offset), // 正上
        vec2( offset,  offset), // 右上
        vec2(-offset,  0.0f),   // 左
        vec2( 0.0f,    0.0f),   // 中
        vec2( offset,  0.0f),   // 右
        vec2(-offset, -offset), // 左下
        vec2( 0.0f,   -offset), // 正下
        vec2( offset, -offset)  // 右下
    );
    
//    float kernel[9] = float[](
//        0,0,0,
//        0,1,0,
//        0,0,0
//    );

//    边缘
//    float kernel[9] = float[](
//        1,1,1,
//        1,-8,1,
//        1,1,1
//    );
    
//    锐化核
//    float kernel[9] = float[](
//        -1, -1, -1,
//        -1,  9, -1,
//        -1, -1, -1
//    );
    
//    模糊核
//    float kernel[9] = float[](
//        1.0 / 16, 2.0 / 16, 1.0 / 16,
//        2.0 / 16, 4.0 / 16, 2.0 / 16,
//        1.0 / 16, 2.0 / 16, 1.0 / 16
//    );
    float kernel[9] = float[](
        0.0947416,0.118318,0.0947416,
        0.118318,0.147761,0.118318,
        0.0947416,0.118318,0.0947416
    );

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(getColor(texture(screenTexture, TexCoords.st + offsets[i])));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];
    
    return col;
}

void main()
{
    vec3 col = getKernel();
    vec4 ori = texture(screenTexture, TexCoords.st);
//    FragColor = ori;
    FragColor = bloomFactor*vec4(col, 1.0)+ori;
}
