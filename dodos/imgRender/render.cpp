//
// Created by someone on 2020/1/14.
//

#include "render.h"

vec4f LIGHTDIR = vec4f(1,1,0,0).getNor();
const float LIGHT_INST = 2.;

Model* model;

vec4f CENTER = vec4f(0,0,0,0);
vec4f EYE = vec4f(0,0.,1.0,0);
vec4f UP = vec4f(0.,1.,0,0);
int WIDTH;
int HEIGHT;
Matrix mat_mv;
Matrix mat_proj;
Matrix mat_mvp;
Matrix mat_view;

Matrix mat_shadow;

vec4f varying_vert[3];
vec4f varying_norm[3];
vec4f varying_uvs[3];

vec2i screen_verts[3];

TGAColor varying_frag_color;

Ishader* shader;

float* zBuffer;

TGAColor ambit = TGAColor(5,5,5,5);

void renderer::init(TGAImage& image){
    WIDTH = image.get_width();
    HEIGHT = image.get_height();
    model =new Model();
    zBuffer = new float[WIDTH*HEIGHT];
    shader = new zBufferShader();

    for(int i=0;i<WIDTH*HEIGHT;i++){
        zBuffer[i]=-std::numeric_limits<float>::max();
    }
    model->readFromFile("Resource/diablo3_pose.obj");
    model->readDiffTextureFromFile("Resource/diablo3_pose_diffuse.tga");
    model->readNormalFromFile("Resource/diablo3_pose_nm.tga");
    model->readSpecFromFile("Resource/diablo3_pose_spec.tga");

    render(image);
    delete model;
    delete [] zBuffer;
    delete shader;
}

vec4f renderer::getBaryCoord(vec2i* points, vec2i &endp){
    vec2i a = points[0]-points[2];
    vec2i b = points[1]-points[2];
    vec2i c = endp-points[2];

    float x = (c.x*b.y-c.y*b.x)/(float)(a.x*b.y-a.y*b.x);
    float y = (c.x*a.y-c.y*a.x)/(float)(b.x*a.y-b.y*a.x);
    vec4f result(x,y,1.0-x-y,0);
    if ((result.x>=0.&&result.x<=1.)&&(result.y>=0.&&result.y<=1.)&&(result.z>=0.&&result.z<=1.)){
        return result;
    } else{
        return vec4f(-1.,-1.,-1.,0);
    }
}

void renderer::drawTriangle(TGAImage &image){
    for(int i=0;i<3;i++) screen_verts[i] = (mat_view*Matrix(varying_vert[i])).getVec2i();
    int xyMinMax[4] = {screen_verts[0].x, screen_verts[0].x, screen_verts[0].y, screen_verts[0].y};
    for(int i=0;i<4;i++){
        for(int j=0;j<3;j++){
            int tval = i <= 1 ? screen_verts[j].x : screen_verts[j].y;
            bool isUpdate = (i % 2 == 0) ? xyMinMax[i] > tval : xyMinMax[i] < tval;
            xyMinMax[i] = isUpdate ? tval : xyMinMax[i];
        }
    }

    for(int i=0;i<4;i++){
        if(xyMinMax[i]<0||xyMinMax[i]>=WIDTH){
            return;
        }
    }

    for(int x=xyMinMax[0];x<xyMinMax[1];x+=1){
        for(int y=xyMinMax[2];y<xyMinMax[3];y+=1) {
            auto end = vec2i(x,y);
            vec4f result = getBaryCoord(screen_verts, end);
            float zbuf = zBuffer[y*WIDTH+x];
            float z = result*vec4f(varying_vert[0].z, varying_vert[1].z, varying_vert[2].z, 0.);

//            不在三角形内
            if(!(result.x>=0.&&result.y>=0.&&result.z>=0.)) {
                continue;
            }

            if(z>=zbuf){
                zBuffer[y*WIDTH+x] = z;
            } else{
                continue;
            }

            if(shader->fragment_shader(result)){
                image.set(x,y,varying_frag_color);
            }
        }
    }
}

void renderer::drawZbuffer(TGAImage& image){
    float min = std::numeric_limits<float>::max();
    float max = -std::numeric_limits<float>::max();

    for(int i=0;i<WIDTH*HEIGHT;i++){
        float factor = zBuffer[i];
        if(factor==-std::numeric_limits<float>::max()){
            continue;
        }
        min = min>factor?factor:min;
        max = max<factor?factor:max;
    }

    for(int x=0;x<WIDTH;x++){
        for(int y=0;y<HEIGHT;y++){
            float factor = zBuffer[x+y*WIDTH];
            if(factor>-std::numeric_limits<float>::max()){
//                factor*=255.;
                factor = (factor-min)/(max-min)*255.;
                image.set(x,y,TGAColor(factor,factor,factor,255));
            } else{
                image.set(x,y,TGAColor(0,0,0,255));
            }
        }
    }
}

void renderer::render(TGAImage& image){
    TGAImage shadowImg(WIDTH,HEIGHT,TGAImage::RGBA);

    mat_mv = Matrix::lookAt(CENTER,LIGHTDIR,UP);

    mat_proj = Matrix::identity(4);
    mat_proj[3][2] = -0.5;

    mat_mvp = mat_proj*mat_mv;

    mat_view = Matrix::identity(4);
    mat_view[0][0] = WIDTH/2.;
    mat_view[0][3] = WIDTH/2.;
    mat_view[1][0] = HEIGHT/2.;
    mat_view[1][3] = HEIGHT/2.;

    for (int i = 0; i <model->getFacesCount() ; i++) {
        for(int j=0;j<3;j++){
             shader->vertex_shader(i,j);
        }

        drawTriangle(image);
    }

    drawZbuffer(shadowImg);
    shadowImg.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    shadowImg.write_tga_file("output/shadow.tga");
    std::system("open output/shadow.tga");

}

vec4f normalShader::vertex_shader(int face,int index){
    vec4f vert = model->getFaceVecs(face,index);
    varying_vert[index] = (mat_mvp*Matrix(vert)).getVec4f();
    varying_norm[index] = model->getFaceNors(face,index);
    varying_uvs[index] = model->getUvVecs(face,index);
    return varying_vert[index];
}

bool normalShader::fragment_shader(vec4f batcoord){
    vec4f uv = varying_uvs[0] * batcoord.x + varying_uvs[1] * batcoord.y + varying_uvs[2] * batcoord.z;

    vec4f normal = varying_norm[0]*batcoord.x+varying_norm[1]*batcoord.y+varying_norm[2]*batcoord.z;
    normal =  model->getNorFromMapByUv(uv);
    float ins = normal*LIGHTDIR*LIGHT_INST;
    float spec = limit(model->getSpecFromMapByUv(uv),1,255);

    vec4f r = (normal*(normal*LIGHTDIR*2.f) - LIGHTDIR).getNor();   // reflected light
    r.z = r.z>=0. ? r.z:0.;
    float specf = pow(r.z,spec);
    specf= specf>=0. ?specf:0.;

    ins += (specf)*.6;

    if(ins>=0.8){
        ins = 2.0;
    } else if(ins>=0.6){
        ins = 1.0;
    } else if(ins>=0.4){
        ins = 0.7;
    } else if(ins>=0.2){
        ins = 0.5;
    } else{
        ins = 0.2;
    }
    varying_frag_color = model->getDiffuseColorByUv(uv)*(ins)+5;

    return true;
}

vec4f zBufferShader::vertex_shader(int face,int index){
    vec4f vert = model->getFaceVecs(face,index);
    varying_vert[index] = (mat_mvp*Matrix(vert)).getVec4f();
    varying_norm[index] = model->getFaceNors(face,index);
    varying_uvs[index] = model->getUvVecs(face,index);
    return varying_vert[index];
}

bool zBufferShader::fragment_shader(vec4f batcoord){
    return false;
}

