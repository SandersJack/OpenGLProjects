#include "MatrixTransform.hh"

Matrix4 translate(Matrix4 mat, Vector3 vec) {
    Matrix4 transf = Matrix4(1, 0, 0, vec.x,
                          0, 1, 0, vec.y,
                          0, 0, 1, vec.z,
                          0, 0, 0, 1);

    return mat * transf;

}

