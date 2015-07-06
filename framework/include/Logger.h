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


#ifndef LOGGER_H
#define LOGGER_H

#include <cstdio>
#include <ctime>
#include <cstdarg>
#include <iostream>
#include <exception>
#include <stdexcept>
#include <sstream>
#include <fstream>
#include <cstring>

namespace ocls {
class Logger;
extern Logger* logger;

class Logger {
public:

    enum Level {
        NO_LOG = -1,
        ERROR = 0,
        WARNING = 1,
        DEBUG = 2,
    };

    Logger(Level mask) : m_next(NULL) {
        m_mask = std::max(mask, NO_LOG);
    }

    virtual ~Logger() {
        delete m_next;
    }

    Logger *chainLogger(Logger *logger) {
        m_next = logger;
        return m_next;
    }

    std::string maskToString(Level mask) {
        switch (mask) {
            case ERROR:
                return "ERROR\t";
            case WARNING:
                return "WARNING\t";
            case DEBUG:
                return "DEBUG\t";
            case NO_LOG:
                return "";
        }
    }

    std::string getTime() {
        time_t rawtime;
        struct tm *timeinfo;

        time(&rawtime);
        timeinfo = localtime(&rawtime);

        char *time_str = asctime(timeinfo);
        time_str[strlen(time_str) - 1] = '\0';
        return std::string(time_str);
    }

    void log(Level level, const char *message, ...) {
        char buffer[512];
        va_list argptr;
        va_start(argptr, message);
        vsnprintf(buffer, 512, message, argptr);
        va_end(argptr);

        clog(level, &buffer[0]);
    }

protected:
    void clog(Level level, const char *message) {
        if (m_mask >= level) {
            write(level, message);
        }
        if (m_next != NULL) {
            m_next->clog(level, message);
        }
    }

    virtual void write(Level level, const char *message) = 0;

protected:
    Logger *m_next;
    Level m_mask;
};

class ConsoleLogger : public Logger {
public:
    ConsoleLogger(Level mask) : Logger(mask) {
    }

    void write(Level level, const char *message) {
        std::cout << maskToString(level) << "[" << getTime() << "]: " << message << std::endl;
    }
};

class CrashLogger : public Logger {
public:
    CrashLogger(Level mask) : Logger(mask) {
    }

    void write(Level level, const char *message) {
        std::stringstream ss;
        ss << maskToString(level) << "[" << getTime() << "]: " << message << std::endl;
        std::cerr << ss.str();
        throw std::runtime_error(ss.str());
    }
};

class FileLogger : public Logger {
public:
    FileLogger(Level mask) : Logger(mask) {
    }

    void write(Level level, const char *message) {
        std::fstream file;
        file.open("fw.log", std::fstream::out | std::ios_base::app);

        file << maskToString(level) << "[" << getTime() << "]: " << message << std::endl;

        file.close();
    }
};

}

#endif
