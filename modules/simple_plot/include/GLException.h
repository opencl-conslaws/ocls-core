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
