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


#include "Program.h"
#include <iostream>
#include <sstream>
#include <Utility.h>
#include "PerformanceAnalyzer.h"

namespace ocls {

Program::Program(ComputeContext* compute_context,const Domain& domain, const std::string& name_, const std::string& source_, const std::string& compiler_arg_){
    m_compute_context = compute_context;
    m_execution_domain = domain;
    m_name = name_;
    cl_int err = CL_SUCCESS;
    
    const char* src[] = {source_.c_str()};
    m_program = clCreateProgramWithSource(compute_context->context(), 1, src, NULL, &err);
    CHECK_CL_ERROR(err);
    
    compile(compute_context->device(), compiler_arg_);

    std::stringstream ss;
    ss << name_ << "_kernel";
    m_entry_point = clCreateKernel (m_program, ss.str().c_str(), &err);
    CHECK_CL_ERROR_MSG(err, ss.str().c_str());

    logger->log(Logger::DEBUG, "Creating Program %s", m_name.c_str());
}
    
void Program::compile(cl_device_id device, const std::string& compiler_arg_) {
    cl_int err;

    err = clBuildProgram(m_program, 1, &device, compiler_arg_.c_str(), NULL, NULL);
    if(err != CL_SUCCESS){
        if(err == CL_BUILD_PROGRAM_FAILURE){
            std::stringstream ss;
            ss << "Kernel failed to compile with error(s).\n";

            size_t size;
            clGetProgramBuildInfo(m_program, device, CL_PROGRAM_BUILD_LOG, 0, NULL, &size);
                
            // Get the log
            std::string log;
            log.resize(size);
            clGetProgramBuildInfo(m_program, device, CL_PROGRAM_BUILD_LOG, size, const_cast<char*>(log.data()),0);
                
            // Store the log
            ss << log.c_str() << std::endl;

            std::cout << ss.str() << std::endl;
            logger->log(Logger::ERROR, ss.str().c_str());
        }else{
            CHECK_CL_ERROR(err);
        }
    }else{
        // There might still be some warnings
        size_t size;
        clGetProgramBuildInfo(m_program, device, CL_PROGRAM_BUILD_LOG, 0, NULL, &size);
        if(size > 2) {
            std::stringstream ss;
            ss << "Kernel compiled with warning(s).\n";

            // Get the log
            std::string log;
            log.resize(size);
            clGetProgramBuildInfo(m_program, device, CL_PROGRAM_BUILD_LOG, size, const_cast<char *>(log.data()), 0);

            ss << log.c_str() << std::endl;
            logger->log(Logger::WARNING, ss.str().c_str());
        }
    }
}


Program::~Program(){
    logger->log(Logger::DEBUG, "Deleting Program %s", m_name.c_str());
    CHECK_CL_ERROR(clReleaseKernel(m_entry_point));
    CHECK_CL_ERROR(clReleaseProgram (m_program));
}


Domain Program::getExecutionDomain() const{
    return m_execution_domain;
}

std::string Program::getName() const{
    return m_name;
}

cl_kernel Program::getEntryPoint() const{
    return m_entry_point;
}
} //cls
