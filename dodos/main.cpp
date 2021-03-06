//
//  main.cpp
//  dodos
//
//  Created by 马腾理 on 2020/10/11.
///Users/matengli/Desktop/matengli/tempRes/未命名文件夹 3/glfw-3.3.2.bin.MACOS/include/GLFW/glfw3.h

#include "glad/glad.h"
#include "glfw3.h"
#include "math.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader/shader.hpp"
#include "model/Model.hpp"
#include "stb_image.h"

#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Label.hpp"

const int screenWidth = 800;
const int screenHeight = 600;

glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

bool firstMouse = true;
float yaw   = -90.0f;    // yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch =  0.0f;
float lastX =  800.0f / 2.0;
float lastY =  600.0 / 2.0;
float fov   =  45.0f;

float factor1 = 0.0f;
int factor2 = 0;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    float cameraSpeed = 0.05f; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraUp;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraUp;
    
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        factor1 -= 0.1f;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        factor1 += 0.1f;
    
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        factor2 = 1;
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        factor2 = 0;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

void setTransform(Shader* shader,glm::vec3 position,glm::vec3 scale){
    float time = glfwGetTime();
//        int loc = glad_glGetUniformLocation(shaderProgram,"ourColorFactor");
//
//     需要注意的是：查询uniform地址不要求你之前使用过着色器程序，
//     但是更新一个uniform之前你必须先使用程序（调用glUseProgram)，
//     因为它是在当前激活的着色器程序中设置uniform的。
//     说是这么说。。。。。。我直接使用也是ok的
//        glad_glUniform1f(loc,light);
//        glad_glUseProgram(shaderProgram);
    // create transformations
    glm::mat4 model         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 view          = glm::mat4(1.0f);
    glm::mat4 projection    = glm::mat4(1.0f);

    model = glm::translate(model, position);
//    model = glm::translate(model, glm::vec3(sin(time),0,cos(time)));
    model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, scale);
    
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    projection = glm::perspective(glm::radians(fov), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
    
    glm::mat4 transform = glm::mat4(1.0f);

    unsigned int transformLoc = glGetUniformLocation(shader->ID, "model");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model));
    
    transformLoc = glGetUniformLocation(shader->ID, "view");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(view));
    
    transformLoc = glGetUniformLocation(shader->ID, "projection");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(projection));
    
    transformLoc = glGetUniformLocation(shader->ID, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
    
    shader->use();
}

void setTransform(Shader* shader,glm::vec3 position){
    setTransform(shader,position,glm::vec3(1.0f,1.0f,1.0f));
}

void setUpDirLight(Shader* shader,glm::vec3 dir,glm::vec3 color){
//    struct DirLight {
//        vec3 direction;
//
//        vec3 ambient;
//        vec3 diffuse;
//        vec3 specular;
//    };
//    uniform DirLight dirLight;
    shader->setVec3("dirLight.direction",dir);
    shader->setVec3("dirLight.ambient",color);
    shader->setVec3("dirLight.diffuse",color);
    shader->setVec3("dirLight.specular",color);
}

void setUpPointLight(Shader* shader,glm::vec3 position,glm::vec3 color,glm::vec3 paras,int index){
//    struct PointLight {
//        vec3 position;
//
//        float constant;
//        float linear;
//        float quadratic;
//
//        vec3 ambient;
//        vec3 diffuse;
//        vec3 specular;
//    };
//    #define NR_POINT_LIGHTS 4
//    uniform PointLight pointLights[NR_POINT_LIGHTS];
    std::ostringstream   ostr;
    ostr<<"pointLights["<<index<<"]";
    std::string str = ostr.str();
    
    shader->setVec3((str+".position").c_str(),position);
    shader->setFloat((str+".constant").c_str(),paras.x);
    shader->setFloat((str+".linear").c_str(),paras.y);
    shader->setFloat((str+".quadratic").c_str(),paras.z);
    shader->setVec3((str+".ambient").c_str(),color*0.0f);
    shader->setVec3((str+".diffuse").c_str(),color);
    shader->setVec3((str+".specular").c_str(),color);
}

//void setUpSpotLight(Shader* shader,glm::vec3 position,glm::vec3 color,glm::vec3 direction,int index){
void setUpSpotLight(Shader* shader,glm::vec3 position,glm::vec3 color,glm::vec3 direction,int index){

//    struct SpotLight {
//        vec3 position;
//        vec3 direction;
//
//        float outerCutOff;
//        float cutOff;
//
//        vec3 ambient;
//        vec3 diffuse;
//        vec3 specular;
//
//        float constant;
//        float linear;
//        float quadratic;
//    };
    std::ostringstream   ostr;
    ostr<<"spotLights["<<index<<"]";
    std::string str = ostr.str();
    
    shader->setVec3((str+".position").c_str(),position);
    shader->setVec3((str+".direction").c_str(),direction);
    shader->setVec3((str+".ambient").c_str(),color*0.0f);
    shader->setVec3((str+".diffuse").c_str(),color);
    shader->setVec3((str+".specular").c_str(),color);
    
    shader->setFloat((str+".outerCutOff").c_str(),glm::cos(glm::radians(12.5f)));
    shader->setFloat((str+".cutOff").c_str(),glm::cos(glm::radians(2.0f)));
    
    shader->setFloat((str+".constant").c_str(), 1.0f);
    shader->setFloat((str+".linear").c_str(), 0.09);
    shader->setFloat((str+".quadratic").c_str(), 0.032);
}

unsigned int getQuadVAO(){
    float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };
    
    // screen quad VAO
    unsigned int quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    
    return quadVAO;
}

unsigned int getPlaneVAO(){
    float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        1.0f,0.0f, -1.0f,  0.0f,1.0f,0.0f, 1.0f, 0.0f,
        -1.0f,0.0f, -1.0f,  0.0f,1.0f,0.0f, 0.0f, 0.0f,
        -1.0f,0.0f,  1.0f,  0.0f,1.0f,0.0f, 0.0f, 1.0f,

        1.0f,0.0f,  1.0f,  0.0f,1.0f,0.0f, 1.0f, 1.0f,
        1.0f,0.0f, -1.0f,  0.0f,1.0f,0.0f, 1.0f, 0.0f,
        -1.0f,0.0f,  1.0f,  0.0f,1.0f,0.0f, 0.0f, 1.0f
    };
    
    // screen quad VAO
    unsigned int quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    
    return quadVAO;
}

unsigned int getSkyBoxVAO(){
    float skyboxVertices[] = {
        // positions
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    
    return skyboxVAO;;
}
unsigned int loadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

unsigned int genFrameBuffer(){
    //    创建帧缓冲
    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    //    绑定当前的framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    //    创建渲染对象
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    //    创建模版，深度缓冲
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    
    //    将渲染对象绑定到framebuffer上
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    
    //    检查完整性
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    //    解绑
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    return framebuffer;
}

unsigned int bindFrameBufferWithTexture(unsigned int framebuffer){
    //    创建texture
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    unsigned int textureColorbuffer;
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    //    将texture绑定到texture上
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    return textureColorbuffer;
}

unsigned int bindFrameBufferWithTextureSingleChanal(unsigned int framebuffer){
    const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

    GLuint depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                 SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    //    不包含颜色缓冲，只是包含深度信息,必须这样设置来让帧缓冲有效
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    return depthMap;
}

int main(int argc, const char * argv[]) {
    
    // 初始化，
    glfwInit();
    //  指定版本
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //  核心模式
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //  APPLE平台
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    //  窗口
    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Learn", nullptr, nullptr);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if(window==nullptr){
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    glEnable(GL_CULL_FACE);
    glEnable(GL_STENCIL_TEST);
//    glDepthFunc(GL_ALWAYS);
    
    Shader shaderSingle("shader/window.vert","shader/window.frag");
    shaderSingle.use();
    
    Shader shader("shader/test.vert","shader/test.frag");
    shader.use();
    
    Shader noramlShowShader("shader/normalShow.vert","shader/normalShow.frag","shader/normalShow.gs");
    noramlShowShader.use();
    
    Model newModel("Resource/diablo/diablo3_pose.obj");
    newModel.meshes[0].setTexture("Resource/diablo/diablo3_pose_spec.tga","texture_specular");
    newModel.meshes[0].setTexture("Resource/diablo/diablo3_pose_diffuse.tga","texture_diffuse");
    newModel.meshes[0].setTexture("Resource/diablo/diablo3_pose_nm.tga","texture_normal");
    
    Model newModelf("Resource/nanosuit/nanosuit.obj");
    
    
    Shader shaderSky("shader/light.vert","shader/light.frag");
    shaderSky.use();
    unsigned int skyVAO = getSkyBoxVAO();

    glm::vec3 cubePositions[] = {
      glm::vec3( 0.0f,  0.0f,  0.0f),
      glm::vec3( 2.0f,  5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f),
      glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3( 2.4f, -0.4f, -3.5f),
      glm::vec3(-1.7f,  3.0f, -7.5f),
      glm::vec3( 1.3f, -2.0f, -2.5f),
      glm::vec3( 1.5f,  2.0f, -2.5f),
      glm::vec3( 1.5f,  0.2f, -1.5f),
      glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    
//    创建帧缓冲
    unsigned int framebuffer = genFrameBuffer();
//    glGenFramebuffers(1, &framebuffer);
//    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
//
////    创建texture
    unsigned int textureColorbuffer = bindFrameBufferWithTexture(framebuffer);
    unsigned int quadVAO = getQuadVAO();
    factor1 = 0.2f;
    
    
    unsigned int shadowFrameBuffer = genFrameBuffer();
    unsigned int shadowTexture = bindFrameBufferWithTextureSingleChanal(shadowFrameBuffer);
    
    unsigned int planeVAO = getPlaneVAO();
    
    vector<std::string> faces
    {
        "Resource/skyBox/right.jpg",
        "Resource/skyBox/left.jpg",
        "Resource/skyBox/top.jpg",
        "Resource/skyBox/bottom.jpg",
        "Resource/skyBox/front.jpg",
        "Resource/skyBox/back.jpg"
    };
   
    unsigned int cubemapTexture = loadCubemap(faces);
    
    Label label;
    label.init("Resource/arial.ttf", 64);
    
    Mesh mesh;
    auto tex = mesh.getImage("Resource/floor.jpg");
    
    GLuint depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
    
    const GLuint SHADOW_WIDTH = 800, SHADOW_HEIGHT = 600;

    GLuint depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    while(!glfwWindowShouldClose(window))
    {
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        
////        drawScene
        {
            processInput(window);
            glfwSetCursorPosCallback(window, mouse_callback);

            {
                shader.use();
                shader.setVec3("viewDir", cameraPos);
                shader.setFloat("Material.shininess", 16.0f);
                shader.use();

                glPolygonMode(GL_FRONT, GL_FILL);
                
                shader.use();
                setTransform(&shader,glm::vec3( -2.0f,  0.0f,  0.0f),glm::vec3( 11.0f,1.0f,11.0f));
                glBindVertexArray(planeVAO);
                glBindTexture(GL_TEXTURE_2D, tex);
                glActiveTexture(GL_TEXTURE0);
                shader.setInt("texture_diffuse", 0);
                glDrawArrays(GL_TRIANGLES, 0, 6);
                glBindVertexArray(0);
                
                setTransform(&shader,glm::vec3( -2.0f,  1.0f,  0.0f),glm::vec3( 1.1f,  1.1f,  1.1f));
                newModel.Draw(shader);

        //            setTransform(&shader,glm::vec3( 0.0f,  0.5f,  0.0f),glm::vec3( 1.1f,  1.1f,  1.1f));
        //            newModel.Draw(shader);
        //
//                setTransform(&shader,glm::vec3( 2.0f,  0.5f,  0.0f),glm::vec3( 1.1f,  1.1f,  1.1f));
//                newModel.Draw(shader);


                setUpPointLight(&shader, cameraPos, 5.0f*glm::vec3(1.0f,1.0f,1.0f), glm::vec3(1.0f,0.7f,1.8f), 0);
//                setUpDirLight(&shader, glm::vec3(0.0f,-1.0f,0.0f), 1.5f*glm::vec3(1.0f,1.0f,1.0f));

//                setUpSpotLight(&shader, cameraPos, 1.0f*glm::vec3(1.0f,1.0f,1.0f), cameraFront, 0);

                setTransform(&shader,glm::vec3( -2.0f,  0.0f,  0.0f),glm::vec3( 0.101f,  0.101f,  0.101f));
//                newModelf.Draw(shader);
//
//                noramlShowShader.use();
//                setTransform(&noramlShowShader,glm::vec3( -2.0f,  0.0f,  0.0f),glm::vec3( 0.101f,  0.101f,  0.101f));
//                noramlShowShader.setFloat("time", glfwGetTime());
//                newModelf.Draw(noramlShowShader);

                shader.setBool("isNormalTextureMap", factor2==1);

                noramlShowShader.use();
                setTransform(&noramlShowShader,glm::vec3( 2.0f,  0.5f,  0.0f),glm::vec3( 1.1f,  1.1f,  1.1f));
                noramlShowShader.setFloat("time", glfwGetTime());
                newModel.Draw(noramlShowShader);
                
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                label.RenderText("asdfasdf", 30, 30, 1, glm::vec3(1.0f,0.0f,0.0f));
                glDisable(GL_BLEND);
            }

            {
                glDepthFunc(GL_LEQUAL); // set depth function back to default
                shaderSky.use();
                glm::mat4 view          = glm::mat4(1.0f);
                glm::mat4 projection    = glm::mat4(1.0f);

                view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
                projection = glm::perspective(glm::radians(fov), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
                view = glm::mat4(glm::mat3(view)); // remove translation from the view matrix
                shaderSky.setMat4("view", view);
                shaderSky.setMat4("projection", projection);

                glBindVertexArray(skyVAO);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
                glDrawArrays(GL_TRIANGLES, 0, 36);
                glBindVertexArray(0);
                glDepthFunc(GL_LESS); // set depth function back to default
            }
        }

        // 第二处理阶段
        glBindFramebuffer(GL_FRAMEBUFFER, 0); // 返回默认
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shaderSingle.use();
        shaderSingle.setFloat("bloomFactor", factor1);
        glBindVertexArray(quadVAO);
        glDisable(GL_DEPTH_TEST);
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    
    return 0;
}

