/**
* C Wrapper
*/

#ifndef C_FRAMEWORK_H
#define C_FRAMEWORK_H

#ifdef __cplusplus
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>

#else
#include "stdlib.h"
#include "stdio.h"
#include "stdint.h"
#include "stddef.h"
#endif

#include "c_structures.h"
#include "c_domain.h"
#include "c_data.h"
#include "c_analyser.h"

#ifdef __cplusplus
extern "C" {
#endif
/** \defgroup c_wrap C API
*  @{
*  \defgroup c_wrap_framework Framework
*  \ingroup c_wrap
*  @{
*/

/**
* Return the last error
*/
const char* oclsGetError(void);

/**
* Initialize API
*/
int oclsInit(Framework* framework);

/**
* Initialize API
*/
int oclsInitByType(Framework* framework, int type);

/**
* Destroy and clean up API
*/
int oclsClean(Framework framework);

/**
* Load source
*/
int oclsLoadFromFile(Framework framework, const char* path, CLSSource* source_out);
/**
* Load source
*/
int oclsLoadFromString(Framework framework, const char* src, CLSSource* source_out);

/**
* Create data memset to zero
*/
int oclsCreateData(Framework framework, Domain domain, const char* ref, Data* data_out);

/**
* Create data from initial function
*/
int oclsCreateDataFromFunction(Framework framework, Domain domain, const char* ref, CLSSource src, const char* function, Data* data_out);

/**
* Creates a boundary function
*/
int oclsCreateBoundaryFunction(Framework framework, Domain domain, CLSSource src, const char* function, BoundaryFunction* func_out);

/**
* Create a flux function
*/
int oclsCreateFluxFunction(Framework framework, Domain domain, CLSSource src, const char* function, FluxFunction* func_out);

/**
* Create a Integrator function
*/
int oclsCreateIntegratorFunction(Framework framework, Domain domain, CLSSource src, const char* function, IntegratorFunction* func_out);

/**
* Call a functions
*/
int oclsCall(CallableFunction function, size_t length, Parameter* args, Data* return_out, int* return_length);

/**
* @}
* @}*/
#ifdef __cplusplus
}
#endif

#endif