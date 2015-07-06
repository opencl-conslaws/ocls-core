#ifndef TEXTURE_FBO_H
#define TEXTURE_FBO_H

#include "GLUtils.hpp"

class TextureFBO {
public:
    TextureFBO(GLuint width, GLuint height, GLint format = GL_RGBA32F, GLenum type = GL_FLOAT);
    ~TextureFBO();

    void bind(GLenum action);
    void unbind(GLenum action);

    GLuint getHeight();
    GLuint getWidth();
    GLuint getTexture();

private:
    GLuint m_fbo;
    GLuint m_depth;
    GLuint m_texture;

    GLuint m_width;
    GLuint m_height;
};

#endif