#include "MathGL.hh"


Matrix4 Ortho(float left, float right, float bottom, float top, float zNear, float zFar){
    /// Creation of orthographic projection matrix
    
    Matrix4 orthores = Matrix4(2/(right - left), 0,              0,                -(right+left)/(right-left),
                            0,                2/(top-bottom), 0,                -(top+bottom)/(top-bottom),
                            0,                0,             -2/(zFar - zNear), -(zFar+zNear)/(zFar-zNear),
                            0,                0,              0,                1);

    return orthores;
}


Matrix4 Perspective(float fovy, float aspect, float near, float far) {
    float tanHalfFovy = tan((fovy) / 2.0f);
    float f = 1.0f / tanHalfFovy;

    Matrix4 result = Matrix4(f / aspect, 0.f,   0.f,                                 0.f,
                             0.f,        f,     0.f,                                 0.f,
                             0.f,        0.f,   (far + near) / (near - far),        (2.0f * far * near) / (near - far),
                             0.f,        0.f,   -1.0f,                               0.0f);

    return result;
}

Matrix4 LookAt(const Vector3& eye, const Vector3& center, const Vector3& up) {
    Vector3 f = (center - eye).normalize(); // Forward
    Vector3 r = (f.cross(up)).normalize(); // Right
    Vector3 tmp = up;
    Vector3 u = tmp.normalize(); //f.cross(r); // Up
    Matrix4 result = Matrix4(r.x,         u.x,         -f.x,        -r.dot(eye),
                             r.y,         u.y,         -f.y,        -u.dot(eye),
                             r.z,         u.z,         -f.z,         f.dot(eye),
                             0.0f,        0.0f,         0.0f,        1.0f);

    return result;
}  