/**
  *  This file is part of the OpenCL-ConsLaws framework
  *  Copyright (C) 2010 - 2013 André R. Brodtkorb
  *  Copyright (C) 2014, 2015 Jens Kristoffer Reitan Markussen
  *
  * This program is free software: you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation, either version 3 of the License, or
  * (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with this program.  If not, see <http://www.gnu.org/licenses/>.
  *
  */



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
