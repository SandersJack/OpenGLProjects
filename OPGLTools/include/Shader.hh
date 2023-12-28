#ifndef Shader_HPP
#define Shader_HPP

#include "MathGL.hh"
class Shader {

    public:
        Shader();

        Shader  &Use();
        void Compile(const char * vertex_file_path,const char * fragment_file_path);

        unsigned int GetID() {return fID;}
        const unsigned int* value_ptr() const {
            return &fID;
        }

        // utility functions
        void    SetFloat    (const char *name, float value, bool useShader = false);
        void    SetInteger  (const char *name, int value, bool useShader = false);
        void    SetVector2f (const char *name, float x, float y, bool useShader = false);
        void    SetVector2f (const char *name, const Vector2 &value, bool useShader = false);
        void    SetVector3f (const char *name, float x, float y, float z, bool useShader = false);
        void    SetVector3f (const char *name, const Vector3 &value, bool useShader = false);
        void    SetVector4f (const char *name, float x, float y, float z, float w, bool useShader = false);
        void    SetVector4f (const char *name, const Vector4 &value, bool useShader = false);
        void    SetMatrix4  (const char *name, const Matrix4 &matrix, bool useShader = false);

    private:
        unsigned int fID;  

};

#endif