//
//  Mesh.hpp
//  dodos
//
//  Created by 马腾理 on 2020/12/10.
//

#ifndef Mesh_hpp
#define Mesh_hpp

#include <stdio.h>
#include "glad/glad.h"
#include "glfw3.h"
#include "math.h"
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader/shader.hpp"
#include "stb_image.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace std;

struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
};

struct Texture {
    unsigned int id;
    string type;
    string path;
};

class Mesh{
public:
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
    void Draw(Shader& shader);
    void setTexture(string path,string typeName);
    
private:
    /*  渲染数据  */
    unsigned int VAO, VBO, EBO;
    /*  函数  */
    void setupMesh();
    unsigned int getImage(string path);
};

#endif /* Mesh_hpp */
