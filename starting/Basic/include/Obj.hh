#ifndef Obj_HPP
#define Obj_HPP


// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//using namespace glm;

#include <vector>
#include <string>
using namespace std;

class Obj
{
    private:
        vector<glm::vec3> fVertices;
        vector<glm::vec2> fUvs;
        vector<glm::vec3> fNormals;

        string fName;

    public:
        Obj(string name);
        ~Obj();
        
        bool LoadOBJ(const char* path);

        vector<glm::vec3> GetVertices(){ return fVertices;}
        vector<glm::vec2> GetUvs(){ return fUvs;}
        vector<glm::vec3> GetNormals(){ return fNormals;}
};

#endif