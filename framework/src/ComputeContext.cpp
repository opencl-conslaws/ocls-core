#include "ComputeContext.h"
#include "RenderContext.h"
#include <iostream>
#include <sstream>
#include <Utility.h>
#include <bitset>

void context_error_callback(const char *errinfo,
                            const void *private_info, size_t cb,
                            void *user_data) {

    std::cout << errinfo << std::endl;
    fputs(errinfo, stderr);
}

namespace ocls {

ComputeContext::ComputeContext(RenderContext* rendercontext, DeviceType type) {
    cl_int err = CL_SUCCESS;

    queryPlatforms();
    for (size_t i = 0; i < m_platforms.size(); i++){
        m_platform = m_platforms[i];

        bool found_device = false;
        cl_device_type typeBuf;

        quaryDevices(m_platform);

        for (size_t j = 0; j < m_devices.size(); j++) {
            m_device = m_devices[j];


            clGetDeviceInfo(m_device,
                    CL_DEVICE_TYPE,
                    sizeof(cl_device_type),
                    &typeBuf,
                    NULL);

            if (typeBuf & CL_DEVICE_TYPE_CPU && type == __CPU__) {
                found_device = true;
                break;
            }
            else if (typeBuf & CL_DEVICE_TYPE_GPU && type == __GPU__) {
                found_device = true;
                break;
            }
            else if (typeBuf & CL_DEVICE_TYPE_ACCELERATOR && type == __ACCELERATOR__) {
                found_device = true;
                break;
            }
        }
        if(found_device){
            break;
        }
    }



    buildPlatformInfo();
    logger->log(Logger::DEBUG, "Using OpenCL platform: %s", getPlatformInfo("NAME").c_str());
    buildDeviceInfo();
    logger->log(Logger::DEBUG, "Using OpenCL device: %s", getDeviceInfo("NAME").c_str());

    // OpenCL Specs:
    // OpenCL<space><major_version.minor_version><space><vendor-specific information>
    std::string v_str = m_device_info["VERSION"];
    logger->log(Logger::DEBUG, "Using OpenCL version: %s", v_str.c_str());

    int major = std::atoi(&v_str[7]);
    int minor = std::atoi(&v_str[9]);
    m_cl_version = (major * 100) + (minor * 10);

    if(rendercontext != NULL && rendercontext->isContextValid() &&
            (isExtensionAvailable("cl_khr_gl_sharing") || isExtensionAvailable("cl_APPLE_gl_sharing"))) {
        m_share_context = true;
        // Make sure correct OpenGL context is bound
        switchContext(rendercontext->getContext());

        cl_context_properties properties[] = {
                // We need to add information about the OpenGL context with
                // which we want to exchange information with the OpenCL context.
#if defined (WIN32)
        CL_GL_CONTEXT_KHR , (cl_context_properties) wglGetCurrentContext() ,
        CL_WGL_HDC_KHR , (cl_context_properties) wglGetCurrentDC() ,
#elif defined (__linux__)
                CL_GL_CONTEXT_KHR, (cl_context_properties) glXGetCurrentContext(),
                CL_GLX_DISPLAY_KHR, (cl_context_properties) glXGetCurrentDisplay(),
#elif defined (__APPLE__)
        CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE , (cl_context_properties) CGLGetShareGroup( CGLGetCurrentContext() ) ,
#endif
                CL_CONTEXT_PLATFORM, (cl_context_properties) m_platform,
                0, 0,
        };

        m_context = clCreateContext(properties, 1, &m_device, context_error_callback, NULL, &err);
        CHECK_CL_ERROR(err);

        m_render_interoperability = true;
    }else{
        if(rendercontext == NULL || (!rendercontext->isContextValid())){
            m_render_interoperability = false;
        }

        logger->log(Logger::WARNING, "Unable to create a shared OpenCL/OpenGL context");

        m_share_context = false;
        cl_context_properties properties[] = {
                CL_CONTEXT_PLATFORM, (cl_context_properties) m_platform,
                0, 0,
        };

        m_context = clCreateContext(properties, 1, &m_device, context_error_callback, NULL, &err);
        CHECK_CL_ERROR(err);
    }

    m_queue = clCreateCommandQueue(m_context, m_device, CL_QUEUE_PROFILING_ENABLE, &err);
    CHECK_CL_ERROR(err);

    logger->log(Logger::DEBUG, "OpenCL context created");
}
    
ComputeContext::~ComputeContext(){
    logger->log(Logger::DEBUG, "Destroying compute context");
    clFinish(m_queue);
    clFlush(m_queue);

    CHECK_CL_ERROR(clReleaseCommandQueue(m_queue));
    CHECK_CL_ERROR(clReleaseContext(m_context));
/*
    if (m_context != NULL) {
        cl_uint refs;
        clGetContextInfo(m_context, CL_CONTEXT_REFERENCE_COUNT, sizeof(cl_uint), &refs, NULL);
        logger->log(Logger::WARNING, "refs: %d", refs);
    }
*/
}
    
cl_command_queue& ComputeContext::queue(){
    return m_queue;
}
cl_device_id& ComputeContext::device(){
    return m_device;
}
cl_context& ComputeContext::context(){
    return m_context;
}
cl_platform_id& ComputeContext::platform(){
    return m_platform;
}
    
std::string ComputeContext::getDeviceInfo(std::string key){
    return m_device_info[key];
}
std::string ComputeContext::getPlatformInfo(std::string key){
    return m_platform_info[key];
}
    
void ComputeContext::writeDeviceInfo(std::ostream& out_){
    std::map<std::string, std::string>::iterator it;
    
    for (it=m_device_info.begin(); it!=m_device_info.end(); ++it){
        out_ << it->first << ": " << it->second << std::endl;
    }
}
void ComputeContext::writePlatformInfo(std::ostream& out_){
    std::map<std::string, std::string>::iterator it;
    
    for (it=m_platform_info.begin(); it!=m_platform_info.end(); ++it){
        out_ << it->first << ": " << it->second << std::endl;
    }
}

bool ComputeContext::usingSharedContext() {
    return m_share_context;
}

void ComputeContext::queryPlatforms() {
    cl_int err = CL_SUCCESS;

    cl_uint count;
    CHECK_CL_ERROR(clGetPlatformIDs(0, NULL, &count));
    
    m_platforms.resize(count);
    logger->log(Logger::DEBUG, "Found %d platform(s) on system", count);
    CHECK_CL_ERROR(clGetPlatformIDs(count, &m_platforms[0], NULL));
    for (int i = 0; i < count; ++i) {
        char buffer[512];
        clGetPlatformInfo(m_platforms[i], CL_PLATFORM_NAME, 512, buffer, NULL);
        logger->log(Logger::DEBUG, "Platform[%d]: %s", i, buffer);
    }
}

void ComputeContext::quaryDevices(cl_platform_id platform) {
    cl_int err = CL_SUCCESS;

    cl_uint count;
    CHECK_CL_ERROR(clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 0, NULL, &count));

    m_devices.resize(count);
    logger->log(Logger::DEBUG, "Found %d device(s) on platform", count);
    CHECK_CL_ERROR(clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, count, &m_devices[0],  NULL));
    for (int i = 0; i < count; ++i) {
        char buffer[512];
        clGetDeviceInfo(m_devices[i], CL_DEVICE_NAME, 512, buffer, NULL);
        logger->log(Logger::DEBUG, "Device[%d]: %s", i, buffer);
    }
}
    
void ComputeContext::buildPlatformInfo(){
    char buffer[512];
    CHECK_CL_ERROR(clGetPlatformInfo(m_platform,
                             CL_PLATFORM_PROFILE,
                             512,
                             buffer,
                             NULL));
    m_platform_info["PROFILE"] = std::string(buffer);
    
    CHECK_CL_ERROR(clGetPlatformInfo(m_platform,
                             CL_PLATFORM_VERSION,
                             512,
                             buffer,
                             NULL));
    m_platform_info["VERSION"] = std::string(buffer);
    
    CHECK_CL_ERROR(clGetPlatformInfo(m_platform,
                             CL_PLATFORM_NAME,
                             512,
                             buffer,
                             NULL));
    m_platform_info["NAME"] = std::string(buffer);
    
    CHECK_CL_ERROR(clGetPlatformInfo(m_platform,
                             CL_PLATFORM_VENDOR,
                             512,
                             buffer,
                             NULL));
    m_platform_info["VENDOR"] = std::string(buffer);
    
    CHECK_CL_ERROR(clGetPlatformInfo(m_platform,
                             CL_PLATFORM_EXTENSIONS,
                             512,
                             buffer,
                             NULL));
    m_platform_info["EXTENSIONS"] = std::string(buffer);
    
}
void ComputeContext::buildDeviceInfo(){

    char cstrBuf[512];
    size_t uintBuf;
    cl_bool boolBuf;
    cl_device_fp_config fpBuf;
    cl_device_exec_capabilities exCapBuf;
    cl_device_mem_cache_type memCacheBuf;
    cl_device_local_mem_type memTypeBuf;
    cl_device_type typeBuf;
    cl_ulong ulongBuf;
    
    std::stringstream ss;
    
    CHECK_CL_ERROR(clGetDeviceInfo(m_device,
                           CL_DEVICE_ADDRESS_BITS,
                           sizeof(size_t),
                           &uintBuf,
                           NULL));
    ss << uintBuf;
    m_device_info["ADDRESS_BITS"] = ss.str();
    ss.str(std::string());
    
    CHECK_CL_ERROR(clGetDeviceInfo(m_device,
                           CL_DEVICE_AVAILABLE,
                           sizeof(cl_bool),
                           &boolBuf,
                           NULL));
    m_device_info["AVAILABLE"] = boolBuf == 0 ? "FALSE" : "TRUE";
    
    CHECK_CL_ERROR(clGetDeviceInfo(m_device,
                           CL_DEVICE_COMPILER_AVAILABLE,
                           sizeof(cl_bool),
                           &boolBuf,
                           NULL));
    m_device_info["COMPILER_AVAILABLE"] = boolBuf == 0 ? "FALSE" : "TRUE";
    
    cl_int err = clGetDeviceInfo(m_device,
                           CL_DEVICE_DOUBLE_FP_CONFIG,
                           sizeof(cl_device_fp_config),
                           &fpBuf,
                           NULL);
    if (err == CL_SUCCESS) {
        m_device_info["DOUBLE_FP_CONFIG"] = extractFloatingPointInfo(fpBuf);
    }else{
        m_device_info["DOUBLE_FP_CONFIG"] = "UNSUPPORTED";
        err = CL_SUCCESS;
    }
    
    
    CHECK_CL_ERROR(clGetDeviceInfo(m_device,
                           CL_DEVICE_SINGLE_FP_CONFIG,
                           sizeof(cl_device_fp_config),
                           &fpBuf,
                           NULL));
    m_device_info["SINGLE_FP_CONFIG"] = extractFloatingPointInfo(fpBuf);
    
    err = clGetDeviceInfo(m_device,
                           CL_DEVICE_HALF_FP_CONFIG,
                           sizeof(cl_device_fp_config),
                           &fpBuf,
                           NULL);
    if (err == CL_SUCCESS) {
        m_device_info["HALF_FP_CONFIG"] = extractFloatingPointInfo(fpBuf);
    }else{
        m_device_info["HALF_FP_CONFIG"] = "UNSUPPORTED";
    }
    
    CHECK_CL_ERROR(clGetDeviceInfo(m_device,
                           CL_DEVICE_ENDIAN_LITTLE,
                           sizeof(cl_bool),
                           &boolBuf,
                           NULL));
    m_device_info["ENDIAN_LITTLE"] = boolBuf == 0 ? "FALSE" : "TRUE";
    
    CHECK_CL_ERROR(clGetDeviceInfo(m_device,
                           CL_DEVICE_ERROR_CORRECTION_SUPPORT,
                           sizeof(cl_bool),
                           &boolBuf,
                           NULL));
    m_device_info["ERROR_CORRECTION_SUPPORT"] = boolBuf == 0 ? "FALSE" : "TRUE";
    
    CHECK_CL_ERROR(clGetDeviceInfo(m_device,
                           CL_DEVICE_EXECUTION_CAPABILITIES,
                           sizeof(cl_device_exec_capabilities),
                           &exCapBuf,
                           NULL));
    m_device_info["EXECUTION_CAPABILITIES"] = extractExecutionCapabilitiesInfo(exCapBuf);
    
    /*CHECK_CL_ERROR(*/clGetDeviceInfo(m_device,
                             CL_DEVICE_EXTENSIONS,
                             512,
                             cstrBuf,
                             NULL)/*)*/;
    m_device_info["EXTENSIONS"] = std::string(cstrBuf);
    
    CHECK_CL_ERROR(clGetDeviceInfo(m_device,
                           CL_DEVICE_GLOBAL_MEM_CACHE_SIZE,
                           sizeof(cl_ulong),
                           &ulongBuf,
                           NULL));
    ss << ulongBuf;
    m_device_info["GLOBAL_MEM_CACHE_SIZE"] = ss.str();
    ss.str(std::string());
    
    CHECK_CL_ERROR(clGetDeviceInfo(m_device,
                           CL_DEVICE_GLOBAL_MEM_CACHE_TYPE,
                           sizeof(cl_device_mem_cache_type),
                           &memCacheBuf,
                           NULL));
    
    switch (memCacheBuf) {
        case CL_NONE:
            m_device_info["GLOBAL_MEM_CACHE_TYPE"] = "NONE";
            break;
        case CL_READ_ONLY_CACHE:
            m_device_info["GLOBAL_MEM_CACHE_TYPE"] = "READ_ONLY_CACHE";
            break;
        case CL_READ_WRITE_CACHE:
            m_device_info["GLOBAL_MEM_CACHE_TYPE"] = "READ_WRITE_CACHE";
            break;
            
        default:
            m_device_info["GLOBAL_MEM_CACHE_TYPE"] = "UNKNOWN";
            break;
    }
    
    CHECK_CL_ERROR(clGetDeviceInfo(m_device,
                           CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE,
                           sizeof(size_t),
                           &uintBuf,
                           NULL));
    ss << uintBuf;
    m_device_info["GLOBAL_MEM_CACHELINE_SIZE"] = ss.str();
    ss.str(std::string());
    
    CHECK_CL_ERROR(clGetDeviceInfo(m_device,
                           CL_DEVICE_GLOBAL_MEM_SIZE,
                           sizeof(cl_ulong),
                           &ulongBuf,
                           NULL));
    ss << ulongBuf;
    m_device_info["GLOBAL_MEM_SIZE"] = ss.str();
    ss.str(std::string());
    
    CHECK_CL_ERROR(clGetDeviceInfo(m_device,
                           CL_DEVICE_IMAGE_SUPPORT,
                           sizeof(cl_bool),
                           &boolBuf,
                           NULL));
    m_device_info["IMAGE_SUPPORT"] = boolBuf == 0 ? "FALSE" : "TRUE";
    
    CHECK_CL_ERROR(clGetDeviceInfo(m_device,
                           CL_DEVICE_IMAGE2D_MAX_HEIGHT,
                           sizeof(size_t),
                           &uintBuf,
                           NULL));
    ss << uintBuf;
    m_device_info["IMAGE2D_MAX_HEIGHT"] = ss.str();
    ss.str(std::string());
    
    CHECK_CL_ERROR(clGetDeviceInfo(m_device,
                           CL_DEVICE_IMAGE2D_MAX_WIDTH,
                           sizeof(size_t),
                           &uintBuf,
                           NULL));
    ss << uintBuf;
    m_device_info["IMAGE2D_MAX_WIDTH"] = ss.str();
    ss.str(std::string());
    
    CHECK_CL_ERROR(clGetDeviceInfo(m_device,
                           CL_DEVICE_IMAGE3D_MAX_DEPTH,
                           sizeof(size_t),
                           &uintBuf,
                           NULL));
    ss << uintBuf;
    m_device_info["IMAGE3D_MAX_DEPTH"] = ss.str();
    ss.str(std::string());
    
    CHECK_CL_ERROR(clGetDeviceInfo(m_device,
                           CL_DEVICE_IMAGE3D_MAX_HEIGHT,
                           sizeof(size_t),
                           &uintBuf,
                           NULL));
    ss << uintBuf;
    m_device_info["IMAGE3D_MAX_HEIGHT"] = ss.str();
    ss.str(std::string());
    
    CHECK_CL_ERROR(clGetDeviceInfo(m_device,
                           CL_DEVICE_IMAGE3D_MAX_WIDTH,
                           sizeof(size_t),
                           &uintBuf,
                           NULL));
    ss << uintBuf;
    m_device_info["IMAGE3D_MAX_WIDTH"] = ss.str();
    ss.str(std::string());
    
    CHECK_CL_ERROR(clGetDeviceInfo(m_device,
                           CL_DEVICE_LOCAL_MEM_SIZE,
                           sizeof(cl_ulong),
                           &ulongBuf,
                           NULL));
    ss << ulongBuf;
    m_device_info["LOCAL_MEM_SIZE"] = ss.str();
    ss.str(std::string());
    
    CHECK_CL_ERROR(clGetDeviceInfo(m_device,
                           CL_DEVICE_LOCAL_MEM_TYPE,
                           sizeof(cl_device_local_mem_type),
                           &memTypeBuf,
                           NULL));
    m_device_info["LOCAL_MEM_TYPE"] = memTypeBuf == CL_LOCAL ? "LOCAL" : "GLOBAL";
    
    CHECK_CL_ERROR(clGetDeviceInfo(m_device,
                           CL_DEVICE_MAX_CLOCK_FREQUENCY,
                           sizeof(size_t),
                           &uintBuf,
                           NULL));
    ss << uintBuf;
    m_device_info["MAX_CLOCK_FREQUENCY"] = ss.str();
    ss.str(std::string());
    
    CHECK_CL_ERROR(clGetDeviceInfo(m_device,
                           CL_DEVICE_MAX_COMPUTE_UNITS,
                           sizeof(size_t),
                           &uintBuf,
                           NULL));
    ss << uintBuf;
    m_device_info["MAX_COMPUTE_UNITS"] = ss.str();
    ss.str(std::string());
    
    CHECK_CL_ERROR(clGetDeviceInfo(m_device,
                           CL_DEVICE_MAX_CONSTANT_ARGS,
                           sizeof(size_t),
                           &uintBuf,
                           NULL));
    ss << uintBuf;
    m_device_info["MAX_CONSTANT_ARGS"] = ss.str();
    ss.str(std::string());
    
    CHECK_CL_ERROR(clGetDeviceInfo(m_device,
                           CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE,
                           sizeof(cl_ulong),
                           &ulongBuf,
                           NULL));
    ss << ulongBuf;
    m_device_info["MAX_CONSTANT_BUFFER_SIZE"] = ss.str();
    ss.str(std::string());
    
    CHECK_CL_ERROR(clGetDeviceInfo(m_device,
                           CL_DEVICE_MAX_MEM_ALLOC_SIZE,
                           sizeof(cl_ulong),
                           &ulongBuf,
                           NULL));
    ss << ulongBuf;
    m_device_info["MAX_MEM_ALLOC_SIZE"] = ss.str();
    ss.str(std::string());
    
    CHECK_CL_ERROR(clGetDeviceInfo(m_device,
                           CL_DEVICE_MAX_PARAMETER_SIZE,
                           sizeof(size_t),
                           &uintBuf,
                           NULL));
    ss << uintBuf;
    m_device_info["MAX_PARAMETER_SIZE"] = ss.str();
    ss.str(std::string());
    
    CHECK_CL_ERROR(clGetDeviceInfo(m_device,
                           CL_DEVICE_MAX_READ_IMAGE_ARGS,
                           sizeof(size_t),
                           &uintBuf,
                           NULL));
    ss << uintBuf;
    m_device_info["MAX_READ_IMAGE_ARGS"] = ss.str();
    ss.str(std::string());
    
    CHECK_CL_ERROR(clGetDeviceInfo(m_device,
                           CL_DEVICE_MAX_SAMPLERS,
                           sizeof(size_t),
                           &uintBuf,
                           NULL));
    ss << uintBuf;
    m_device_info["MAX_SAMPLERS"] = ss.str();
    ss.str(std::string());
    
    CHECK_CL_ERROR(clGetDeviceInfo(m_device,
                           CL_DEVICE_MAX_WORK_GROUP_SIZE,
                           sizeof(size_t),
                           &uintBuf,
                           NULL));
    ss << uintBuf;
    m_device_info["MAX_WORK_GROUP_SIZE"] = ss.str();
    ss.str(std::string());
    
    CHECK_CL_ERROR(clGetDeviceInfo(m_device,
                           CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS,
                           sizeof(size_t),
                           &uintBuf,
                           NULL));
    ss << uintBuf;
    m_device_info["MAX_WORK_ITEM_DIMENSIONS"] = ss.str();
    ss.str(std::string());
    
    size_t* dimBuf = new size_t[uintBuf];
    CHECK_CL_ERROR(clGetDeviceInfo(m_device,
                          CL_DEVICE_MAX_WORK_ITEM_SIZES,
                          sizeof(size_t)*uintBuf,
                          dimBuf,
                          NULL));
    for (size_t i = 0; i < uintBuf-1; i++) {
        ss << dimBuf[i] << ", ";
    }
    ss << dimBuf[uintBuf-1];
    m_device_info["MAX_WORK_ITEM_SIZES"] = ss.str();
    ss.str(std::string());
    delete[] dimBuf;
    
    CHECK_CL_ERROR(clGetDeviceInfo(m_device,
                           CL_DEVICE_MAX_WRITE_IMAGE_ARGS,
                           sizeof(size_t),
                           &uintBuf,
                           NULL));
    ss << uintBuf;
    m_device_info["MAX_WRITE_IMAGE_ARGS"] = ss.str();
    ss.str(std::string());
    
    CHECK_CL_ERROR(clGetDeviceInfo(m_device,
                           CL_DEVICE_MEM_BASE_ADDR_ALIGN,
                           sizeof(size_t),
                           &uintBuf,
                           NULL));
    ss << uintBuf;
    m_device_info["MEM_BASE_ADDR_ALIGN"] = ss.str();
    ss.str(std::string());
    
    CHECK_CL_ERROR(clGetDeviceInfo(m_device,
                           CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE,
                           sizeof(size_t),
                           &uintBuf,
                           NULL));
    ss << uintBuf;
    m_device_info["MIN_DATA_TYPE_ALIGN_SIZE"] = ss.str();
    ss.str(std::string());
    
    CHECK_CL_ERROR(clGetDeviceInfo(m_device,
                           CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR,
                           sizeof(size_t),
                           &uintBuf,
                           NULL));
    ss << uintBuf;
    m_device_info["PREFERRED_VECTOR_WIDTH_CHAR"] = ss.str();
    ss.str(std::string());
    
    CHECK_CL_ERROR(clGetDeviceInfo(m_device,
                           CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT,
                           sizeof(size_t),
                           &uintBuf,
                           NULL));
    ss << uintBuf;
    m_device_info["PREFERRED_VECTOR_WIDTH_SHORT"] = ss.str();
    ss.str(std::string());
    
    CHECK_CL_ERROR(clGetDeviceInfo(m_device,
                           CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT,
                           sizeof(size_t),
                           &uintBuf,
                           NULL));
    ss << uintBuf;
    m_device_info["PREFERRED_VECTOR_WIDTH_INT"] = ss.str();
    ss.str(std::string());
    
    CHECK_CL_ERROR(clGetDeviceInfo(m_device,
                           CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG,
                           sizeof(size_t),
                           &uintBuf,
                           NULL));
    ss << uintBuf;
    m_device_info["PREFERRED_VECTOR_WIDTH_LONG"] = ss.str();
    ss.str(std::string());
    
    CHECK_CL_ERROR(clGetDeviceInfo(m_device,
                           CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT,
                           sizeof(size_t),
                           &uintBuf,
                           NULL));
    ss << uintBuf;
    m_device_info["PREFERRED_VECTOR_WIDTH_FLOAT"] = ss.str();
    ss.str(std::string());
    
    CHECK_CL_ERROR(clGetDeviceInfo(m_device,
                           CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE,
                           sizeof(size_t),
                           &uintBuf,
                           NULL));
    ss << uintBuf;
    m_device_info["PREFERRED_VECTOR_WIDTH_DOUBLE"] = ss.str();
    ss.str(std::string());
    
    CHECK_CL_ERROR(clGetDeviceInfo(m_device,
                           CL_DEVICE_PROFILING_TIMER_RESOLUTION,
                           sizeof(size_t),
                           &uintBuf,
                           NULL));
    ss << uintBuf;
    m_device_info["PROFILING_TIMER_RESOLUTION"] = ss.str();
    ss.str(std::string());
    
    CHECK_CL_ERROR(clGetDeviceInfo(m_device,
                           CL_DEVICE_TYPE,
                           sizeof(cl_device_type),
                           &typeBuf,
                           NULL));
    m_device_info["TYPE"] = extractDeviceTypeInfo(typeBuf);
    
    CHECK_CL_ERROR(clGetDeviceInfo(m_device,
                             CL_DEVICE_VENDOR,
                             512,
                             cstrBuf,
                             NULL));
    m_device_info["VENDOR"] = std::string(cstrBuf);
    
    CHECK_CL_ERROR(clGetDeviceInfo(m_device,
                           CL_DEVICE_VENDOR_ID,
                           sizeof(size_t),
                           &uintBuf,
                           NULL));
    ss << uintBuf;
    m_device_info["VENDOR_ID"] = ss.str();
    ss.str(std::string());
    
    CHECK_CL_ERROR(clGetDeviceInfo(m_device,
                             CL_DEVICE_VERSION,
                             512,
                             cstrBuf,
                             NULL));
    m_device_info["VERSION"] = std::string(cstrBuf);
    
    CHECK_CL_ERROR(clGetDeviceInfo(m_device,
                             CL_DRIVER_VERSION,
                             512,
                             cstrBuf,
                             NULL));
    m_device_info["DRIVER_VERSION"] = std::string(cstrBuf);

    CHECK_CL_ERROR(clGetDeviceInfo(m_device,
                            CL_DEVICE_NAME,
                            512,
                            cstrBuf,
                            NULL));
    m_device_info["NAME"] = std::string(cstrBuf);
}
    
std::string ComputeContext::extractFloatingPointInfo(cl_device_fp_config info){
    std::stringstream ss;
    if (info & CL_FP_DENORM) {
        ss << "FP_DENORM ";
    }
    if (info & CL_FP_INF_NAN) {
        ss << "FP_INF_NAN ";
    }
    if (info & CL_FP_ROUND_TO_NEAREST) {
        ss << "FP_ROUND_TO_NEAREST ";
    }
    if (info & CL_FP_ROUND_TO_ZERO) {
        ss << "FP_ROUND_TO_ZERO ";
    }
    if (info & CL_FP_ROUND_TO_INF) {
        ss << "FP_ROUND_TO_INF ";
    }
    //if (info & CP_FP_FMA) {
    //    ss << "FP_FMA ";
    //}
    return ss.str();
}
    
std::string ComputeContext::extractExecutionCapabilitiesInfo(cl_device_exec_capabilities info){
    std::stringstream ss;
    if (info & CL_EXEC_KERNEL) {
        ss << "EXEC_KERNEL ";
    }
    if (info & CL_EXEC_NATIVE_KERNEL) {
        ss << "EXEC_NATIVE_KERNEL ";
    }
    return ss.str();
}
    
std::string ComputeContext::extractDeviceTypeInfo(cl_device_type info){
    std::stringstream ss;
    if (info & CL_DEVICE_TYPE_CPU) {
        m_type = __CPU__;
        ss << "TYPE_CPU ";
    }
    if (info & CL_DEVICE_TYPE_GPU) {
        m_type = __GPU__;
        ss << "TYPE_GPU ";
    }
    if (info & CL_DEVICE_TYPE_ACCELERATOR) {
        m_type = __ACCELERATOR__;
        ss << "TYPE_ACCELERATOR ";
    }
    if (info & CL_DEVICE_TYPE_DEFAULT) {
        m_type = __UNKNOWN__;
        logger->log(Logger::WARNING, "Unknown device type, several features may not function as intended");
        ss << "TYPE_DEFAULT ";
    }
    return ss.str();
}

bool ComputeContext::isExtensionAvailable(const std::string &extension_name) {
    return (getDeviceInfo("EXTENSIONS").find(extension_name) != std::string::npos);
}

bool ComputeContext::usingRenderInteroperability() {
    return m_render_interoperability;
}

DeviceType ComputeContext::type() {
    return m_type;
}

int ComputeContext::version() {
    return m_cl_version;
}
}










