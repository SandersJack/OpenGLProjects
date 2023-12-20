#ifndef Texture2D_HPP
#define Texture2D_HPP


class Texture2D {

    public:
        Texture2D();
        void Generate(unsigned int width, unsigned int height, unsigned char* data);
        // binds the texture as the current active GL_TEXTURE_2D texture object
        void Bind() const;

        void SetInternal_Format(unsigned int val){ fInternal_Format = val;}
        void SetImage_Format(unsigned int val){ fImage_Format = val;}

        
    private:
        unsigned int fID;
        // texture image dimensions
        unsigned int fWidth, fHeight; // width and height of loaded image in pixels
        // texture Format
        unsigned int fInternal_Format; // format of texture object
        unsigned int fImage_Format; // format of loaded image
        // texture configuration
        unsigned int fWrap_S; // wrapping mode on S axis
        unsigned int fWrap_T; // wrapping mode on T axis
        unsigned int fFilter_Min; // filtering mode if texture pixels < screen pixels
        unsigned int fFilter_Max; // filtering mode if texture pixels > screen pixels

};
#endif