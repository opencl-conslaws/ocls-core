#ifndef C_STRUCTURES_H
#define C_STRUCTURES_H

#ifdef __cplusplus
#include <cstdint>
#include <cstddef>
#else
#include "stdint.h"
#include "stddef.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
*  \defgroup c_wrap_error_codes Error Codes
*  \ingroup c_wrap
*  @{
*/
#define ERROR_OK 0
#define ERROR_FATAL -1
/** @}*/

enum{
 X = 1, Y = 2, Z = 3
};

static char error_buffer[2048];

/**
* Highway to the ...
* http://youtu.be/yK0P1Bk8Cx4
*/
struct oclsPtrHandle{};
typedef struct oclsPtrHandle* PointerHandle;

typedef PointerHandle Data;
typedef PointerHandle CLSSource;
typedef PointerHandle Framework;

struct CallableFunctionHandle;
typedef struct CallableFunctionHandle* CallableFunction;

typedef CallableFunction BoundaryFunction;
typedef CallableFunction FluxFunction;
typedef CallableFunction IntegratorFunction;


/**
*  \ingroup c_wrap_domain
*  @{
*/
typedef struct {
    float min;
    float max;
}oclsBoundary;
typedef oclsBoundary Boundary;

/**
*  \ingroup c_wrap_domain
*  @{
*/
typedef struct {
    unsigned short dimensions;
    Boundary bounds[3];
    size_t mesh[3];
    unsigned short ghost_cells;
}oclsDomain;
typedef oclsDomain Domain;

typedef struct {
    void* value;
    size_t size;
    int allocation;
}oclsParameter;
typedef oclsParameter Parameter;

#define TO_PARAMETER(x) {&x, sizeof(typeof(x))}


typedef struct {
    int count;

    float total_time;
    float total_api_overhead;
    float total_execution_time;

    float average_time;
    float average_api_overhead;
    float average_execution_time;
} KernelAnalysis;

typedef struct {
    int count;

    double total_time ;
    long long total_bytes;
    double total_rate;

    double average_time;
    long long average_bytes;
    double average_rate;
} MemEntry;

typedef struct  {
    MemEntry device_device;
    MemEntry device_host;
    MemEntry host_device;
} MemoryAnalysis;

#ifdef __cplusplus
}
#endif

#endif