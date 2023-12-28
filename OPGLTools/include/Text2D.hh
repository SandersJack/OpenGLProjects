#ifndef Text2D_HPP
#define Text2D_HPP

#include <GL/glew.h>

#include <GLFW/glfw3.h>
//GLFWwindow* window;

#include <iostream>
#include <map>

#include "MathGL.hh"

#include <ft2build.h>
#include FT_FREETYPE_H


class Text2D {

    struct Character {
        unsigned int TextureID;  // ID handle of the glyph texture
        iVector2   Size;       // Size of glyph
        iVector2   Bearing;    // Offset from baseline to left/top of glyph
        long int Advance;    // Offset to advance to next glyph
    };

    public:
        Text2D();
        ~Text2D();

        static Text2D *GetInstance();

        void initFreeType(const char* fontPath, int fontSize);
        void RenderText(std::string text, float x, float y, float scale, Vector3 color);

        void SetShaderProgram(GLuint val){ textProgram2D = val;}


    private:
        void initText();
        static Text2D *fInstance;
        GLuint VAO_text, VBO_text;
        GLuint textProgram2D;

        std::map<char, Character> Characters;

};

#endif