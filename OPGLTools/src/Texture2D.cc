#include "Texture2D.hh"

#include <GL/glew.h>


Texture2D::Texture2D()
    : fWidth(0), fHeight(0), fInternal_Format(GL_RGB), fImage_Format(GL_RGB), fWrap_S(GL_REPEAT),
    fWrap_T(GL_REPEAT), fFilter_Min(GL_LINEAR), fFilter_Max(GL_LINEAR)
{
    glGenTextures(1, &this->fID);
}

void Texture2D::Generate(unsigned int width, unsigned int height, unsigned char* data)
{
    this->fWidth = width;
    this->fHeight = height;
    // create Texture
    glBindTexture(GL_TEXTURE_2D, this->fID);
    glTexImage2D(GL_TEXTURE_2D, 0, this->fInternal_Format, width, height, 0, this->fImage_Format, GL_UNSIGNED_BYTE, data);
    // set Texture wrap and filter modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->fWrap_S);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->fWrap_T);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->fFilter_Min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->fFilter_Max);
    // unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Bind() const
{
    glBindTexture(GL_TEXTURE_2D, this->fID);
}