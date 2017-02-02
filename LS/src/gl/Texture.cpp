#include "gl/Texture.h"

Texture::Texture()
{

}
Texture::Texture(std::string path)
{
    _texture = createTexture(path);
}
Texture::~Texture()
{

}
void Texture::bindTexture(int index)
{
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, _texture);
}
GLuint Texture::createTexture(std::string path)
{
    ILuint image;
    ilGenImages(1, &image);
    ilBindImage(image);
    ilEnable(IL_ORIGIN_SET);
    ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
    ILboolean loadSucsess = ilLoad(IL_PNG, path.c_str());
    if (loadSucsess == false) {
        std::cout << "File not found: '" << path.c_str() << "'" << std::endl;
        ILenum err = ilGetError() ;
        printf( "the error %d\n", err );
        printf( "string is %s\n", ilGetString( err ) );
        ilBindImage(0);
        ilDeleteImage(image);
        return -1;
    }

    ILboolean convertSuccess = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
    if (!convertSuccess) {
        ilBindImage(0);
        std::cout << "Failed to convert image: '" << path << "'" << std::endl;
        ilDeleteImage(image);
        return -1;
    }
    GLuint texture;
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT );

    glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_TYPE), ilGetData());
    glBindTexture(GL_TEXTURE_2D, 0);

    ilBindImage(0);
    ilDeleteImage(image);
    return texture;
}
