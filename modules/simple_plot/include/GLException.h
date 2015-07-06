/**
  *  This file is part of the OpenCL-ConsLaws framework
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



#ifndef _GLEXCEPTION_H__
#define _GLEXCEPTION_H__

#include <stdexcept>
#include <string>
#include <iostream>
#include <sstream>

class GLException : public std::runtime_error {
public:
	GLException(const char* file, unsigned int line, const char* msg) : std::runtime_error(msg) {
		std::cerr << file << ":" << line << ": " << msg << std::endl;
	}

	GLException(const char* file, unsigned int line, const std::string msg) : std::runtime_error(msg) {
		std::cerr << file << ":" << line << ": " << msg << std::endl;
	}
};


#define THROW_EXCEPTION(msg) throw GLException(__FILE__, __LINE__, msg)

#endif
