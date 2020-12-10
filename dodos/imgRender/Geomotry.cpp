//
// Created by someone on 2020/1/2.
//

#include "Geomotry.h"
#include <iostream>

Matrix::Matrix(int r, int c) : m(std::vector<std::vector<float> >(r, std::vector<float>(c, 0.f))), rows(r), cols(c) { }
Matrix::Matrix(vec4f vals):m(std::vector<std::vector<float> >(4, std::vector<float>(1, 0.f))), rows(4), cols(1){
    for(int i=0;i<4;i++){
        m[i][0] = vals[i];
    }
}

int Matrix::nrows() {
    return rows;
}

int Matrix::ncols() {
    return cols;
}

Matrix Matrix::identity(int dimensions) {
    Matrix E(dimensions, dimensions);
    for (int i=0; i<dimensions; i++) {
        for (int j=0; j<dimensions; j++) {
            E[i][j] = (i==j ? 1.f : 0.f);
        }
    }
    return E;
}

std::vector<float>& Matrix::operator[](const int i) {
    assert(i>=0 && i<rows);
    return m[i];
}

Matrix Matrix::operator*(const Matrix& a) {
    assert(cols == a.rows);
    Matrix result(rows, a.cols);
    for (int i=0; i<rows; i++) {
        for (int j=0; j<a.cols; j++) {
            result.m[i][j] = 0.f;
            for (int k=0; k<cols; k++) {
                result.m[i][j] += m[i][k]*a.m[k][j];
            }
        }
    }
    return result;
}

Matrix Matrix::transpose() {
    Matrix result(cols, rows);
    for(int i=0; i<rows; i++)
        for(int j=0; j<cols; j++)
            result[j][i] = m[i][j];
    return result;
}

Matrix Matrix::inverse() {
    assert(rows==cols);
    // augmenting the square matrix with the identity matrix of the same dimensions a => [ai]
    Matrix result(rows, cols*2);
    for(int i=0; i<rows; i++)
        for(int j=0; j<cols; j++)
            result[i][j] = m[i][j];
    for(int i=0; i<rows; i++)
        result[i][i+cols] = 1;
    // first pass
    for (int i=0; i<rows-1; i++) {
        // normalize the first row
        for(int j=result.cols-1; j>=0; j--)
            result[i][j] /= result[i][i];
        for (int k=i+1; k<rows; k++) {
            float coeff = result[k][i];
            for (int j=0; j<result.cols; j++) {
                result[k][j] -= result[i][j]*coeff;
            }
        }
    }
    // normalize the last row
    for(int j=result.cols-1; j>=rows-1; j--)
        result[rows-1][j] /= result[rows-1][rows-1];
    // second pass
    for (int i=rows-1; i>0; i--) {
        for (int k=i-1; k>=0; k--) {
            float coeff = result[k][i];
            for (int j=0; j<result.cols; j++) {
                result[k][j] -= result[i][j]*coeff;
            }
        }
    }
    // cut the identity matrix back
    Matrix truncate(rows, cols);
    for(int i=0; i<rows; i++)
        for(int j=0; j<cols; j++)
            truncate[i][j] = result[i][j+cols];
    return truncate;
}

vec4f Matrix::getVec4f() {
    assert(cols==1&&rows==4);
    float w = m[3][0]==0.?1.:m[3][0];
    return vec4f(m[0][0]/w,m[1][0]/w,m[2][0]/w,m[3][0]==0.?0.:1.);
}

vec2i Matrix::getVec2i() {
    assert(cols==1&&rows==4);
    float w = m[3][0]==0.?1.:m[3][0];
    return vec2i(m[0][0]/w,m[1][0]/w);
}

Matrix Matrix::lookAt(vec4f center,vec4f eye,vec4f up){
    Matrix mMat = Matrix::identity(4);
    Matrix rMat = Matrix::identity(4);
    vec4f zAxis = (eye - center).getNor();
//    vec4f xAxis = (zAxis ^ up).getNor();
    vec4f xAxis = (up^zAxis).getNor();
//    vec4f yAxis = (xAxis ^ zAxis).getNor();
    vec4f yAxis = (zAxis^xAxis).getNor();

    vec4f axise[3] = {xAxis,yAxis,zAxis};
    for(int i=0;i<3;i++){
        rMat[0][i] = xAxis[i];
        rMat[1][i] = yAxis[i];
        rMat[2][i] = zAxis[i];

        mMat[i][3] = -center[i];
    }
    return rMat*mMat;
}

std::ostream& operator<<(std::ostream& s, Matrix& m) {
    for (int i=0; i<m.nrows(); i++)  {
        for (int j=0; j<m.ncols(); j++) {
            s << m[i][j];
            if (j<m.ncols()-1) s << "\t";
        }
        s << "\n";
    }
    return s;
}
