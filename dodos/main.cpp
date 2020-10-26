//
//  main.cpp
//  dodos
//
//  Created by 马腾理 on 2020/10/11.
///Users/matengli/Desktop/matengli/tempRes/未命名文件夹 3/glfw-3.3.2.bin.MACOS/include/GLFW/glfw3.h

#include "glad/glad.h"
#include "glfw3.h"
#include "math.h"
#include "imgRender/Model.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader/shader.hpp"
#include "stb_image.h"

#include <iostream>

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

void processInput(GLFWwindow *window,float* x,float* y,float* scale)
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
//    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//        *x = *x-0.1;
//    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//        *x = *x+0.1;
//    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//    {
//        *y = *y-0.01;
//        std::cout << "y:"<<*y << std::endl;
//    }
//    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//    {
//        *y = *y+0.01;
//        std::cout << "y:"<<*y << std::endl;
//    }
//    if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
//        *scale = *scale+0.001;
//    if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
//        *scale = *scale-0.001;
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
    
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    
    //  VBO
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    
    // VAO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    // Texture
    unsigned int texture1;
    unsigned int texture2;
    // 生成数量，对应地址
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
    
    //  读取图片
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char *data = stbi_load("Resource/diablo3_pose_diffuse.tga", &width, &height, &nrChannels, 0);
    if(data){
        // 纹理目标，mipmap级别，存储格式，宽度，高度，0（历史问题）,图像数据
        glad_glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
        // 自动创建mipmaps
        glad_glGenerateMipmap(GL_TEXTURE_2D);
    }
    // 释放对应内存
    stbi_image_free(data);
    
    // 生成数量，对应地址
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    
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
    
    //  读取图片
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    data = stbi_load("Resource/diablo3_pose_diffuse.tga", &width, &height, &nrChannels, 0);
    if(data){
        // 纹理目标，mipmap级别，存储格式，宽度，高度，0（历史问题）,图像数据
        glad_glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
        // 自动创建mipmaps
        glad_glGenerateMipmap(GL_TEXTURE_2D);
    }
    // 释放对应内存
    stbi_image_free(data);

    // 1. 绑定VAO
    glBindVertexArray(VAO);
    
    Model * model = new Model();
    model->readFromFile("Resource/diablo3_pose.obj");

    float vertices[model->verts.size()*8];

    for(int i=0; i < model->verts.size(); i++) {
        vertices[i*8+0] = model->verts[i].x;
        vertices[i*8+1] = model->verts[i].y;
        vertices[i*8+2] = model->verts[i].z;

        vertices[i*8+3] = 1.0f;
        vertices[i*8+4] = 1.0f;
        vertices[i*8+5] = 1.0f;
    }
////
    unsigned int indices[model->getFacesCount()*3];
    for(int i=0; i < model->getFacesCount(); i++) {
        indices[i*3+0] = model->faces[i][0];
        indices[i*3+1] = model->faces[i][1];
        indices[i*3+2] = model->faces[i][2];
    }

    for(int i=0; i < model->getFacesCount(); i++) {
        int vid[] = {model->faces[i][0],model->faces[i][1],model->faces[i][2]};
        
        vec4f v1 = model->verts[vid[0]]-model->verts[vid[1]];
        vec4f v2 = model->verts[vid[2]]-model->verts[vid[1]];
        
        vec4f f = (v2^v1).getNor();

        for(int j=0; j < 3; j++) {
            int id = i*3+j;
            int tid = model->faces[i][j];
            vec4f uvId = model->getUvVecs(i,j);
            
            vertices[tid*8+6] = uvId.x;
            vertices[tid*8+7] = uvId.y;
            
            vertices[vid[j]*8+3] = f.x;
            vertices[vid[j]*8+4] = f.y;
            vertices[vid[j]*8+5] = f.z;
        }
    }
    
    int counts = model->getFacesCount();
    
//    float vertices[] = {
//    //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
//         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
//         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
//        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
//        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
//    };
//
//    unsigned int indices[] = {
//        0, 1, 3, // first triangle
//        1, 2, 3  // second triangle
//    };
    
    // 2. 把VBO,EBO复制到缓冲中供OpenGL使用
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);
    
    // 3. 设置顶点属性指针
    //（layout的取值,对应属性的大小(几个数),数据类型,是否标准化,步长，偏移量)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);
    
    
    // 绑定贴图
    glBindTexture(GL_TEXTURE_2D, texture1);
    glBindVertexArray(VAO);
    
    Shader* shader = new Shader("shader/test.vert","shader/test.frag");
    shader->use();
    
    shader->setInt("texture1", 0); // 设置贴图
    shader->setInt("texture2", 1); // 设置贴图
    
    float x = 0.0f;
    float y = 0.0f;
    float scale = 1.0f;
    
    glEnable(GL_DEPTH_TEST);
    
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
        processInput(window,&x,&y,&scale);
        
        glfwSetCursorPosCallback(window, mouse_callback);
        
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glBindVertexArray(VAO);
        for(unsigned int i = 0; i < 10; i++)
        {

          glDrawArrays(GL_TRIANGLES, 0, 36);
        
            // 多个纹理需要激活纹理单元
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture1);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texture2);
            
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 3*counts, GL_UNSIGNED_INT, 0);
    //        线框模式绘制
//            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //        默认模式绘制
            glPolygonMode(GL_FRONT, GL_FILL);

            float time = glfwGetTime();
            float light = abs(sin(time));
    //        int loc = glad_glGetUniformLocation(shaderProgram,"ourColorFactor");
    //
    //     需要注意的是：查询uniform地址不要求你之前使用过着色器程序，
    //     但是更新一个uniform之前你必须先使用程序（调用glUseProgram)，
    //     因为它是在当前激活的着色器程序中设置uniform的。
    //     说是这么说。。。。。。我直接使用也是ok的
    //        glad_glUniform1f(loc,light);
    //        glad_glUseProgram(shaderProgram);
            shader->setFloat("ourColorFactor", light);
            shader->use();
            
            // create transformations
            glm::mat4 model         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
            glm::mat4 view          = glm::mat4(1.0f);
            glm::mat4 projection    = glm::mat4(1.0f);
            
//        OLD
//            model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//            view  = glm::translate(view, glm::vec3(x, y, -3.0f));
//            projection = glm::perspective(glm::radians(0.0f), (float)screenWidth / (float)screenHeight, 0.1f, 40.0f);
            
            
//            NEW
//            Eye,Center,Up
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::scale(model, glm::vec3(scale,scale,scale));
            
            view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
            // pass projection matrix to shader (note that in this case it could change every frame)
            projection = glm::perspective(glm::radians(fov), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
            
    ////      TRANSLATION
            glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//            transform = glm::scale(transform, glm::vec3(scale,scale,scale));
//            transform = glm::translate(transform, glm::vec3(cubePositions[i]));
//            transform = glm::rotate(transform, scale, glm::vec3(0.0f, 1.0f, 0.0f));
    ////      TRANSLATION

            unsigned int transformLoc = glGetUniformLocation(shader->ID, "model");
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model));
            
            transformLoc = glGetUniformLocation(shader->ID, "view");
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(view));
            
            transformLoc = glGetUniformLocation(shader->ID, "projection");
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(projection));
            
            transformLoc = glGetUniformLocation(shader->ID, "transform");
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
            
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    
    return 0;
}
