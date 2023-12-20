#include "Shader.hh"
#include "LoadShaders.hh"

Shader::Shader(){

}

void Shader::Compile(const char * vertex_file_path,const char * fragment_file_path){
    fID = LoadShaders(vertex_file_path, fragment_file_path);
}