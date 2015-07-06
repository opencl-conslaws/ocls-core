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


#ifndef PROGRAM_MANAGER_H
#define PROGRAM_MANAGER_H

#include "Program.h"
#include <map>

namespace ocls {

class Framework;

/** \ingroup main_api
*  @{
*/
class ProgramManager{
public:
    /**
    *
    */
    ProgramManager(Framework* framework);
    /**
    *
    */
    ~ProgramManager();

    /**
    * Manage a program object
    */
    Program* manage(Program* program){
        m_programs.push_back(program);
        return program;
    }

private:
    Framework* m_framework;
    std::vector<Program*> m_programs;

};
/** @} */

} //cls

#endif //
