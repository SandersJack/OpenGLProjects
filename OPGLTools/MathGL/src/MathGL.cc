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
    float tanHalfFovy = tan(Radians(fovy) / 2.0f);
    float f = 1.0f / tanHalfFovy;

    Matrix4 result = Matrix4(f / aspect, 0.f,   0.f,                                 0.f,
                             0.f,        f,     0.f,                                 0.f,
                             0.f,        0.f,   (far + near) / (near - far),        -1.0f,
                             0.f,        0.f,   (2.0f * far * near) / (near - far) , 0.0f);

    return result;
}

Matrix4 LookAt(const Vector3& eye, const Vector3& center, const Vector3& up) {
    Vector3 f = (center - eye).normalize(); // Forward
    Vector3 r = (up.cross(f)).normalize(); // Right
    Vector3 u = f.cross(r); // Up

    Matrix4 result = Matrix4(r.x,         u.x,         -f.x,        0.0f,
                             r.y,         u.y,         -f.y,        0.0f,
                             r.z,         u.z,         -f.z,        0.0f,
                             -r.dot(eye), -u.dot(eye), -f.dot(eye), 1.0f);

    return result;
}