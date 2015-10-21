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



/**
* \file header.cl
* Global Built-in Device code functions.
*/


/**
 * Extended OpenCL vector types.
 *
 * These type names are reserved in OpenCL 1.* and might be
 * added in a later version of OpenCL and may then be
 * removed here. Probably....
 */

// Fallback for other platforms are not yet implemented
/// @cond DISABLE
#define __NVIDIA__

#if defined(__NVIDIA__) //|| defined(__INTEL__) // Untested

typedef float __attribute__((ext_vector_type(5))) __float5;
typedef float __attribute__((ext_vector_type(6))) __float6;
typedef float __attribute__((ext_vector_type(7))) __float7;
typedef float __attribute__((ext_vector_type(9))) __float9;
typedef float __attribute__((ext_vector_type(10))) __float10;
typedef float __attribute__((ext_vector_type(11))) __float11;
typedef float __attribute__((ext_vector_type(12))) __float12;
typedef float __attribute__((ext_vector_type(13))) __float13;
typedef float __attribute__((ext_vector_type(14))) __float14;
typedef float __attribute__((ext_vector_type(15))) __float15;

#else

typedef union{
    float v1[5];
    struct {float x,y,z,w;};
    struct {float s0,s1,s2,s3,s4;};
} __float5;

typedef union{
    float v1[6];
    struct {float x,y,z,w;};
    struct {float s0,s1,s2,s3,s4,s5;};
} __float6;

typedef union{
    float v1[7];
    struct {float x,y,z,w;};
    struct {float s0,s1,s2,s3,s4,s5,s6;};
} __float7;

typedef union{
    float v1[9];
    struct {float x,y,z,w;};
    struct {float s0,s1,s2,s3,s4,s5,s6,s7,s8;};
} __float9;

typedef union{
    float v1[10];
    struct {float x,y,z,w;};
    struct {float s0,s1,s2,s3,s4,s5,s6,s7,s8,s9;};
} __float10;

typedef union{
    float v1[11];
    struct {float x,y,z,w;};
    struct {float s0,s1,s2,s3,s4,s5,s6,s7,s8,s9,sA;};
} __float11;

typedef union{
    float v1[12];
    struct {float x,y,z,w;};
    struct {float s0,s1,s2,s3,s4,s5,s6,s7,s8,s9,sA,sB;};
} __float12;

typedef union{
    float v1[13];
    struct {float x,y,z,w;};
    struct {float s0,s1,s2,s3,s4,s5,s6,s7,s8,s9,sA,sB,sC;};
} __float13;

typedef union{
    float v1[14];
    struct {float x,y,z,w;};
    struct {float s0,s1,s2,s3,s4,s5,s6,s7,s8,s9,sA,sB,sC,sD;};
} __float14;

typedef union{
    float v1[15];
    struct {float x,y,z,w;};
    struct {float s0,s1,s2,s3,s4,s5,s6,s7,s8,s9,sA,sB,sC,sD,sE;};
} __float15;

#endif

#define float5 __float5
#define float6 __float6
#define float7 __float7
#define float9 __float9
#define float10 __float10
#define float11 __float11
#define float12 __float12
#define float13 __float13
#define float14 __float14
#define float15 __float15
/// @endcond

/**
 * Common functions
 */
#if (_DIMENSIONS_ == 1)
/**
*  Retruns the array index based on the cell number and an offset
*/
size_t get_index(uint cell, int offset);

/**
* Retruns the index to the mirror cell
*/
int get_mirror_cell_offset(uint cell);

/**
* Fetch the value at a cell
*/
float  fetch(__global float* array, uint cell);

/**
* Fetch the value at a cell
*/
float2  fetch2(__global float** array, uint cell);

/**
* Fetch the value at a cell
*/
float3  fetch3(__global float** array, uint cell);

/**
* Fetch the value at a cell
*/
float4  fetch4(__global float** array, uint cell);

/**
* Fetch the value at a cell
*/
float5  fetch5(__global float** array, uint cell);

/**
* Fetch the value at a cell
*/
float6  fetch6(__global float** array, uint cell);

/**
* Fetch the value at a cell
*/
float8  fetch8(__global float** array, uint cell);

/**
* Store a value at a cell number
*/
void   store(__global float* array, float value, uint cell);

/**
* Store a value at a cell number
*/
void   store2(__global float** array, float2 value, uint cell);

/**
* Store a value at a cell number
*/
void   store3(__global float** array, float3 value, uint cell);

/**
* Store a value at a cell number
*/
void   store4(__global float** array, float4 value, uint cell);

/**
* Store a value at a cell number
*/
void   store5(__global float** array, float5 value, uint cell);

/**
* Store a value at a cell number
*/
void   store6(__global float** array, float6 value, uint cell);

/**
* Store a value at a cell number
*/
void   store8(__global float** array, float8 value, uint cell);

/**
* Fetch with an offset
*/
float  fetch_offset(__global float* array, uint cell, int offset);

/**
* Fetch with an offset
*/
float2  fetch2_offset(__global float** array, uint cell, int offset);

/**
* Fetch with an offset
*/
float3  fetch3_offset(__global float** array, uint cell, int offset);

/**
* Fetch with an offset
*/
float4  fetch4_offset(__global float** array, uint cell, int offset);

/**
* Fetch with an offset
*/
float5  fetch5_offset(__global float** array, uint cell, int offset);

/**
* Fetch with an offset
*/
float6  fetch6_offset(__global float** array, uint cell, int offset);

/**
* Fetch with an offset
*/
float8  fetch8_offset(__global float** array, uint cell, int offset);

/**
* Store with an offset
*/
void   store_offset(__global float* array, float value, uint cell, int offset);

/**
* Store with an offset
*/
void   store2_offset(__global float** array, float2 value, uint cell, int offset);

/**
* Store with an offset
*/
void   store3_offset(__global float** array, float3 value, uint cell, int offset);

/**
* Store with an offset
*/
void   store4_offset(__global float** array, float4 value, uint cell, int offset);

/**
* Store with an offset
*/
void   store5_offset(__global float** array, float5 value, uint cell, int offset);

/**
* Store with an offset
*/
void   store6_offset(__global float** array, float6 value, uint cell, int offset);

/**
* Store with an offset
*/
void   store8_offset(__global float** array, float8 value, uint cell, int offset);

/**
* Fetch inner domain mirror cell
*/
float fetch_mirror(__global float* array, uint cell);

/**
* Fetch inner domain mirror cell
*/
float2 fetch2_mirror(__global float** array, uint cell);

/**
* Fetch inner domain mirror cell
*/
float3 fetch3_mirror(__global float** array, uint cell);

/**
* Fetch inner domain mirror cell
*/
float4 fetch4_mirror(__global float** array, uint cell);



#elif (_DIMENSIONS_ == 2)
size_t get_index(uint2 cell, int2 offset);
int2 get_mirror_cell_offset(uint2 cell);

float  fetch(__global float* array, uint2 cell);
float2  fetch2(__global float** array, uint2 cell);
float3  fetch3(__global float** array, uint2 cell);
float4  fetch4(__global float** array, uint2 cell);
float5  fetch5(__global float** array, uint2 cell);
float6  fetch6(__global float** array, uint2 cell);
float8  fetch8(__global float** array, uint2 cell);
void   store(__global float* array, float value, uint2 cell);
void   store2(__global float** array, float2 value, uint2 cell);
void   store3(__global float** array, float3 value, uint2 cell);
void   store4(__global float** array, float4 value, uint2 cell);
void   store5(__global float** array, float5 value, uint2 cell);
void   store6(__global float** array, float6 value, uint2 cell);
void   store8(__global float** array, float8 value, uint2 cell);
float  fetch_offset(__global float* array, uint2 cell, int2 offset);
float2  fetch2_offset(__global float** array, uint2 cell, int2 offset);
float3  fetch3_offset(__global float** array, uint2 cell, int2 offset);
float4  fetch4_offset(__global float** array, uint2 cell, int2 offset);
float5  fetch5_offset(__global float** array, uint2 cell, int2 offset);
float6  fetch6_offset(__global float** array, uint2 cell, int2 offset);
float8  fetch8_offset(__global float** array, uint2 cell, int2 offset);
void   store_offset(__global float* array, float value, uint2 cell, int2 offset);
void   store2_offset(__global float** array, float2 value, uint2 cell, int2 offset);
void   store3_offset(__global float** array, float3 value, uint2 cell, int2 offset);
void   store4_offset(__global float** array, float4 value, uint2 cell, int2 offset);
void   store5_offset(__global float** array, float5 value, uint2 cell, int2 offset);
void   store6_offset(__global float** array, float6 value, uint2 cell, int2 offset);
void   store8_offset(__global float** array, float8 value, uint2 cell, int2 offset);
float fetch_mirror(__global float* array, uint2 cell);
float2 fetch2_mirror(__global float** array, uint2 cell);
float3 fetch3_mirror(__global float** array, uint2 cell);
float4 fetch4_mirror(__global float** array, uint2 cell);
float5 fetch5_mirror(__global float** array, uint2 cell);
float6 fetch6_mirror(__global float** array, uint2 cell);

#elif (_DIMENSIONS_ == 3)
size_t get_index(uint3 cell, int3 offset);
int3 get_mirror_cell_offset(uint3 cell);

float  fetch(__global float* array, uint3 cell);
float2  fetch2(__global float** array, uint3 cell);
float3  fetch3(__global float** array, uint3 cell);
float4  fetch4(__global float** array, uint3 cell);
float5  fetch5(__global float** array, uint3 cell);
float6  fetch6(__global float** array, uint3 cell);
float8  fetch8(__global float** array, uint3 cell);
void   store(__global float* array, float value, uint3 cell);
void   store2(__global float** array, float2 value, uint3 cell);
void   store3(__global float** array, float3 value, uint3 cell);
void   store4(__global float** array, float4 value, uint3 cell);
void   store5(__global float** array, float5 value, uint3 cell);
void   store6(__global float** array, float6 value, uint3 cell);
void   store8(__global float** array, float8 value, uint3 cell);
float  fetch_offset(__global float* array, uint3 cell, int3 offset);
float2  fetch2_offset(__global float** array, uint3 cell, int3 offset);
float3  fetch3_offset(__global float** array, uint3 cell, int3 offset);
float4  fetch4_offset(__global float** array, uint3 cell, int3 offset);
float5  fetch5_offset(__global float** array, uint3 cell, int3 offset);
float6  fetch6_offset(__global float** array, uint3 cell, int3 offset);
float8  fetch8_offset(__global float** array, uint3 cell, int3 offset);
void   store_offset(__global float* array, float value, uint3 cell, int3 offset);
void   store2_offset(__global float** array, float2 value, uint3 cell, int3 offset);
void   store3_offset(__global float** array, float3 value, uint3 cell, int3 offset);
void   store4_offset(__global float** array, float4 value, uint3 cell, int3 offset);
void   store5_offset(__global float** array, float5 value, uint3 cell, int3 offset);
void   store6_offset(__global float** array, float6 value, uint3 cell, int3 offset);
void   store8_offset(__global float** array, float8 value, uint3 cell, int3 offset);
float fetch_mirror(__global float* array, uint3 cell);
float2 fetch2_mirror(__global float** array, uint3 cell);
float3 fetch3_mirror(__global float** array, uint3 cell);
float4 fetch4_mirror(__global float** array, uint3 cell);
float5 fetch5_mirror(__global float** array, uint3 cell);
float6 fetch6_mirror(__global float** array, uint3 cell);

#endif

/**
* Built in limiter functions
*/

/**
* __attribute__((overloadable))
* Intel and Nvidia support overloadable attributes,
* not AMD. So sad.
*/
//float __attribute__((overloadable)) minmod(float a, float b);
//float __attribute__((overloadable)) minmod(float a, float b, float c);

/**
* Minmod function with two parameters
*/
float minmodAB(float a, float b);
float2 minmodAB2(float2 a, float2 b);
float3 minmodAB3(float3 a, float3 b);
float4 minmodAB4(float4 a, float4 b);
/**
* Minmod function with three parameters
*/
float minmodABC(float a, float b, float c);
float2 minmodABC2(float2 a, float2 b, float2 c);
float3 minmodABC3(float3 a, float3 b, float3 c);
float4 minmodABC4(float4 a, float4 b, float4 c);
float6 minmodABC6(float6 a, float6 b, float6 c);

/**
* Maxmod function with two parameters
*/
float maxmod(float a, float b);
float2 maxmod2(float2 a, float2 b);
float3 maxmod3(float3 a, float3 b);
float4 maxmod4(float4 a, float4 b);


/**
* Slope function using minmod
*/
float minmod_slope(float Q, float Qneg, float Qpos);

/**
* Superbee slope function
*/
float superbee_slope(float Q, float Qneg, float Qpos);

/**
* MC slope limiter function
*/
float mclimiter_slope(float Q, float Qneg, float Qpos);

/**
* Built in container for reconstructed 2nd order integration points
*/
typedef union {
    /*__local*/ float v[18];

    // Linear reconstruction
    struct {
        float left;
        float left_plus;
        float left_minus;

        float right;
        float right_plus;
        float right_minus;
    };

    // Planar reconstruction
    struct {
        float north;
        float north_plus;
        float north_minus;

        float east;
        float east_plus;
        float east_minus;

        float south;
        float south_plus;
        float south_minus;

        float west;
        float west_plus;
        float west_minus;
    };

    // Cubic(?) reconstruction
    struct {
        float spacer0, spacer1, spacer2,
                spacer3, spacer4, spacer5;

        float top;
        float top_plus;
        float top_minus;

        float bottom;
        float bottom_plus;
        float bottom_minus;

        float front;
        float front_plus;
        float front_minus;

        float back;
        float back_plus;
        float back_minus;
    };
} integration_points;
typedef union {
    /*__local*/ float2 v[18];

    // Linear reconstruction
    struct {
        float2 left;
        float2 left_plus;
        float2 left_minus;

        float2 right;
        float2 right_plus;
        float2 right_minus;
    };

    // Planar reconstruction
    struct {
        float2 north;
        float2 north_plus;
        float2 north_minus;

        float2 east;
        float2 east_plus;
        float2 east_minus;

        float2 south;
        float2 south_plus;
        float2 south_minus;

        float2 west;
        float2 west_plus;
        float2 west_minus;
    };

    // Cubic(?) reconstruction
    struct {
        float2 spacer0, spacer1, spacer2,
                spacer3, spacer4, spacer5;

        float2 top;
        float2 top_plus;
        float2 top_minus;

        float2 bottom;
        float2 bottom_plus;
        float2 bottom_minus;

        float2 front;
        float2 front_plus;
        float2 front_minus;

        float2 back;
        float2 back_plus;
        float2 back_minus;
    };
} integration_points2;
typedef union {
    /*__local*/ float3 v[18];

    // Linear reconstruction
    struct {
        float3 left;
        float3 left_plus;
        float3 left_minus;

        float3 right;
        float3 right_plus;
        float3 right_minus;
    };

    // Planar reconstruction
    struct {
        float3 north;
        float3 north_plus;
        float3 north_minus;

        float3 east;
        float3 east_plus;
        float3 east_minus;

        float3 south;
        float3 south_plus;
        float3 south_minus;

        float3 west;
        float3 west_plus;
        float3 west_minus;
    };

    // Cubic(?) reconstruction
    struct {
        float3 spacer0, spacer1, spacer2,
                spacer3, spacer4, spacer5;

        float3 top;
        float3 top_plus;
        float3 top_minus;

        float3 bottom;
        float3 bottom_plus;
        float3 bottom_minus;

        float3 front;
        float3 front_plus;
        float3 front_minus;

        float3 back;
        float3 back_plus;
        float3 back_minus;
    };
} integration_points3;
typedef union {
    /*__local*/ float4 v[18];

    // Linear reconstruction
    struct {
        float4 left;
        float4 left_plus;
        float4 left_minus;

        float4 right;
        float4 right_plus;
        float4 right_minus;
    };

    // Planar reconstruction
    struct {
        float4 north;
        float4 north_plus;
        float4 north_minus;

        float4 east;
        float4 east_plus;
        float4 east_minus;

        float4 south;
        float4 south_plus;
        float4 south_minus;

        float4 west;
        float4 west_plus;
        float4 west_minus;
    };

    // Cubic(?) reconstruction
    struct {
        float4 spacer0, spacer1, spacer2,
                spacer3, spacer4, spacer5;

        float4 top;
        float4 top_plus;
        float4 top_minus;

        float4 bottom;
        float4 bottom_plus;
        float4 bottom_minus;

        float4 front;
        float4 front_plus;
        float4 front_minus;

        float4 back;
        float4 back_plus;
        float4 back_minus;
    };
} integration_points4;

/**
* Built in 2nd order reconstruction functions
*/

/**
* Linear reconstruction for 1D problems
*/
void linear(__global float* data, uint cell, integration_points* out);
void linear2(__global float** data, uint cell, integration_points2* out);
void linear3(__global float** data, uint cell, integration_points3* out);
void linear4(__global float** data, uint cell, integration_points4* out);

/**
* Planar reconstruction for 2D problems
*/
void planar(__global float* data, uint2 cell, integration_points* out);
void planar2(__global float** data, uint2 cell, integration_points2* out);
void planar3(__global float** data, uint2 cell, integration_points3* out);
void planar4(__global float** data, uint2 cell, integration_points4* out);


/**
* Cubic reconstruction for 3D problems
*/
void cubic(__global float* data, uint3 cell, integration_points* out);
void cubic2(__global float** data, uint3 cell, integration_points2* out);
void cubic3(__global float** data, uint3 cell, integration_points3* out);
void cubic4(__global float** data, uint3 cell, integration_points4* out);

/**
 * Custom functions
 */

/**
 * Custom sign function for vectors with 6 elements
 */
float6 __OVERLOADABLE__ sign(float6 data);

/**
 * Custom fabs function for vectors with 6 elements
 */
float6 __OVERLOADABLE__ fabs(float6 data);

/**
 * Custom min function for vectors with 6 elements
 */
float6 __OVERLOADABLE__ min(float6 v1, float6 v2);

