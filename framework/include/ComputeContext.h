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


#ifndef COMPUTE_CONTEXT_H
#define COMPUTE_CONTEXT_H

#include <string>
#include <map>
#include <vector>

#ifndef GLEW_MX
#define GLEW_MX
#endif

#ifdef OCLS_USE_VIS
#include <GL/glew.h>
#endif

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/opencl.h>
#endif

#include "Logger.h"

namespace ocls {

class RenderContext;

#define CL_VERSION_1_00 100
#define CL_VERSION_1_10 110
#define CL_VERSION_1_11 111
#define CL_VERSION_1_20 120

enum DeviceType{
    __GPU__, __CPU__, __ACCELERATOR__, __UNKNOWN__
};
    
/**
 * OpenCL context
 */
class ComputeContext{
public:
    /**
     * Create OpenCL context with OpenGL interoprability
     */
    ComputeContext(RenderContext* rendercontext, DeviceType type=__GPU__);
    
    /**
     * Tear down context
     */
    ~ComputeContext();

    cl_command_queue& queue();
    cl_device_id& device();
    cl_context& context();
    cl_platform_id& platform();
    
    std::string getDeviceInfo(std::string key);
    std::string getPlatformInfo(std::string key);
    
    void writeDeviceInfo(std::ostream& out_);
    void writePlatformInfo(std::ostream& out_);

    bool usingSharedContext();
    bool usingRenderInteroperability();

    bool isExtensionAvailable(const std::string &extension_name);

    DeviceType type();
    int version();
    
private:
    void queryPlatforms();
    void quaryDevices(cl_platform_id platform);

    void buildPlatformInfo();
    void buildDeviceInfo();
    
    std::string extractFloatingPointInfo(cl_device_fp_config info);
    std::string extractExecutionCapabilitiesInfo(cl_device_exec_capabilities info);
    std::string extractDeviceTypeInfo(cl_device_type info);
    
private:
    cl_command_queue m_queue;
    cl_device_id m_device;
    cl_context m_context;
    cl_platform_id m_platform;

    std::vector<cl_platform_id> m_platforms;
    std::vector<cl_device_id> m_devices;

    bool m_share_context;
    bool m_render_interoperability;

    int m_cl_version;
    DeviceType m_type;

    std::map<std::string, std::string> m_platform_info;
    std::map<std::string, std::string> m_device_info;
};

} //cls

#endif //COMPUTE_CONTEXT_H
