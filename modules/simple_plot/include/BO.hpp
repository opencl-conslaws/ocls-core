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



#ifndef _BO_HPP__
#define _BO_HPP__

#include <GL/glew.h>

namespace GLUtils {

template <GLenum T>
class BO {
public:
	BO(const void* data, unsigned int bytes, int usage=GL_STATIC_DRAW) {
		glGenBuffers(1, &vbo_name);
		bind();
		glBufferData(T, bytes, data, usage);
		unbind();
	}

	~BO() {
		unbind();
		glDeleteBuffers(1, &vbo_name);
	}

	inline void bind() {
		glBindBuffer(T, vbo_name);
	}

	static inline void unbind() {
		glBindBuffer(T, 0);
	}

	inline GLuint name() {
		return vbo_name;
	}

private:
	BO() {}
	GLuint vbo_name; //< VBO name
};

};//namespace GLUtils

#endif
