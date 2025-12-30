#ifndef _TEXTURE_H
#define _TEXTURE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include "stb-image.h"
#include <iostream> 
using namespace std;

class Texture2D {
private:
    string imagePath;

    // Texture unit, e.g., GL_TEXTURE0, GL_TEXTURE1, etc.
    GLuint texture;
    GLuint unit;
    GLuint mipmapLevel;
    GLuint dataType;

public:
    Texture2D(
        GLuint unit,
        string imagePath,
        GLuint mipmapLevel = 0,
        GLuint dataType = GL_UNSIGNED_BYTE
    ):  unit(unit),
        imagePath(imagePath),
        mipmapLevel(mipmapLevel),
        dataType(dataType)
    {
        glGenTextures(1, &texture);
        glActiveTexture(unit);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    ~Texture2D() {
        glDeleteTextures(1, &texture);
    }

    void setTextureWrap(GLint wrapS, GLint wrapT) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
    }

    void setTextureFilter(GLint minFilter, GLint magFilter) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    }

    void load() {
        int texWidth, texHeight, nrChannels;
       
        // OpenGL expects the 0.0 coordinate on y-axis to be on the bottom side of the image
        // So we flip vertically whenever we want to load the image
        stbi_set_flip_vertically_on_load(true);

        unsigned char* imgData = stbi_load(
            this->imagePath.c_str(),
            &texWidth,
            &texHeight,
            &nrChannels,
            0
        );

        if (!imgData) throw runtime_error("ERROR::TEXTURE_2D::CANNOT_LOAD_TEXTURE");

        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;

        glTexImage2D(
            GL_TEXTURE_2D,
            this->mipmapLevel,
            format,
            texWidth,
            texHeight,
            0,  // legacy stuff which simply we ignore it
            format,
            this->dataType,
            imgData
        );
        glGenerateMipmap(GL_TEXTURE_2D);

        // clean up the image memory after generation of texture
        stbi_image_free(imgData);
    }

    void use() {
        glActiveTexture(this->unit);
        glBindTexture(GL_TEXTURE_2D, this->texture);
    }
};

#endif
