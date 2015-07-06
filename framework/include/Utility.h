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


#ifndef UTILITY_H
#define UTILITY_H

#include <limits>

#ifdef __APPLE__
#include <OpenCL/cl_platform.h>

#else
#include <CL/cl_platform.h>
#endif
#include "ComputeContext.h"
#include "Logger.h"
#include "Data.h"
#include "ProgramLauncher.h"

#define CHECK_CL_ERROR(ret) checkCLError(ret,"",__FILE__,__LINE__);
#define CHECK_CL_ERROR_MSG(ret, msg) checkCLError(ret,msg,__FILE__,__LINE__);

static const char* cluGetErrorString(cl_int code){
    switch (code){
        case CL_SUCCESS:
            return "CL_SUCCESS";
        case CL_DEVICE_NOT_FOUND:
            return "CL_DEVICE_NOT_FOUND";
        case CL_DEVICE_NOT_AVAILABLE:
            return "CL_DEVICE_NOT_AVAILABLE";
        case CL_COMPILER_NOT_AVAILABLE:
            return "CL_COMPILER_NOT_AVAILABLE";
        case CL_MEM_OBJECT_ALLOCATION_FAILURE:
            return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
        case CL_OUT_OF_RESOURCES:
			return "CL_OUT_OF_RESOURCES";
        case CL_OUT_OF_HOST_MEMORY:
			return "CL_OUT_OF_HOST_MEMORY";
        case CL_PROFILING_INFO_NOT_AVAILABLE:
			return "CL_PROFILING_INFO_NOT_AVAILABLE";
        case CL_MEM_COPY_OVERLAP:
			return "CL_MEM_COPY_OVERLAP";
        case CL_IMAGE_FORMAT_MISMATCH:
			return "CL_IMAGE_FORMAT_MISMATCH";
        case CL_IMAGE_FORMAT_NOT_SUPPORTED:
			return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
        case CL_BUILD_PROGRAM_FAILURE:
			return "CL_BUILD_PROGRAM_FAILURE";
        case CL_MAP_FAILURE:
			return "CL_MAP_FAILURE";
        case CL_INVALID_VALUE:
			return "CL_INVALID_VALUE";
        case CL_INVALID_DEVICE_TYPE:
			return "CL_INVALID_DEVICE_TYPE";
        case CL_INVALID_PLATFORM:
			return "CL_INVALID_PLATFORM";
        case CL_INVALID_DEVICE:
			return "CL_INVALID_DEVICE";
        case CL_INVALID_CONTEXT:
			return "CL_INVALID_CONTEXT";
        case CL_INVALID_QUEUE_PROPERTIES:
			return "CL_INVALID_QUEUE_PROPERTIES";
        case CL_INVALID_COMMAND_QUEUE:
			return "CL_INVALID_COMMAND_QUEUE";
        case CL_INVALID_HOST_PTR:
			return "CL_INVALID_HOST_PTR";
        case CL_INVALID_MEM_OBJECT:
			return "CL_INVALID_MEM_OBJECT";
        case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:
			return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
        case CL_INVALID_IMAGE_SIZE:
			return "CL_INVALID_IMAGE_SIZE";
        case CL_INVALID_SAMPLER:
			return "CL_INVALID_SAMPLER";
        case CL_INVALID_BINARY:
			return "CL_INVALID_BINARY";
        case CL_INVALID_BUILD_OPTIONS:
			return "CL_INVALID_BUILD_OPTIONS";
        case CL_INVALID_PROGRAM:
			return "CL_INVALID_PROGRAM";
        case CL_INVALID_PROGRAM_EXECUTABLE:
			return "CL_INVALID_PROGRAM_EXECUTABLE";
        case CL_INVALID_KERNEL_NAME:
			return "CL_INVALID_KERNEL_NAME";
        case CL_INVALID_KERNEL_DEFINITION:
			return "CL_INVALID_KERNEL_DEFINITION";
        case CL_INVALID_KERNEL:
			return "CL_INVALID_KERNEL";
        case CL_INVALID_ARG_INDEX:
			return "CL_INVALID_ARG_INDEX";
        case CL_INVALID_ARG_VALUE:
			return "CL_INVALID_ARG_VALUE";
        case CL_INVALID_ARG_SIZE:
			return "CL_INVALID_ARG_SIZE";
        case CL_INVALID_KERNEL_ARGS:
			return "CL_INVALID_KERNEL_ARGS";
        case CL_INVALID_WORK_DIMENSION:
			return "CL_INVALID_WORK_DIMENSION";
        case CL_INVALID_WORK_GROUP_SIZE:
			return "CL_INVALID_WORK_GROUP_SIZE";
        case CL_INVALID_WORK_ITEM_SIZE:
			return "CL_INVALID_WORK_ITEM_SIZE";
        case CL_INVALID_GLOBAL_OFFSET:
			return "CL_INVALID_GLOBAL_OFFSET";
        case CL_INVALID_EVENT_WAIT_LIST:
			return "CL_INVALID_EVENT_WAIT_LIST";
        case CL_INVALID_EVENT:
			return "CL_INVALID_EVENT";
        case CL_INVALID_OPERATION:
			return "CL_INVALID_OPERATION";
        case CL_INVALID_GL_OBJECT:
			return "CL_INVALID_GL_OBJECT";
        case CL_INVALID_BUFFER_SIZE:
			return "CL_INVALID_BUFFER_SIZE";
        case CL_INVALID_MIP_LEVEL:
			return "CL_INVALID_MIP_LEVEL";
        case CL_INVALID_GLOBAL_WORK_SIZE:
			return "CL_INVALID_GLOBAL_WORK_SIZE";
        default:
            return "CL_UNKNOWN";
    }
}

inline void checkCLError(cl_int ret, const char* msg, const char* file, unsigned int line) {
    if (ret != CL_SUCCESS){
        ocls::logger->log(ocls::Logger::ERROR, "OpenCL error: %s at %s:%d %s", cluGetErrorString(ret), file, line, msg);
    }
}

static cl_int cluEnqueueCopyBufferToImageRGBA(cl_command_queue queue, cl_mem buffer, cl_image image, size_t dim, size_t* region, size_t* data_region){
    ocls::Program* memcpy_prog = ocls::ProgramLauncher::getMemcpyProgram(dim);
    std::vector<ocls::Program::Parameter> params;
    params.push_back(ocls::Program::Parameter((void*)&buffer, sizeof(cl_mem)));
    params.push_back(ocls::Program::Parameter((void*)&image, sizeof(cl_image)));

    cl_uint3 data_size;
    data_size.s[0] = (unsigned int)data_region[0];
    data_size.s[1] = (unsigned int)data_region[1];
    data_size.s[2] = (unsigned int)data_region[2];
    params.push_back(ocls::Program::Parameter((void*)&data_size, sizeof(cl_uint3)));

    ocls::ProgramLauncher::launch(memcpy_prog, params, dim, region, true);

    return 0;//TODO: return error
}

static size_t gcd ( size_t a, size_t b ) {
    if ( a==0 ) return b;
    return gcd ( b%a, a );
}

static size_t roundUp(size_t numToRound, size_t multiple)
{
    if(multiple == 0)
    {
        return numToRound;
    }

    size_t remainder = numToRound % multiple;
    if (remainder == 0)
        return numToRound;
    return numToRound + multiple - remainder;
}

static cl_int cluEnqueueWritePaddedToUnpadded(cl_context context, cl_command_queue queue, cl_mem padded, cl_mem unpadded, size_t dim, size_t* padded_size, size_t* unpadded_size){
    ocls::Program* prog = ocls::ProgramLauncher::getPaddedToUnpaddedProgram(dim);

    if(dim == 1){
        std::vector<ocls::Program::Parameter> params;
        params.push_back(ocls::Program::Parameter((void*)&padded, sizeof(cl_mem)));

        cl_int val = (cl_int)unpadded_size[0];

        params.push_back(ocls::Program::Parameter(&val, sizeof(cl_int)));
        params.push_back(ocls::Program::Parameter((void*)&unpadded, sizeof(cl_mem)));

        ocls::ProgramLauncher::launch(prog, params, dim, padded_size, false);
    }
    else if(dim == 2){
        std::vector<ocls::Program::Parameter> params;
        params.push_back(ocls::Program::Parameter((void*)&padded, sizeof(cl_mem)));

        cl_int2 val_unpad = {(cl_int)unpadded_size[0],
                            (cl_int)unpadded_size[1]};
        cl_int2 val_pad =   {(cl_int)padded_size[0],
                            (cl_int)padded_size[1]};

        params.push_back(ocls::Program::Parameter(&val_unpad, sizeof(cl_int2)));
        params.push_back(ocls::Program::Parameter(&val_pad, sizeof(cl_int2)));
        params.push_back(ocls::Program::Parameter((void*)&unpadded, sizeof(cl_mem)));

        ocls::ProgramLauncher::launch(prog, params, dim, padded_size, false);
    }
    else if(dim == 3){
        std::vector<ocls::Program::Parameter> params;
        params.push_back(ocls::Program::Parameter((void*)&padded, sizeof(cl_mem)));

        cl_int3 val_unpad = {(cl_int)unpadded_size[0],
                (cl_int)unpadded_size[1],
                (cl_int)unpadded_size[2]};
        cl_int3 val_pad =   {(cl_int)padded_size[0],
                (cl_int)padded_size[1],
                (cl_int)padded_size[2]};

        params.push_back(ocls::Program::Parameter(&val_unpad, sizeof(cl_int3)));
        params.push_back(ocls::Program::Parameter(&val_pad, sizeof(cl_int3)));
        params.push_back(ocls::Program::Parameter((void*)&unpadded, sizeof(cl_mem)));

        ocls::ProgramLauncher::launch(prog, params, dim, padded_size, false);
    }

    return 0;
}

static cl_int cluEnqueueWriteUnpaddedToPadded(cl_context context, cl_command_queue queue, cl_mem unpadded, cl_mem padded, size_t dim, size_t* unpadded_size, size_t* padded_size){
    ocls::Program* prog = ocls::ProgramLauncher::getUnpaddedToPaddedProgram(dim);

    if(dim == 1){
        std::vector<ocls::Program::Parameter> params;
        params.push_back(ocls::Program::Parameter((void*)&unpadded, sizeof(cl_mem)));

        cl_int val = (cl_int)unpadded_size[0];
        params.push_back(ocls::Program::Parameter(&val, sizeof(int)));
        params.push_back(ocls::Program::Parameter((void*)&padded, sizeof(cl_mem)));

        ocls::ProgramLauncher::launch(prog, params, dim, padded_size, false);
    }
    else if(dim == 2){
        std::vector<ocls::Program::Parameter> params;
        params.push_back(ocls::Program::Parameter((void*)&unpadded, sizeof(cl_mem)));

        cl_int2 val_unpad = {(cl_int)unpadded_size[0],
                             (cl_int)unpadded_size[1]};
        cl_int2 val_pad =   {(cl_int)padded_size[0],
                            (cl_int)padded_size[1]};

        params.push_back(ocls::Program::Parameter(&val_unpad, sizeof(cl_int2)));
        params.push_back(ocls::Program::Parameter(&val_pad, sizeof(cl_int2)));
        params.push_back(ocls::Program::Parameter((void*)&padded, sizeof(cl_mem)));

        ocls::ProgramLauncher::launch(prog, params, dim, padded_size, false);
    }
    else if(dim == 3){
        std::vector<ocls::Program::Parameter> params;
        params.push_back(ocls::Program::Parameter((void*)&unpadded, sizeof(cl_mem)));

        cl_int3 val_unpad = {(cl_int)unpadded_size[0],
                            (cl_int)unpadded_size[1],
                            (cl_int)unpadded_size[2]};
        cl_int3 val_pad =   {(cl_int)padded_size[0],
                            (cl_int)padded_size[1],
                            (cl_int)padded_size[2]};

        params.push_back(ocls::Program::Parameter(&val_unpad, sizeof(cl_int3)));
        params.push_back(ocls::Program::Parameter(&val_pad, sizeof(cl_int3)));
        params.push_back(ocls::Program::Parameter((void*)&padded, sizeof(cl_mem)));

        ocls::ProgramLauncher::launch(prog, params, dim, padded_size, false);
    }

    return 0;
}

typedef enum{
    MIN, MAX
} Reduction_Type;

static cl_int cluEnqueueReductionCPU(cl_context context, cl_command_queue queue, cl_mem buffer, size_t size, float* value, Reduction_Type type){
    ocls::logger->log(ocls::Logger::DEBUG, "CPU style reduction");

    std::vector<float> tmp;
    tmp.resize(size);

    clEnqueueReadBuffer(queue, buffer, CL_TRUE, 0, sizeof(float)*size, &tmp[0], 0, NULL, NULL);

    if(type == MIN){
        *value = std::numeric_limits<float>::max();
    }else{
        *value = -std::numeric_limits<float>::max();
    }

    for (int i = 0; i < size; ++i) {
        if(type == MIN){
            *value = std::min(*value,tmp[i]);
        }else{
            *value = std::max(*value,tmp[i]);
        }
    }
/*
    ocls::Program* find_prog = NULL;
    switch (type) {
        case MAX:
            find_prog = ocls::ProgramLauncher::getFindMaximumProgram(ocls::DeviceType::__CPU__);
            break;
        case MIN:
            find_prog = ocls::ProgramLauncher::getFindMinimumProgram(ocls::DeviceType::__CPU__);
            break;
        default:
            ocls::logger->log(ocls::Logger::ERROR, "Reduction form not implemented");
            break;
    }

    size_t n_size = 1;

    // TODO: Cache tmp storage for increased performance.
    cl_mem value_buffer0 = clCreateBuffer(context,
            CL_MEM_READ_WRITE, sizeof(float)*n_size, NULL, NULL);

    std::vector<ocls::Program::Parameter> params;
    params.push_back(ocls::Program::Parameter((void*)&buffer, sizeof(cl_mem)));
    params.push_back(ocls::Program::Parameter(&size, sizeof(size_t)));
    params.push_back(ocls::Program::Parameter(&n_size, sizeof(int)));
    params.push_back(ocls::Program::Parameter((void*)&value_buffer0, sizeof(cl_mem)));

    size_t region[] = {n_size, 1, 1};
    size_t local[] = {n_size, 1, 1};
    ocls::ProgramLauncher::launch(find_prog, params, 1, region, local, false);


    clEnqueueReadBuffer(queue, value_buffer0, CL_TRUE, 0, sizeof(float), value, 0, NULL, NULL);
    clFinish(queue);

    clReleaseMemObject(value_buffer0);

*/
    return 0;
}

static cl_int cluEnqueueReduction(cl_context context, cl_command_queue queue, cl_mem buffer, size_t size, float* value, Reduction_Type type){
    ocls::logger->log(ocls::Logger::DEBUG, "GPU style reduction");

    size_t t = 256;
    if(size <= t){
        t = size;
    }

    ocls::Program* find_prog = NULL;
    switch (type) {
        case MAX:
            find_prog = ocls::ProgramLauncher::getFindMaximumProgram(ocls::DeviceType::__GPU__);
            break;
        case MIN:
            find_prog = ocls::ProgramLauncher::getFindMinimumProgram(ocls::DeviceType::__GPU__);
            break;
        default:
            ocls::logger->log(ocls::Logger::ERROR, "Reduction form not implemented");
            break;
    }

    size_t n_size = roundUp(size/t, 256);

    // TODO: Cache tmp storage for increased performance.
    cl_mem value_buffer0 = clCreateBuffer(context,
            CL_MEM_READ_WRITE, sizeof(float)*n_size, NULL, NULL);

    std::vector<ocls::Program::Parameter> params;
    params.push_back(ocls::Program::Parameter((void*)&buffer, sizeof(cl_mem)));
    params.push_back(ocls::Program::Parameter(NULL, sizeof(float)*n_size));
    params.push_back(ocls::Program::Parameter(&size, sizeof(int)));
    params.push_back(ocls::Program::Parameter((void*)&value_buffer0, sizeof(cl_mem)));

    size_t region[] = {size, 1, 1};
    size_t local[] = {t, 1, 1};
    ocls::ProgramLauncher::launch(find_prog, params, 1, region, local, false);

    if((size/t) != 1){
        cluEnqueueReduction(context, queue, value_buffer0, n_size, value, type);
    }else{
        clEnqueueReadBuffer(queue, value_buffer0, CL_TRUE, 0, sizeof(float), value, 0, NULL, NULL);
        clFinish(queue);
    }

    clReleaseMemObject(value_buffer0);

    return 0;
}

#endif //UTILITY_H
