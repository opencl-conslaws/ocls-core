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


#ifndef DATA_H
#define DATA_H

#include "Domain.h"
#ifdef OCLS_USE_VIS
#include "RenderContext.h"
#else
typedef int GLuint;
#endif
#include "ComputeContext.h"
#include "Program.h"
#include "Logger.h"

#include <vector>
#include <string>
#include <iostream>
#include <cstring>

typedef cl_mem cl_image;
namespace ocls {

class DataManager;

/** \ingroup main_api
*  @{
*/
class Data {
public:
    enum Usage{
        READ, WRITE, READ_WRITE
    };
    
    
private:
    friend class DataManager;
    
    /**
     *
     */
    Data(ComputeContext* compute_context, RenderContext* render_context,
         const size_t* size_, const Usage& usage_, const std::string name_);
    /**
     *
     */
    ~Data();
    
public:
    /**
    * Retuns a read-only pointer
    */
    float* getDataPointer();

    /**
    * Returns a OpenGL texture ID
    */
    GLuint getTextureID();


    operator float*();
    operator GLuint();
    
    void upload(float* data, size_t* size, size_t* offset);

    float at(size_t x);
    float at(size_t x, size_t y);
    float at(size_t x, size_t y, size_t z);

    void markAsChanged();
    size_t* getDimSize();
    size_t* getUnpaddedDimSize();
    size_t getDim();
    size_t getBytes();

    void copy(Data* src);
    
    std::string getName() const;
    
    Program::Parameter getParameter();

    void max(float* max);
    float max();
    void min(float* min);
    float min();

    void minmax(float* minmax);
    
private:
    void download();
    void copyToTexture();
    void downloadUnpadded();
    
    void createImage();
    void createBuffer();
    
    cl_mem_flags usageToFlag(const Usage usage_);
    
private:
    RenderContext* m_render_context;
    ComputeContext* m_compute_context;
    
    bool m_need_download;
    bool m_need_unpadded;
    bool m_need_texture;
    bool m_min_cache_invalid;
    bool m_max_cache_invalid;
    
    std::vector<float> m_data;
    std::vector<float> m_data_unpadded;

    std::vector<float> m_texture_buffer;

    GLuint m_texture;
    cl_image m_image;

    cl_mem m_buffer;
    cl_mem m_unpadded_buffer;

    size_t m_dim;
    size_t m_size[3];
    size_t m_unpadded_size[3];
    size_t m_bytes;
    Usage m_usage;
    
    std::string m_name;

    float m_min_cache;
    float m_max_cache;
};
/** @} */
    
} //cls

#endif //DATA_OBJECT_H
