//
// Created by someone on 2019/12/27.
//

#include "Model.h"

bool Model::readFromFile(const char *fileName) {
    std::string line;
    std::ifstream in;
    in.open(fileName, std::ios::in);
    if (!in.is_open()) {
        in.close();
        std::cerr << "can't open file " << fileName << "\n";
        return false;
    }
    while (!in.eof()) {
        std::getline(in, line);
        std::stringstream str(line);
        char trash;
        if(!line.compare(0,2,"v ")){
            float x,y,z;
            str>>trash>>x>>y>>z;
            verts.push_back(vec4f(x, y, z, 1.));
        } else if (!line.compare(0,2,"f ")){
            str>>trash;

            std::vector<int> val;
            std::vector<int> uvval;
            int ival,iuvval, itrash;
            char ctrash;
            while (str>>ival>>ctrash>>iuvval>>ctrash>>itrash) {
                val.push_back(ival-1);
                uvval.push_back(iuvval-1);
            }
            faces.push_back(val);
            uvs.push_back(uvval);
        } else if (!line.compare(0,3,"vt ")){
            float x,y,z;
            str>>trash>>trash>>x>>y>>z;
            uvverts.push_back(vec4f(x, y, z, 0.));
        } else if (!line.compare(0,3,"vn ")){
            float x,y,z;
            str>>trash>>trash>>x>>y>>z;
            normals.push_back(vec4f(x, y, z, 0.).getNor());
        }
    }

    return true;
}

Model::Model() {

}

Model::Model(const char *fileName) {

}

