#ifndef MatrixTransform_HPP
#define MatrixTransform_HPP

#include "Matrix4.hh"
#include "Vector3.hh"

Matrix4 translate(Matrix4 mat, Vector3 vec);

Matrix4 Rotate(Matrix4 mat, float angle, Vector3 axis);

#endif