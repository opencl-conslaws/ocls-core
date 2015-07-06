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




#include "c_structures.h"
#include "c_analyser.h"
#include "PerformanceAnalyzer.h"

extern "C" {

int oclsAnalyserStart(void){
    try {
        ocls::PerformanceAnalyser::start();
    }catch(std::exception& e){
        strcpy(error_buffer, e.what());
        return ERROR_FATAL;
    }
    return ERROR_OK;
}

int oclsAnalyserEnd(void){
    try {
        ocls::PerformanceAnalyser::end();
    }catch(std::exception& e){
        strcpy(error_buffer, e.what());
        return ERROR_FATAL;
    }
    return ERROR_OK;
}

int oclsAnalyserPrint(void){
    try {
        ocls::PerformanceAnalyser::writeAnalyticsTimeline(std::cout);
    }catch(std::exception& e){
        strcpy(error_buffer, e.what());
        return ERROR_FATAL;
    }
    return ERROR_OK;
}

int oclsAnalyserRead(char** output){
    try {
        std::stringstream ss;
        ocls::PerformanceAnalyser::writeAnalyticsTimeline(ss);
        *output = (char*)malloc(ss.str().length());

        strcpy(*output, ss.str().c_str());
    }catch(std::exception& e){
        strcpy(error_buffer, e.what());
        return ERROR_FATAL;
    }
    return ERROR_OK;
}


int oclsAnalyserKernelRead(const char *kernel_name, char **output) {
    try {
        std::stringstream ss;
        ocls::PerformanceAnalyser::writeKernelAnalyse(std::string(kernel_name),ss);
        *output = (char*)malloc(ss.str().length());

        strcpy(*output, ss.str().c_str());
    }catch(std::exception& e){
        strcpy(error_buffer, e.what());
        return ERROR_FATAL;
    }
    return ERROR_OK;
}

int oclsAnalyserMemoryRead(char **output) {
    try {
        std::stringstream ss;
        ocls::PerformanceAnalyser::writeMemoryAnalyse(ss);
        *output = (char*)malloc(ss.str().length());

        strcpy(*output, ss.str().c_str());
    }catch(std::exception& e){
        strcpy(error_buffer, e.what());
        return ERROR_FATAL;
    }
    return ERROR_OK;
}

int oclsAnalyserKernel(const char *kernel_name, KernelAnalysis** kernelAnalysis) {
    try {
        ocls::PerformanceAnalyser::KernelAnalysis analysis = ocls::PerformanceAnalyser::analyseKernel(std::string(kernel_name));
        *kernelAnalysis = (KernelAnalysis*)malloc(sizeof(KernelAnalysis));

        memcpy(*kernelAnalysis, &analysis, sizeof(KernelAnalysis));
    }catch(std::exception& e){
        strcpy(error_buffer, e.what());
        return ERROR_FATAL;
    }
    return ERROR_OK;
}

int oclsAnalyserMemory(MemoryAnalysis** memoryAnalysis) {
    try {
        ocls::PerformanceAnalyser::MemoryAnalysis analysis = ocls::PerformanceAnalyser::analyseMemory();
        *memoryAnalysis = (MemoryAnalysis*)malloc(sizeof(MemoryAnalysis));

        memcpy(*memoryAnalysis, &analysis, sizeof(MemoryAnalysis));
    }catch(std::exception& e){
        strcpy(error_buffer, e.what());
        return ERROR_FATAL;
    }
    return ERROR_OK;
}

}
