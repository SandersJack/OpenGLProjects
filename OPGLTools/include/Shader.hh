#ifndef Shader_HPP
#define Shader_HPP

class Shader {

    public:
        Shader();

        void Compile(const char * vertex_file_path,const char * fragment_file_path);

    private:
        unsigned int fID;  

};

#endif