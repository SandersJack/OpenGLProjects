#include "Shader.hh"
#include "LoadShaders.hh"

Shader::Shader(){

}

Shader &Shader::Use()
{
    glUseProgram(fID);
    return *this;
}

void Shader::Compile(const char * vertex_file_path,const char * fragment_file_path){
    fID = LoadShaders(vertex_file_path, fragment_file_path);
}

void Shader::SetFloat(const char *name, float value, bool useShader)
{
    if (useShader)
        this->Use();
    glUniform1f(glGetUniformLocation(fID, name), value);
}
void Shader::SetInteger(const char *name, int value, bool useShader)
{
    if (useShader)
        this->Use();
    glUniform1i(glGetUniformLocation(fID, name), value);
}
void Shader::SetVector2f(const char *name, float x, float y, bool useShader)
{
    if (useShader)
        this->Use();
    glUniform2f(glGetUniformLocation(this->fID, name), x, y);
}
void Shader::SetVector2f(const char *name, const Vector2 &value, bool useShader)
{
    if (useShader)
        this->Use();
    glUniform2f(glGetUniformLocation(fID, name), value.x, value.y);
}
void Shader::SetVector3f(const char *name, float x, float y, float z, bool useShader)
{
    if (useShader)
        this->Use();
    glUniform3f(glGetUniformLocation(fID, name), x, y, z);
}
void Shader::SetVector3f(const char *name, const Vector3 &value, bool useShader)
{
    if (useShader)
        this->Use();
    glUniform3f(glGetUniformLocation(fID, name), value.x, value.y, value.z);
}
void Shader::SetVector4f(const char *name, float x, float y, float z, float w, bool useShader)
{
    if (useShader)
        this->Use();
    glUniform4f(glGetUniformLocation(fID, name), x, y, z, w);
}
void Shader::SetVector4f(const char *name, const Vector4 &value, bool useShader)
{
    if (useShader)
        this->Use();
    glUniform4f(glGetUniformLocation(fID, name), value.x, value.y, value.z, value.w);
}
void Shader::SetMatrix4(const char *name, const Matrix4 &matrix, bool useShader)
{
    if (useShader)
        this->Use();
    glUniformMatrix4fv(glGetUniformLocation(fID, name), 1, false, matrix.value_ptr());
}