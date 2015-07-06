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


#ifndef CALLABLE_FUNCTION_H
#define CALLABLE_FUNCTION_H

#include "Data.h"
#include "Collection.h"
#include "Program.h"
#include <type_traits>

#define __VARIADIC_TEMPLATE_ARGUMENT__

namespace ocls {
class Framework;
//class CallableFunction;

class ReturnType{
public:
    ReturnType(){}
    ReturnType(Data* data) :
            m_data(data) {
        m_collection = Collection::glob(data);
    }
    ReturnType(Collection collection):
            m_data(collection[0]), m_collection(collection){}
    ~ReturnType(){}

    Data* asData(){
        return m_data;
    }
    Collection asCollection(){
        return m_collection;
    }

    operator Data*(){
        return asData();
    }
    operator Collection(){
        return asCollection();
    }

private:
    Data* m_data;
    Collection m_collection;
};

class CallableFunction {
public:
    CallableFunction(Domain* domain, Framework *framework,
                std::vector<CLSSource::Variable*>& expected_parameters) :
            m_domain(*domain), m_framework(framework), m_expected_parameters(expected_parameters){}

    virtual ~CallableFunction(){
    }

    ReturnType call(size_t length, Program::Parameter* args) {
        std::vector<Program::Parameter> params;
        params.resize(length);
        memcpy(params.data(), args, length*sizeof(Program::Parameter));
        return call(params);
    }

#ifdef __VARIADIC_TEMPLATE_ARGUMENT__
    template<typename ... Arguments>
    ReturnType operator()(Arguments& ... args) {
        std::vector<Program::Parameter> params;
        extract(&params, args...);
        return call(params);
    }
#endif

private:
    void processArgument(std::vector<Program::Parameter>* params, Data* arg){
        arg->markAsChanged(); // < Data is most likely to be altered
        params->push_back(
                arg->getParameter()
        );
    }
    void processArgument(std::vector<Program::Parameter>* params, Collection& arg){
        for (int i = 0; i < arg.size(); ++i) {
            arg[i]->markAsChanged(); // < Data is most likely to be altered
            params->push_back(
                    arg[i]->getParameter()
            );
        }
    }

#ifdef __VARIADIC_TEMPLATE_ARGUMENT__
    template<typename T,
            typename std::enable_if<!std::is_pointer<T>::value &&
                                    !std::is_same<T,Collection>::value>::type* = nullptr >
    void processArgument(std::vector<Program::Parameter>* params, T& arg) {
        params->push_back(
                Program::Parameter((void *) &(arg), sizeof(T), std::string(typeid(arg).name()))
        );
    }

    void extract(std::vector<Program::Parameter>* params){
        /* Terminate recursion */
    }

    template<typename Argument, typename ... Arguments>
    void extract(std::vector<Program::Parameter>* params, Argument& arg, Arguments& ... args){
        processArgument(params, arg);
        extract(params, args...);
    }
#endif

protected:
    virtual ReturnType call(std::vector<Program::Parameter>& parameters) = 0;

protected:
    std::vector<CLSSource::Variable*> m_expected_parameters;

    Framework *m_framework;
    Domain m_domain;

};

}
#endif
