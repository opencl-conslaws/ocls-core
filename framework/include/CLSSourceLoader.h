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


#ifndef CLS_SOURCE_LOADER_H
#define CLS_SOURCE_LOADER_H

#include <map>
#include "Logger.h"

namespace ocls {

class CLSSource;
class Framework;

/** \ingroup main_api
*  @{
*/
class CLSSourceLoader{
private:
    friend class Framework;
    
    /**
     *
     */
    CLSSourceLoader(Framework* framwork);
    /**
     *
     */
    ~CLSSourceLoader();

public:
    /**
     * Load source from string
     */
    CLSSource* loadFromString(std::string str);
    
    /**
     * Load source from file
     */
    CLSSource* loadFromFile(std::string path);
    
private:
    void removeComments(std::string* str);
    
    /**
     * Reads a file, returns true if success, false otherwise
     */
    bool readFile(std::string file, std::string* content);

private:
    std::map<size_t, CLSSource*> m_source;
};
/** @} */

} //cls

#endif //CLS_SOURCE_LOADER_H
