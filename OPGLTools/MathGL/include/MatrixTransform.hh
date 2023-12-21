#ifndef MatrixTransform_HPP
#define MatrixTransform_HPP

#include "Matrix4.hh"
#include "Vector3.hh"



template <typename T>
_Matrix4<T> Translate(_Matrix4<T> mat, Vector3 vec){
    _Matrix4<T> transf = _Matrix4<T>(1, 0, 0, vec.x,
                          0, 1, 0, vec.y,
                          0, 0, 1, vec.z,
                          0, 0, 0, 1);

    return mat * transf;

};


template <typename T>
_Matrix4<T> Rotate(_Matrix4<T> mat, float angle, Vector3 axis){
    float a = angle;
    float c = cos(a);
    float s = sin(a);

    Vector3 tmp = Vector3(axis * (1.0f - c));

    _Matrix4<T> rot;
    rot[0][0] = c + tmp[0] * axis[0];
    rot[0][1] = tmp[0] * axis[1] + s * axis[2];
    rot[0][2] = tmp[0] * axis[2] - s * axis[1];

    rot[1][0] = tmp[1] * axis[0] - s * axis[2];
    rot[1][1] = c + tmp[1] * axis[1];
    rot[1][2] = tmp[1] * axis[2] + s * axis[0];

    rot[2][0] = tmp[2] * axis[0] + s * axis[1];
    rot[2][1] = tmp[2] * axis[1] - s * axis[0];
    rot[2][2] = c + tmp[2] * axis[2];

    _Matrix4<T> result;
    result[0] = mat[0] * rot[0][0] + mat[1] * rot[0][1] + mat[2] * rot[0][2];
    result[1] = mat[0] * rot[1][0] + mat[1] * rot[1][1] + mat[2] * rot[1][2];
    result[2] = mat[0] * rot[2][0] + mat[1] * rot[2][1] + mat[2] * rot[2][2];
    result[3] = mat[3];
    return result;
};


template <typename T>
_Matrix4<T> Scale(_Matrix4<T> mat, Vector3 scale){
    _Matrix4<T> result;
    result[0] = mat[0] * scale[0];
    result[1] = mat[1] * scale[1];
    result[2] = mat[2] * scale[2];
    result[3] = mat[3];
    return result;
};


#endif