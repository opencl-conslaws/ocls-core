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



#ifndef _PROGRAM_HPP__
#define _PROGRAM_HPP__

#include "GLException.h"

#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <cassert>

#include <GL/glew.h>

namespace GLUtils {


class Program {
public:
	Program(std::string vs, std::string fs) {
		name = glCreateProgram();

		attachShader(vs, GL_VERTEX_SHADER);
		attachShader(fs, GL_FRAGMENT_SHADER);
		link();
	}

	Program(std::string vs, std::string gs, std::string fs) {
		name = glCreateProgram();

		attachShader(vs, GL_VERTEX_SHADER);
		attachShader(gs, GL_GEOMETRY_SHADER);
		attachShader(fs, GL_FRAGMENT_SHADER);
		link();
	}

	inline void use() {
		glUseProgram(name);
	}

	static inline void disuse() {
		glUseProgram(0);
	}

	inline GLint getUniform(std::string var) {
		GLint loc = glGetUniformLocation(name, var.c_str());
		//assert(loc >= 0);
		return loc;
	}

	inline void setAttributePointer(std::string var, unsigned int size, GLenum type=GL_FLOAT, GLboolean normalized=GL_FALSE, GLsizei stride=0, GLvoid* pointer=NULL) {
		GLint loc = glGetAttribLocation(name, var.c_str());
		assert(loc >= 0);
		glVertexAttribPointer(loc, size, type, normalized, stride, pointer);
		glEnableVertexAttribArray(loc);
	}
    
    inline void bindFragDataLocation(GLuint n, std::string name){
        glBindFragDataLocation(this->name, n, name.c_str());
    }

private:
	void link() {
		std::stringstream log;
		glLinkProgram(name);

		// check for errors
		GLint linkstatus;
		glGetProgramiv(name, GL_LINK_STATUS, &linkstatus);
		if (linkstatus != GL_TRUE) {
			log << "Linking failed!" << std::endl;

			GLint logsize;
			glGetProgramiv(name, GL_INFO_LOG_LENGTH, &logsize);

			if (logsize > 0) {
				std::vector < GLchar > infolog(logsize + 1);
				glGetProgramInfoLog(name, logsize, NULL, &infolog[0]);
				log << "--- error log ---" << std::endl;
				log << std::string(infolog.begin(), infolog.end()) << std::endl;
			} else {
				log << "--- empty log message ---" << std::endl;
			}
			THROW_EXCEPTION(log.str());
		}
	}

	void attachShader(std::string& src, unsigned int type) {
		std::stringstream log;
		// create shader object
		GLuint s = glCreateShader(type);
		if (s == 0) {
			log << "Failed to create shader of type " << type << std::endl;
			THROW_EXCEPTION(log.str());
		}

		// set source code and compile
		const GLchar* src_list[1] = { src.c_str() };
		glShaderSource(s, 1, src_list, NULL);
		glCompileShader(s);

		// check for errors
		GLint compile_status;
		glGetShaderiv(s, GL_COMPILE_STATUS, &compile_status);
		if (compile_status != GL_TRUE) {
			// compilation failed
			log << "Compilation failed!" << std::endl;
			log << "--- source code ---" << std::endl;
			std::istringstream src_ss(src);
			std::string line;
			unsigned int i=0;
			while (std::getline(src_ss, line))
				log << std::setw(4) << std::setfill('0') << ++i << line << std::endl;

			GLint logsize;
			glGetShaderiv(s, GL_INFO_LOG_LENGTH, &logsize);
			if (logsize > 0) {
				std::vector<GLchar> infolog(logsize + 1);
				glGetShaderInfoLog(s, logsize, NULL, &infolog[0]);

				log << "--- error log ---" << std::endl;
				log << std::string(infolog.begin(), infolog.end()) << std::endl;
			}
			else {
				log << "--- empty log message ---" << std::endl;
			}
			THROW_EXCEPTION(log.str());
		}
		
		glAttachShader(name, s);
	}

	GLuint name; //< OpenGL shader program

};

}; //Namespace GLUtils

#endif
