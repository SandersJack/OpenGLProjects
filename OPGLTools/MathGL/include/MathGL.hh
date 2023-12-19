#ifndef MathGL_HPP
#define MathGL_HPP

#include "Constants.hh"
#include "Trigonometry.hh"
#include "Matrix3.hh"
#include "Matrix4.hh"
#include "Vector2.hh"
#include "Vector3.hh"
#include "Vector4.hh"
#include "MatrixTransform.hh"

Matrix4 Ortho(float left, float right, float bottom, float top, float zNear=-1.0f, float zFar=1.0f);

Matrix4 Perspective(float fovy, float aspect, float near, float far);

Matrix4 LookAt(const Vector3& eye, const Vector3& center, const Vector3& up);

#endif