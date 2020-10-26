//
//  shader.cpp
//  dodos
//
//  Created by 马腾理 on 2020/10/14.
//

#include "shader.hpp"

Shader::Shader(const char* vertexPath, const char* fragmentPath){
    std::string vertCode;
    std::string fragCode;
    
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    
    vShaderFile.exceptions(std::ifstream::failbit|std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit|std::ifstream::badbit);
    
    try {
        vShaderFile.open(vertCode);
        fShaderFile.open(vertCode);
        std::stringstream vStream,fStream;
        vStream<<vShaderFile.rdbuf();
        fStream<<fShaderFile.rdbuf();
        
        vShaderFile.close();
        fShaderFile.close();
        
        vertCode   = vStream.str();
        fragCode = fStream.str();
    } catch (std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    
    const char* vShaderCode = vertCode.c_str();
    const char* fShaderCode = fragCode.c_str();
    
    //  编译顶点着色器
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glad_glShaderSource(vertexShader,1,&vShaderCode,nullptr);
    glad_glCompileShader(vertexShader);
    //  错误log
    int success;
    char infoLog[512];
    glad_glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&success);
    if(!success){
        glad_glGetShaderInfoLog(vertexShader,512,nullptr,infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
        
    //  编译片元着色器
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
    glCompileShader(fragmentShader);
    
    //  错误log
    int success2;
    char infoLog2[512];
    glad_glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&success2);
    if(!success2){
        glad_glGetShaderInfoLog(fragmentShader,512,nullptr,infoLog2);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    //    链接
    ID = glad_glCreateProgram();
    glad_glAttachShader(ID,vertexShader);
    glad_glAttachShader(ID,fragmentShader);
    glad_glLinkProgram(ID);
            
    // 打印连接错误（如果有的话）
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // 删除着色器，它们已经链接到我们的程序中了，已经不再需要了
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}


void Shader::use(){
    glad_glUseProgram(ID);
}

//void setBool(const std::string &name, bool value) const
//{
//    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
//}
//void setInt(const std::string &name, int value) const
//{
//    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
//}
//void setFloat(const std::string &name, float value) const
//{
//    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
//}
