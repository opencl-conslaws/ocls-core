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


#include <dirent.h>
#include <fcntl.h>
#include "Data.h"
#include "PerformanceAnalyzer.h"
#include "Utility.h"

#if defined(__SSE__) || (_M_IX86_FP > 0) // GCC/MinGW or Visual C++
#include "xmmintrin.h"
#endif

namespace ocls {


Data::Data(ComputeContext* compute_context, RenderContext* render_context,
        const size_t* size_, const Usage& usage_, const std::string name_) :
        m_name(name_),
        m_usage(usage_),
        m_need_texture(false),
        m_need_download(false),
        m_need_unpadded(false),
        m_min_cache_invalid(true),
        m_max_cache_invalid(true){

    memcpy(&m_unpadded_size[0], size_, 3*sizeof(size_t));
    memcpy(&m_size[0], size_, 3*sizeof(size_t));

    m_compute_context = compute_context;
    m_render_context = render_context;

    size_t multiple = 32; //TODO: Get from compute context

    m_size[0] = roundUp(m_size[0], multiple);
    m_dim = 3;
    if(m_size[2] == 1) m_dim--;
    else m_size[2] = roundUp(m_size[2], multiple);
    if(m_size[1] == 1) m_dim--;
    else m_size[1] = roundUp(m_size[1], multiple);

    //printf("[%zu, %zu, %zu]\n", m_size[0], m_size[1], m_size[2]);

    createBuffer();
    m_bytes = m_data.size()*sizeof(float);
    if (m_render_context != NULL) {
        createImage();
    }
}

Data::~Data(){
    logger->log(Logger::DEBUG, "Deleting data %s", m_name.c_str());

    CHECK_CL_ERROR(clReleaseMemObject(m_buffer));
    CHECK_CL_ERROR(clReleaseMemObject(m_unpadded_buffer));

#ifdef OCLS_USE_VIS
    if(m_compute_context->usingRenderInteroperability()){
        if (m_image != NULL) {
            CHECK_CL_ERROR(clReleaseMemObject(m_image));
        }
        glDeleteTextures(1, &m_texture);
    }
#endif
}



float* Data::getDataPointer() {
    //if (m_need_download) {
    if(m_need_unpadded){
        //download();
        downloadUnpadded();
    }
    //return (float*)m_data.data();
    return m_data_unpadded.data();
}
GLuint Data::getTextureID(){
    if (m_need_texture) {
        copyToTexture();
    }

    return m_texture;
}

Data::operator float*(){
    return getDataPointer();
}
Data::operator GLuint(){
    return getTextureID();
}


float Data::at(size_t x){
    if (m_need_download) {
        download();
    }
    return m_data[x];
}
float Data::at(size_t x, size_t y){
    if (m_need_download) {
        download();
    }
    return m_data[y*m_size[0]+x];
}
float Data::at(size_t x, size_t y, size_t z){
    if (m_need_download) {
        download();
    }
    return m_data[z*(m_size[0]*m_size[1])+y*m_size[0]+x];
}

void Data::markAsChanged(){
    m_need_download = true;
    m_need_unpadded = true;
    m_need_texture = true;
    m_min_cache_invalid = true;
    m_max_cache_invalid = true;
}

std::string Data::getName() const{
    return m_name;
}

Program::Parameter Data::getParameter(){
    markAsChanged();
    Program::Parameter param;
    param.value = &m_buffer;
    param.size = sizeof(cl_mem);
    return param;
}



void Data::download(){
    logger->log(Logger::DEBUG, "Local Data is marked as invalid, downloading from device");

    cl_event event;

    size_t bytes = m_size[0]*m_size[1]*m_size[2]*sizeof(float);
    cl_int err = clEnqueueReadBuffer(m_compute_context->queue(), m_buffer, CL_TRUE, 0, bytes, &m_data[0], 0, NULL, &event);
    if (err != CL_SUCCESS) {
        logger->log(Logger::ERROR, "Failed to read OpenCL buffer");
    }
    m_need_download = false;

    PerformanceAnalyser::analyzeMemoryEvent(&m_compute_context->queue(), event, bytes, PerformanceAnalyser::MemoryEvent::DEVICE_TO_HOST);

    // Make all the data has been downloaded;
    clFinish(m_compute_context->queue());
    clReleaseEvent(event);
}
void Data::copyToTexture(){
    logger->log(Logger::DEBUG, "Synchronizing device data with texture data");

    if(!m_compute_context->usingRenderInteroperability()){
        return;
    }

    cl_int err;
#ifdef OCLS_USE_VIS
    if(m_compute_context->usingSharedContext()) {
        if(m_dim == 3 && (m_image == NULL)){
            //logger->log(Logger::ERROR, "The extension 'cl_khr_3d_image_writes' is required to write 3D data to textures in kernel, "
            //        "Fallback is not yet implemented.");

            if(m_need_download)
                download();
            //float* data = getDataPointer();


            size_t t_size = m_unpadded_size[0]*m_unpadded_size[1]*m_unpadded_size[2];
            if(m_texture_buffer.size() == 0)
                m_texture_buffer.resize(t_size*4);

            for(size_t z = 0; z < m_unpadded_size[2]; z++){
                for(size_t y = 0; y < m_unpadded_size[1]; y++){
                    for(size_t x = 0; x < m_unpadded_size[0]; x++){
                        size_t index = z*(m_size[0]*m_size[1])+y*m_size[0]+x;
                        //size_t index = z*(m_unpadded_size[0]*m_unpadded_size[1])+y*m_unpadded_size[0]+x;
                        size_t tex_index = (z*(m_unpadded_size[0]*m_unpadded_size[1])+y*m_unpadded_size[0]+x)*4;

#if defined(__SSE__) || (_M_IX86_FP > 0) // GCC/MinGW or Visual C++
                        __m128 bytes = _mm_load1_ps(&m_data[index]);
                        _mm_store_ps(&m_texture_buffer[tex_index], bytes);
#else
                        m_texture_buffer[tex_index]     = m_data[index];
                        m_texture_buffer[tex_index+1]   = m_data[index];
                        m_texture_buffer[tex_index+2]   = m_data[index];
                        m_texture_buffer[tex_index+3]   = m_data[index];
#endif
                    }
                }
            }

            glBindTexture(GL_TEXTURE_3D, m_texture);
            glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA32F, (GLuint)m_unpadded_size[0], (GLuint)m_unpadded_size[1], (GLuint)m_unpadded_size[2],
                    0, GL_RGBA, GL_FLOAT, m_texture_buffer.data());
            glBindTexture(GL_TEXTURE_3D, 0);
            //glFinish();

        }else {
            cl_mem mem_objects[1] = {m_image};
            err = clEnqueueAcquireGLObjects(m_compute_context->queue(), 1, mem_objects, 0, NULL, NULL);
            CHECK_CL_ERROR(err);

            cluEnqueueCopyBufferToImageRGBA(m_compute_context->queue(), m_buffer, m_image, m_dim, m_unpadded_size, m_size);

            err = clEnqueueReleaseGLObjects(m_compute_context->queue(), 1, mem_objects, 0, NULL, NULL);
            CHECK_CL_ERROR(err);
        }
        // Don't know if clEnqueueReleaseGLObjects does the job. may need a clfinish
        // to make sure that the GL texture is ready before proceeding.
        clFinish(m_compute_context->queue());

    }else{
        //if(m_need_download)
        //    download();

        logger->log(Logger::ERROR, "Shared Context is required for visualization, "
                "Fallback is not yet implemented.");
    }
#else
    logger->log(Logger::ERROR, "Shared Context is required for visualization, "
            "but has been disabled during build.");
#endif
    m_need_texture = false;
}

void Data::createImage(){
#ifdef OCLS_USE_VIS
    if(!m_compute_context->usingRenderInteroperability()){
        return;
    }


#define CL_OLD // < TODO: find out if OpenCL 1.1 or newer
#ifdef CL_OLD
// Suppress ugly warnings from old OpenCL API
// TODO: add pragma for other compilators
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    if (m_size[2] == 1) {
        // 2D or 1D
        logger->log(Logger::DEBUG, "Creating 1D/2D OpenGL/OpenCL Interoprability Texture");

        glGenTextures(1, &m_texture);
        glBindTexture(GL_TEXTURE_2D, m_texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, (GLsizei)m_unpadded_size[0], (GLsizei)m_unpadded_size[1],
                0, GL_RGBA, GL_FLOAT, NULL);

        cl_int err;
        if(m_compute_context->usingSharedContext()) {
            m_image = clCreateFromGLTexture2D(m_compute_context->context(), CL_MEM_READ_WRITE, GL_TEXTURE_2D, 0, m_texture, &err);
            //m_image = clCreateFromGLTexture (m_compute_context->context(),
            //        CL_MEM_READ_WRITE, GL_TEXTURE_2D,
            //        0, m_texture, &err);
            CHECK_CL_ERROR(err);
        }else{
            logger->log(Logger::WARNING, "Render context is not shared, rendering performance will be affected");
        }

        //glBindTexture(GL_TEXTURE_2D, 0);

    } else {
        //3D
        logger->log(Logger::DEBUG, "Creating 3D OpenGL/OpenCL Interoprability Texture");

        glGenTextures(1, &m_texture);
        glBindTexture(GL_TEXTURE_3D, m_texture);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
        glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA32F, (GLsizei)m_unpadded_size[0], (GLsizei)m_unpadded_size[1], (GLsizei)m_unpadded_size[2], 0, GL_RGBA, GL_FLOAT, NULL);

        cl_int err;
        if(m_compute_context->usingSharedContext() && m_compute_context->isExtensionAvailable("cl_khr_3d_image_writes")) {
            m_image = clCreateFromGLTexture3D(m_compute_context->context(), CL_MEM_READ_WRITE, GL_TEXTURE_3D, 0, m_texture, &err);
            CHECK_CL_ERROR(err)
        }else{
            m_image = NULL;
            logger->log(Logger::WARNING, "Render context is not shared, rendering performance will be affected");
        }

        //glBindTexture(GL_TEXTURE_3D, 0);
    }

#else
// TODO: implement function for new API
    cl_mem clCreateFromGLTexture (cl_context context,
                                  cl_mem_flags flags, GLenum texture_target,
                                  GLint miplevel, GLuint texture, cl_int *errcode_ret)
flags: See clCreateFromGLBuffer
texture_target: GL_TEXTURE_{1D, 2D}[_ARRAY],
    GL_TEXTURE_{3D, BUFFER, RECTANGLE},
    GL_TEXTURE_CUBE_MAP_POSITIVE_{X, Y, Z},
    GL_TEXTURE_CUBE_MAP_NEGATIVE_{X, Y, Z}
    CL Image Objects > GL Renderbuffers
#endif

    m_need_texture = true;
#endif // OCLS_USE_VIS
}
void Data::createBuffer(){
    cl_int err = CL_SUCCESS;

    size_t count = m_size[0]*m_size[1]*m_size[2];
    m_buffer = clCreateBuffer(m_compute_context->context(), usageToFlag(m_usage), count*sizeof(float), NULL, &err);
    CHECK_CL_ERROR(err);
    m_data.resize(count);

    memset(m_data.data(), 0, count*sizeof(float));
    err = clEnqueueWriteBuffer(m_compute_context->queue(), m_buffer, CL_FALSE, 0, count*sizeof(float), m_data.data(), 0, NULL, NULL);
    CHECK_CL_ERROR(err);

    count = m_unpadded_size[0]*m_unpadded_size[1]*m_unpadded_size[2];
    m_unpadded_buffer = clCreateBuffer(m_compute_context->context(), usageToFlag(m_usage), count*sizeof(float), NULL, &err);
    CHECK_CL_ERROR(err);
    m_data_unpadded.resize(count);

    m_need_download = true;
    m_need_unpadded = true;
}

cl_mem_flags Data::usageToFlag(const Usage usage_){
    switch (usage_) {
        case READ:
            return CL_MEM_READ_ONLY;
        case WRITE:
            return CL_MEM_WRITE_ONLY;
        case READ_WRITE:
            return CL_MEM_READ_WRITE;
        default:
            return CL_MEM_READ_WRITE;
    }
}


size_t *Data::getDimSize() {
    return &m_size[0];
}

size_t *Data::getUnpaddedDimSize() {
    return &m_unpadded_size[0];
}

size_t Data::getDim() {
    return m_dim;
}

size_t Data::getBytes() {
    return m_bytes;
}

void Data::copy(Data *src) {
    cl_event event;

    cl_int err = clEnqueueCopyBuffer(m_compute_context->queue(), src->m_buffer,
            m_buffer, 0, 0, m_bytes, 0, NULL, &event);

    CHECK_CL_ERROR(err);

    PerformanceAnalyser::analyzeMemoryEvent(&m_compute_context->queue(), event, m_bytes, PerformanceAnalyser::MemoryEvent::DEVICE_TO_DEVICE);
    m_need_download = true;
    m_need_texture = true;

    clReleaseEvent(event);
}

void Data::max(float* value) {
    if(!m_max_cache_invalid){
        memcpy(value, &m_max_cache, sizeof(float));
        return;
    }

    logger->log(Logger::DEBUG, "Max cache is invalid, rebuilding ...");
    size_t flat_size = m_size[0]*m_size[1]*m_size[2];
    if(m_compute_context->type() == __GPU__) {
        cluEnqueueReduction(m_compute_context->context(), m_compute_context->queue(),
                m_buffer, flat_size, value, MAX);
    }else{
        cluEnqueueReductionCPU(m_compute_context->context(), m_compute_context->queue(),
                m_buffer, flat_size, value, MAX);
    }

    //clFinish(m_compute_context->queue());
    // Cache results for later
    memcpy(&m_max_cache, value, sizeof(float));
    logger->log(Logger::DEBUG, "Max value is: %f", m_max_cache);
    m_max_cache_invalid = false;
}

void Data::min(float* value) {
    if(!m_min_cache_invalid){
        memcpy(value, &m_min_cache, sizeof(float));
        return;
    }

    logger->log(Logger::DEBUG, "Min cache is invalid, rebuilding ...");
    size_t flat_size = m_size[0]*m_size[1]*m_size[2];
    if(m_compute_context->type() == __GPU__) {
        cluEnqueueReduction(m_compute_context->context(), m_compute_context->queue(),
                m_buffer, flat_size, value, MIN);
    }else{
        cluEnqueueReductionCPU(m_compute_context->context(), m_compute_context->queue(),
                m_buffer, flat_size, value, MIN);
    }

    //clFinish(m_compute_context->queue());
    // Cache results for later
    memcpy(&m_min_cache, value, sizeof(float));
    logger->log(Logger::DEBUG, "Min value is: %f", m_min_cache);
    m_min_cache_invalid = false;
}

void Data::minmax(float* value) {
    min(&value[0]);
    max(&value[1]);
}

void Data::upload(float *data, size_t *size, size_t *offset) {
    for(size_t z = 0; z < size[2]; z++){
        for (size_t y = 0; y < size[1]; y++) {
            size_t index = z * (size[0] * size[1]) + y * size[0];
            memcpy(&m_data_unpadded[index], &data[index], sizeof(float)*size[0]);
        }
    }

    cl_event  event;

    logger->log(Logger::DEBUG, "Uploading data without padding");
    size_t bytes = m_unpadded_size[0]*m_unpadded_size[1]*m_unpadded_size[2]*sizeof(float);
    cl_int err = clEnqueueWriteBuffer(m_compute_context->queue(), m_unpadded_buffer, CL_TRUE, 0, bytes, &m_data_unpadded[0], 0, NULL, &event);
    if (err != CL_SUCCESS) {
        logger->log(Logger::ERROR, "Failed to write OpenCL buffer");
    }

    PerformanceAnalyser::analyzeMemoryEvent(&m_compute_context->queue(), event, bytes, PerformanceAnalyser::MemoryEvent::HOST_TO_DEVICE);

    logger->log(Logger::DEBUG, "Adding padding to device data");
    cluEnqueueWriteUnpaddedToPadded(m_compute_context->context(), m_compute_context->queue(),
            m_unpadded_buffer, m_buffer, getDim(), m_unpadded_size, m_size);

    m_need_download = true;
    clReleaseEvent(event);
}

float Data::max() {
    float value;
    max(&value);
    return value;
}

float Data::min() {
    float value;
    min(&value);
    return value;
}

void Data::downloadUnpadded() {
    logger->log(Logger::DEBUG, "Downloading data without padding");
    cluEnqueueWritePaddedToUnpadded(m_compute_context->context(), m_compute_context->queue(),
            m_buffer, m_unpadded_buffer, getDim(), m_size, m_unpadded_size);

    cl_event event;

    size_t bytes = m_unpadded_size[0]*m_unpadded_size[1]*m_unpadded_size[2]*sizeof(float);
    cl_int err = clEnqueueReadBuffer(m_compute_context->queue(), m_unpadded_buffer, CL_TRUE, 0, bytes, &m_data_unpadded[0], 0, NULL, &event);
    if (err != CL_SUCCESS) {
        logger->log(Logger::ERROR, "Failed to read OpenCL buffer");
    }
    m_need_unpadded = false;

    PerformanceAnalyser::analyzeMemoryEvent(&m_compute_context->queue(), event, bytes, PerformanceAnalyser::MemoryEvent::DEVICE_TO_HOST);

    // Make all the data has been downloaded;
    clFinish(m_compute_context->queue());
    clReleaseEvent(event);
}
}
