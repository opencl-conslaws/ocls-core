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


#ifndef CLS_SOURCE_H
#define CLS_SOURCE_H

#include "CLSSourceLoader.h"
#include <map>
#include <string>
#include <cstdlib>
#include <vector>

namespace ocls {

/** \ingroup main_api
*  @{
*/
class CLSSource{
public:
    struct Variable{
        CLSSource* source;
        
        std::string type;
        std::string name;
        std::string value;

        bool array_type;
        size_t array_size;
    };
    struct Structure{
        CLSSource* source;

        std::string name;
        std::map<std::string, Variable*> variables;
    };
    struct Function{
        CLSSource* source;
        
        std::string name;
        std::string signature;
        std::string block;

        std::string returntype;
        std::vector<Variable*> parameters;

        //std::vector<std::string> dependencies;
        std::vector<Function*> function_dependencies;
        std::vector<Structure*> structure_dependencies;
        std::vector<Variable*> variable_dependencies;
    };

    
private:
    friend class CLSSourceLoader;
    
    /**
     *
     */
    CLSSource();
    /**
     *
     */
    ~CLSSource();

    /**
     *
     */
    size_t parseStructure(std::string& src, size_t start);
    
    /**
     *
     */
    size_t parseVariable(std::string& src, size_t start);
    
    /**
     *
     */
    size_t parseVariable(std::string& src, size_t start, std::map<std::string, Variable*>& container);
    
    /**
     *
     */
    size_t parseFunction(std::string& src, size_t start);

    /**
    *
    */
    void parseSignature(Function* function);

    /**
    *
    */
    Variable* parseParameter(std::string str);

    /**
     *
     *
     */
    void solveDependencies();
    
public:
    /**
     *
     */
    Function* getFunction(std::string name);
    
    /**
     *
     */
    Variable* getVariable(std::string name);
    
    /**
     *
     */
    template<class T> T getVariableValue(std::string name){
        return (T)std::atof(getVariable(name)->value.c_str());
    }
    
    /**
     * id
     */
    size_t id();

private:
    std::map<std::string, Function*> m_functions;
    std::map<std::string, Variable*> m_variables;
    std::map<std::string, Structure*> m_structures;
    
    size_t m_id;
    static size_t m_id_gen;
};
/** @} */

} //cls

#endif //CLS_SOURCE_H
