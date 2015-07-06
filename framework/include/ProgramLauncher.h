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


#ifndef PROGRAM_LAUNCHER_H
#define PROGRAM_LAUNCHER_H

#include "Program.h"
#include "ComputeContext.h"

#include <map>

namespace ocls {

class ProgramLauncher {
public:
    //static ProgramLauncher& instance();
    static void init(ComputeContext* compute_context);
    static void destory();

    static void launch(const Program* program, std::vector<Program::Parameter>& parameters, bool blocking);
    static void launch(const Program* program, std::vector<Program::Parameter>& parameters, const Domain& domain, bool blocking);
    static void launch(const Program* program, std::vector<Program::Parameter>& parameters, size_t dim, size_t* global_size, bool blocking);
    static void launch(const Program* program, std::vector<Program::Parameter>& parameters, size_t dim, size_t* global_size, size_t* local_size, bool blocking);

    static Program* getMemcpyProgram(size_t texture_dim);
    static Program* getUnpaddedToPaddedProgram(size_t dim);
    static Program* getPaddedToUnpaddedProgram(size_t dim);

    static Program* getFindMaximumProgram(DeviceType type);
    static Program* getFindMinimumProgram(DeviceType type);

private:
    //ProgramLauncher() :m_is_initialized(false) {};
    //ProgramLauncher(ProgramLauncher const&);    // Don't Implement
    //void operator=(ProgramLauncher const&);     // Don't implement
private:
    // Utility programs
    static Program* m_memcpy_buf_to_tex_program[3];
    static Program* m_find_maximum_value_program[2];
    static Program* m_find_minimum_value_program[2];

    static Program* m_unpadded_to_padded_program[3];
    static Program* m_padded_to_unpadded_program[3];

    // private members
    static bool m_is_initialized;
    static ComputeContext* m_compute_context;
};


}

#endif
