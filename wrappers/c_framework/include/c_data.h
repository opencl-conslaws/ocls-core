#ifndef C_DATA_H
#define C_DATA_H

#include "c_structures.h"

#ifdef __cplusplus
extern "C" {
#endif
/**
*  \defgroup c_wrap_data Data
*  \ingroup c_wrap
*  @{
*/

/**
* Returns a raw data pointer
*/
int oclsGetDataPointer(Data d, float** ptr_out);

/**
* Returns a raw data pointer
*/
int oclsGetDataPointerWithBounds(Data d, size_t bounds[3], float** ptr_out);

/**
* Get the data parameter for launching functions
*/
Parameter oclsGetParameter(Data d);

/**
* Upload data from raw pointer
*/
int oclsUpload(Data d, float* data, size_t* size, size_t* offset);

/**
* Get data at cell
*/
float oclsAtX(Data d, size_t x);
int oclsAtXSafe(Data d, size_t x, float* out);

/**
* Get data at cell
*/
float oclsAtXY(Data d, size_t x, size_t y);
int oclsAtXYSafe(Data d, size_t x, size_t y, float* out);

/**
* Get data at cell
*/
float oclsAtXYZ(Data d, size_t x, size_t y, size_t z);
int oclsAtXYZSafe(Data d, size_t x, size_t y, size_t z, float* out);

/**
* Get data between data objects
*/
int oclsCopy(Data src, Data dest);

/**
* Find maximum value in data set
*/
int oclsFindMax(Data d, float* value_out);

/**
* Find minimum value in data set
*/
int oclsFindMin(Data d, float* value_out);

/**
* Returns the dimensions of the data object
*/
int oclsDimensions(Data d);

/**
* Get the size of the data object
*/
int oclsSize(Data d, size_t* size);

/** @}*/

#ifdef __cplusplus
}
#endif

#endif

