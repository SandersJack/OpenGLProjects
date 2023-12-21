#include "MatrixTransform.hh"

Matrix4 translate(Matrix4 mat, Vector3 vec) {
    Matrix4 transf = Matrix4(1, 0, 0, vec.x,
                          0, 1, 0, vec.y,
                          0, 0, 1, vec.z,
                          0, 0, 0, 1);

    return mat * transf;

}

Matrix4 Rotate(Matrix4 mat, float angle, Vector3 axis){
    float a = angle;
    float c = cos(a);
    float s = sin(a);

    Vector3 tmp = Vector3(axis * (1.0f - c));

    Matrix4 rot;
    rot[0][0] = c + tmp[0] * axis[0];
    rot[0][1] = tmp[0] * axis[1] + s * axis[2];
    rot[0][2] = tmp[0] * axis[2] - s * axis[1];

    rot[1][0] = tmp[1] * axis[0] - s * axis[2];
    rot[1][1] = c + tmp[1] * axis[1];
    rot[1][2] = tmp[1] * axis[2] + s * axis[0];

    rot[2][0] = tmp[2] * axis[0] + s * axis[1];
    rot[2][1] = tmp[2] * axis[1] - s * axis[0];
    rot[2][2] = c + tmp[2] * axis[2];

    Matrix4 result;
    result[0] = mat[0] * rot[0][0] + mat[1] * rot[0][1] + mat[2] * rot[0][2];
    result[1] = mat[0] * rot[1][0] + mat[1] * rot[1][1] + mat[2] * rot[1][2];
    result[2] = mat[0] * rot[2][0] + mat[1] * rot[2][1] + mat[2] * rot[2][2];
    result[3] = mat[3];
    return result;
}

