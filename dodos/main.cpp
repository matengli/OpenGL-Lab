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

unsigned int* getImage(std::string imgName){
    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    // S轴wrap模式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    // T轴wrap模式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    // 设置纹理过滤
    // 一个常见的错误是，将放大过滤的选项设置为多级渐远纹理过滤选项之一。
    // 这样没有任何效果，因为多级渐远纹理主要是使用在纹理被缩小的情况下的：
    // 纹理放大不会使用多级渐远纹理，为放大过滤设置多级渐远纹理的选项会产生一个GL_INVALID_ENUM错误代码。
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
//    "Resource/diablo3_pose_diffuse.tga"
    unsigned short *data = stbi_load_16(imgName.c_str(), &width, &height, &nrChannels, 0);
    if(data){
        // 纹理目标，mipmap级别，存储格式，宽度，高度，0（历史问题）,图像数据
        glad_glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_SHORT,data);
        // 自动创建mipmaps
        glad_glGenerateMipmap(GL_TEXTURE_2D);
    }
    // 释放对应内存
    stbi_image_free(data);
    
    return &texture1;
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
    shader->setFloat((str+".cutOff").c_str(),glm::cos(glm::radians(10.0f)));
    
    shader->setFloat((str+".constant").c_str(), 1.0f);
    shader->setFloat((str+".linear").c_str(), 0.09);
    shader->setFloat((str+".quadratic").c_str(), 0.032);
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
    
    glEnable(GL_DEPTH_TEST);
    
    Shader shader("shader/test.vert","shader/test.frag");
    shader.use();
    
    Model newModel("Resource/diablo/diablo3_pose.obj");
    newModel.meshes[0].setTexture("Resource/diablo/diablo3_pose_diffuse.tga","texture_diffuse");
    newModel.meshes[0].setTexture("Resource/diablo/diablo3_pose_spec.tga","texture_specular");
    newModel.meshes[0].setTexture("Resource/diablo/diablo3_pose_nm.tga","texture_normal");

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

    while(!glfwWindowShouldClose(window))
    {
//        input
        processInput(window);
        
        glfwSetCursorPosCallback(window, mouse_callback);
        
        shader.use();
        shader.setVec3("viewDir", cameraPos);
        shader.setFloat("Material.shininess", 64.0f);
        shader.use();
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glPolygonMode(GL_FRONT, GL_FILL);
        
        newModel.Draw(shader);
        setTransform(&shader,glm::vec3( 0.0f,  0.0f,  0.0f),glm::vec3( 1.1f,  1.1f,  1.1f));
        
        setUpDirLight(&shader, glm::vec3(1.0f,1.0f,1.0f), 0.3f*glm::vec3(1.0f,1.0f,1.0f));
        
        setUpSpotLight(&shader, cameraPos, glm::vec3(11.5f,11.5f,11.5f), cameraFront, 0);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    
    return 0;
}

