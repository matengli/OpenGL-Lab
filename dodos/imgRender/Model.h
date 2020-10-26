//
// Created by someone on 2019/12/27.
//

#ifndef IMGRENDER_MODEL_H
#define IMGRENDER_MODEL_H

#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include "Geomotry.h"
#include "tgaimage.h"
//#include "GlobalFunctions.h"

class Model {
public:
    Model();

    Model(const char *fileName);

    bool readFromFile(const char *fileName);

    inline vec4f getFaceVecs(int faceIndex, int index) {
        return verts[faces[faceIndex][index]];
    }

    inline vec4f getUvVecs(int uvIndex, int index) {
        return uvverts[uvs[uvIndex][index]];
    }

    inline vec4f getFaceNors(int uvIndex, int index) {
        return normals[faces[uvIndex][index]];
    }

    inline int getFacesCount() {
        return faces.size();
    }

    inline int getPointsCount() {
        return verts.size();
    }

    TGAColor getDiffuseColor(int uvIndex, int index){
        auto uv = getUvVecs(uvIndex, index);
        int width = diffuseTexture.get_width();
        int height = diffuseTexture.get_height();
        int x = (int)(uv.x*width);
        int y = (int)(uv.y*height);
        return diffuseTexture.get(x,y);
    }

    inline TGAColor getDiffuseColorByUv(vec4f uv){
        int width = diffuseTexture.get_width();
        int height = diffuseTexture.get_height();
        return diffuseTexture.get(uv.x*width,uv.y*height);
    }

    inline vec4f getNorFromMapByUv(vec4f uv){
        int width = normalTexture.get_width();
        int height = normalTexture.get_height();
        TGAColor c = normalTexture.get((uv.x*width),(uv.y*height));
        return vec4f(c.r / 255. * 2. - 1., c.g / 255. * 2. - 1., c.b / 255. * 2. - 1., 0.).getNor();
    }

    inline float getSpecFromMapByUv(vec4f uv){
        int width = specTexture.get_width();
        int height = specTexture.get_height();
        TGAColor c = specTexture.get(uv.x*width,uv.y*height);
        return c.r;
    }

    bool readDiffTextureFromFile(const char *fileName){
        diffuseTexture.read_tga_file(fileName);
        diffuseTexture.flip_vertically();
        return true;
    }

    bool readNormalFromFile(const char * fileName,bool isTan = false){
        normalTexture.read_tga_file(fileName);
        normalTexture.flip_vertically();
        return true;
    }

    bool readSpecFromFile(const char * fileName){
        specTexture.read_tga_file(fileName);
        specTexture.flip_vertically();
        return true;
    }

    std::vector<vec4f> verts;
    std::vector<std::vector<int>> faces;

private:
    TGAImage diffuseTexture;
    TGAImage normalTexture;
    TGAImage specTexture;

    std::vector<vec4f> normals;
    std::vector<vec4f> uvverts;

    std::vector<std::vector<int>> uvs;
};


#endif //IMGRENDER_MODEL_H
