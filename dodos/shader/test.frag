#version 330 core
out vec4 FragColor;
in vec3 vertexColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform float ourColorFactor; // 在OpenGL程序代码中设定这个变量
uniform sampler2D texture1;
uniform vec3 viewPos;

uniform vec3 ambientColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
    
    vec3 direction;
    float cutOff;
    float outerCutOff;
};

uniform Light light;

void main()
{
    // 环境光
    vec4 fuck = texture(texture1,TexCoord);
    vec3 lightPos = light.position;
    
    float dis = distance(lightPos,FragPos);
    float instenty = 1.0f/(light.constant+light.linear*dis + light.quadratic*dis*dis);
    
    vec3 ambient = light.ambient * material.ambient * fuck.xyz;
    
    // 漫反射
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
//    平行光源
//    vec3 lightDir = normalize(vec3(1.0));
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * fuck.xyz);

    // 镜面光
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), fuck.x*10.0);
    vec3 specular = light.specular  * (spec * material.specular);
    
    float theta     = dot(lightDir, normalize(-light.direction));
    float epsilon   = light.cutOff - light.outerCutOff;
    instenty = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    
    ambient *= instenty;
    diffuse *= instenty;
    specular *= instenty;
    
    vec3 result = ambient + diffuse + specular;
    
    FragColor = vec4(result, 1.0);
}
