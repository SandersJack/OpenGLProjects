#ifndef SpriteRenderer_HPP
#define SpriteRenderer_HPP

#include "Global.hh"


#include "MathGL.hh"
#include "Shader.hh"
#include "Texture2D.hh"


#include <GL/glew.h>
#include <GLFW/glfw3.h>

class SpriteRenderer {
    public:
        SpriteRenderer();
        ~SpriteRenderer();

        void initRenderData();
        void SetShader(Shader val){fShader = val;}
        void DrawSprite(const Texture2D& texture, Vector2 position, Vector2 size = Vector2(10.0f, 10.0f), float rotate = 0.0f, Vector4 color = Vector4(1.0f));
       
        static SpriteRenderer *GetInstance();

    private:

        static SpriteRenderer *fInstance;

        Shader fShader;
        uint fQuadVAO;
};

#endif