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



#ifndef _GLUTILS_HPP__
#define _GLUTILS_HPP__

#include <cstdlib>
#include <sstream>
#include <vector>
#include <assert.h>
#include <iostream>
#include <fstream>

#include <GL/glew.h>

#include "Program.hpp"
#include "BO.hpp"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#define CHECK_GL_ERRORS() GLUtils::checkGLErrors(__FILE__, __LINE__)
#define CHECK_GL_FBO_COMPLETENESS() GLUtils::checkGLFBOCompleteness(__FILE__, __LINE__)

namespace GLUtils {

inline void checkGLErrors(const char* file, unsigned int line) {
	GLenum ASSERT_GL_err = glGetError(); 
    if( ASSERT_GL_err != GL_NO_ERROR ) { 
		std::stringstream ASSERT_GL_string; 
		ASSERT_GL_string << file << '@' << line << ": OpenGL error:" 
             << std::hex << ASSERT_GL_err << " " << "ERROR";
			 THROW_EXCEPTION( ASSERT_GL_string.str() ); 
    } 
}

inline void checkGLFBOCompleteness(const char* file, unsigned int line) {
	GLenum err = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (err != GL_FRAMEBUFFER_COMPLETE) {
		std::stringstream log; 
		log << file << '@' << line << ": FBO incomplete error:" 
             << std::hex << err << " " << "ERROR";
			 throw std::runtime_error(log.str()); 
	}
}


}; //Namespace GLUtils

#endif
