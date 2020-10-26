//
// Created by someone on 2019/12/29.
//
#ifndef IMGRENDER_GEOMOTRY_H
#define IMGRENDER_GEOMOTRY_H

# include <vector>
# include <math.h>

struct vec4f {
    float x;
    float y;
    float z;
    float w;

    vec4f() : x(0.), y(0.), z(0.), w(0.) {

    }

    vec4f(float x, float y, float z, float w) : x(0.), y(0.), z(0.), w(0.) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }

    inline float getLength(){return sqrt(x*x+y*y+z*z);}
    inline vec4f getNor(){float l = getLength();return vec4f(x / l, y / l, z / l, w);}
    inline vec4f operator +(const vec4f &v) { return vec4f(x+v.x,y+v.y,z+v.z,w+v.w);}
    inline vec4f operator -(const vec4f &v) { return vec4f(x-v.x,y-v.y,z-v.z,w-v.w);}
    inline float operator *(const vec4f &v) { return x*v.x+y*v.y+z*v.z+w*v.w;}
    inline vec4f operator *(float v) { return vec4f(x*v,y*v,z*v,w*v);}
    inline vec4f operator ^(const vec4f &v) { return vec4f(y*v.z-z*v.y,z*v.x-x*v.z,x*v.y-y*v.x,0.);}
    inline float operator [](unsigned int i) { return (i==0)?x:(i==1)?y:(i==2)?z:(i==3)?w:1.0;}

};

struct vec2i {
    int x;
    int y;

    vec2i() : x(0.), y(0.) {

    }

    vec2i(int x, int y) : x(0.), y(0.) {
        this->x = x;
        this->y = y;
    }

    inline vec2i operator -(const vec2i &v) {
        return vec2i(x-v.x,y-v.y);
    }

    inline vec2i operator +(const vec2i &v) { return vec2i(x+v.x,y+v.y);}

};


const int DEFAULT_ALLOC=4;

class Matrix {
    std::vector<std::vector<float> > m;
    int rows, cols;
public:
    Matrix(int r=DEFAULT_ALLOC, int c=DEFAULT_ALLOC);
    Matrix(vec4f vals);

    inline int nrows();
    inline int ncols();

    static Matrix identity(int dimensions);
    std::vector<float>& operator[](const int i);
    Matrix operator*(const Matrix& a);
    Matrix transpose();
    Matrix inverse();

    vec4f getVec4f();
    vec2i getVec2i();

        static Matrix lookAt(vec4f center,vec4f eye,vec4f up);
    friend std::ostream& operator<<(std::ostream& s, Matrix& m);
};

#endif //IMGRENDER_GEOMOTRY_H
