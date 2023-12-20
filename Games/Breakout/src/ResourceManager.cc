#include "ResourceManager.hh"
#include <GL/glew.h>

#include "OPGLTools.hh"

ResourceManager *ResourceManager::fInstance = nullptr;

ResourceManager *ResourceManager::GetInstance() {
    if(!fInstance)
        fInstance = new ResourceManager();
    return fInstance;
}


ResourceManager::ResourceManager(){

}

Shader ResourceManager::LoadShader(const char * vertex_file_path,const char * fragment_file_path, std::string name){
    Shader nShader = Shader();
    nShader.Compile(vertex_file_path, fragment_file_path);
    fShaders[name] = nShader;

    return fShaders[name];
}

Texture2D ResourceManager::LoadTexture(const char *file, bool alpha, std::string name){
    fTextures[name] = LoadTextureFromFile(file, alpha);

    return fTextures[name];
}


Texture2D ResourceManager::LoadTextureFromFile(const char *file, bool alpha) {
    // create texture object
    Texture2D texture;
    if (alpha)
    {
        texture.SetInternal_Format(GL_RGBA);
        texture.SetImage_Format(GL_RGBA);
    }
    // load image
    int width, height, nrChannels;
    unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
    // now generate texture
    texture.Generate(width, height, data);
    // and finally free image data
    stbi_image_free(data);
    return texture;
}