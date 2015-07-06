#include "TextureFBO.h"

TextureFBO::TextureFBO(GLuint width, GLuint height, GLint format, GLenum type) :
        m_width(width), m_height(height) {


    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height,
            0, GL_RGBA, type, NULL);


    glGenRenderbuffers(1, &m_depth);
    glBindRenderbuffer(GL_RENDERBUFFER, m_depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, width, height);

    glGenFramebuffers(1, &m_fbo);
    bind(GL_FRAMEBUFFER);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depth);

    //TODO: Not sure if needed for completeness on some systems
    //glDrawBuffer(GL_NONE);
    //glReadBuffer(GL_NONE);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    CHECK_GL_ERRORS();
    CHECK_GL_FBO_COMPLETENESS();
    unbind(GL_FRAMEBUFFER);
}

TextureFBO::~TextureFBO() {
    glDeleteTextures(1, &m_texture);
    glDeleteRenderbuffers(1, &m_depth);
    glDeleteFramebuffers(1, &m_fbo);
}

void TextureFBO::bind(GLenum action) {
    glBindFramebuffer(action, m_fbo);
}

void TextureFBO::unbind(GLenum action) {
    glBindFramebuffer(action, 0);
}

GLuint TextureFBO::getHeight() {
    return m_height;
}

GLuint TextureFBO::getWidth() {
    return m_width;
}

GLuint TextureFBO::getTexture() {
    return m_texture;
}
