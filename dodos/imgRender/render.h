//
// Created by someone on 2020/1/14.
//

#ifndef IMGRENDER_RENDER_H
#define IMGRENDER_RENDER_H

#include "Geomotry.h"
//#include "tgaimage.h"
#include "Model.h"
#include <algorithm>

#define REG(x) (x/180.*PI)
#define SCREENVEC2(_p) (vec2i((_p.x + 1.) * (WIDTH / 2.), (_p.y + 1.) * (HEIGHT / 2.)))

class Ishader{
public:
    Ishader(){}

    virtual vec4f vertex_shader(int face,int index)=0;

    virtual bool fragment_shader(vec4f batcoord)=0;

    virtual ~Ishader(){}

protected:
    void setupVertAttribute(int face,int index);
};

class normalShader: public Ishader{
public:
    normalShader(){}

    virtual vec4f vertex_shader(int face,int index);

    virtual bool fragment_shader(vec4f batcoord);

};

class zBufferShader: public Ishader{
public:
    zBufferShader(){}

    virtual vec4f vertex_shader(int face,int index);

    virtual bool fragment_shader(vec4f batcoord);

};

class renderer{

public:

    renderer(){}

    void init(TGAImage& image);

    vec4f getBaryCoord(vec2i* points, vec2i &endp);

    void drawTriangle(TGAImage &image);

    void render(TGAImage& image);

    void drawZbuffer(TGAImage& image);
};

#endif //IMGRENDER_RENDER_H
