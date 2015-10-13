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
 * Common functions
 */
#if (_DIMENSIONS_ == 1)
size_t get_index(uint cell, int offset){
#if defined(_BOUNDARY_PROGRAM_)
    int b_offset = 0;
    if(POS_X){
        b_offset = Nx+GC;
    }
    return cell+offset+b_offset;
#elif defined(_INTEGRATOR_PROGRAM_)
    return cell+offset;
#elif defined(_FLUX_PROGRAM_)
    return cell+offset;
#else
    return cell+offset;
#endif
}

int get_mirror_cell_offset(uint cell){
#if defined(_BOUNDARY_PROGRAM_)
    if(NEG_X){
        return GC*2-1-cell*2;
    }else{
        return -cell-cell-1;
    }
#else
    return 0;
#endif
}

float  fetch(__global float* array, uint cell){
    return fetch_offset(array, cell, 0);
}
float2  fetch2(__global float** array, uint cell){
    return fetch2_offset(array, cell, 0);
}

float3  fetch3(__global float** array, uint cell){
    return fetch3_offset(array, cell, 0);
}

float4  fetch4(__global float** array, uint cell){
    return fetch4_offset(array, cell, 0);
}

float5  fetch5(__global float** array, uint cell){
    return fetch5_offset(array, cell, 0);
}

float6  fetch6(__global float** array, uint cell){
    return fetch6_offset(array, cell, 0);
}

float8  fetch8(__global float** array, uint cell){
    return fetch8_offset(array, cell, 0);
}

void   store(__global float* array, float value, uint cell){
    store_offset(array, value, cell, 0);
}
void   store2(__global float** array, float2 value, uint cell){
    store2_offset(array, value, cell, 0);
}

void   store3(__global float** array, float3 value, uint cell){
    store3_offset(array, value, cell, 0);
}

void   store4(__global float** array, float4 value, uint cell){
    store4_offset(array, value, cell, 0);
}

void   store5(__global float** array, float5 value, uint cell){
    store5_offset(array, value, cell, 0);
}

void   store6(__global float** array, float6 value, uint cell){
    store6_offset(array, value, cell, 0);
}

void   store8(__global float** array, float8 value, uint cell){
    store8_offset(array, value, cell, 0);
}

float  fetch_offset(__global float* array, uint cell, int offset){
    return array[get_index(cell,offset)];
}

float2  fetch2_offset(__global float** array, uint cell, int offset){
    size_t index = get_index(cell,offset);
    return (float2)(array[0][index],
                    array[1][index]);
}

float3  fetch3_offset(__global float** array, uint cell, int offset){
    size_t index = get_index(cell,offset);
    return (float3)(array[0][index],
                    array[1][index],
                    array[2][index]);
}

float4  fetch4_offset(__global float** array, uint cell, int offset){
    size_t index = get_index(cell,offset);
    return (float4)(array[0][index],
                    array[1][index],
                    array[2][index],
                    array[3][index]);
}

float5  fetch5_offset(__global float** array, uint cell, int offset){
    size_t index = get_index(cell,offset);
    return (float5)(array[0][index],
                    array[1][index],
                    array[2][index],
                    array[3][index],
                    array[4][index]);
}

float6  fetch6_offset(__global float** array, uint cell, int offset){
    size_t index = get_index(cell,offset);
    return (float6)(array[0][index],
                    array[1][index],
                    array[2][index],
                    array[3][index],
                    array[4][index],
		    array[5][index]);
}

float8  fetch8_offset(__global float** array, uint cell, int offset){
    size_t index = get_index(cell,offset);
    return (float8)(array[0][index],
                    array[1][index],
                    array[2][index],
                    array[3][index],
                    array[4][index],
                    array[5][index],
                    array[6][index],
                    array[7][index]);
}

void   store_offset(__global float* array, float value, uint cell, int offset){
    array[get_index(cell,offset)] = value;
}

void   store2_offset(__global float** array, float2 value, uint cell, int offset){
    size_t index = get_index(cell,offset);
    array[0][index] = value.s0;
    array[1][index] = value.s1;
}

void   store3_offset(__global float** array, float3 value, uint cell, int offset){
    size_t index = get_index(cell,offset);
    array[0][index] = value.s0;
    array[1][index] = value.s1;
    array[2][index] = value.s2;
}

void   store4_offset(__global float** array, float4 value, uint cell, int offset){
    size_t index = get_index(cell,offset);
    array[0][index] = value.s0;
    array[1][index] = value.s1;
    array[2][index] = value.s2;
    array[3][index] = value.s3;
}

void   store5_offset(__global float** array, float5 value, uint cell, int offset){
    size_t index = get_index(cell,offset);
    array[0][index] = value.s0;
    array[1][index] = value.s1;
    array[2][index] = value.s2;
    array[3][index] = value.s3;
    array[4][index] = value.s4;
}

void   store6_offset(__global float** array, float6 value, uint cell, int offset){
    size_t index = get_index(cell,offset);
    array[0][index] = value.s0;
    array[1][index] = value.s1;
    array[2][index] = value.s2;
    array[3][index] = value.s3;
    array[4][index] = value.s4;
    array[5][index] = value.s5;
}

void   store8_offset(__global float** array, float8 value, uint cell, int offset){
    size_t index = get_index(cell,offset);
    array[0][index] = value.s0;
    array[1][index] = value.s1;
    array[2][index] = value.s2;
    array[3][index] = value.s3;
    array[4][index] = value.s4;
    array[5][index] = value.s5;
    array[6][index] = value.s6;
    array[7][index] = value.s7;
}

float fetch_mirror(__global float* array, uint cell){
    return fetch_offset(array, cell, get_mirror_cell_offset(cell));
}
float2 fetch2_mirror(__global float** array, uint cell){
    return fetch2_offset(array, cell, get_mirror_cell_offset(cell));
}
float3 fetch3_mirror(__global float** array, uint cell){
    return fetch3_offset(array, cell, get_mirror_cell_offset(cell));
}
float4 fetch4_mirror(__global float** array, uint cell){
    return fetch4_offset(array, cell, get_mirror_cell_offset(cell));
}

#elif (_DIMENSIONS_ == 2)
size_t get_index(uint2 cell, int2 offset){
#if defined(_BOUNDARY_PROGRAM_)
    int2 b_offset = (int2)(0,0);
    if(NEG_X){
        b_offset = (int2)(Nx+GC,0);
    }
    if(NEG_Y){
        b_offset = (int2)(0,Ny+GC);
    }
    return ((NxG) * (cell.y+offset.y+b_offset.y) + (cell.x+offset.x+b_offset.x));
#elif defined(_INTEGRATOR_PROGRAM_)
    return ((NxG) * (cell.y+offset.y) + (cell.x+offset.x));
#elif defined(_FLUX_PROGRAM_)
    return ((NxG) * (cell.y+offset.y) + (cell.x+offset.x));
#else
    return ((NxG) * (cell.y+offset.y) + (cell.x+offset.x));
#endif
}

int2 get_mirror_cell_offset(uint2 cell){
#if defined(_BOUNDARY_PROGRAM_)
    if(NEG_X){
        return (int2)(-cell.x-cell.x-1,0);
    }else if(POS_X){
        return (int2)(GC*2-1-cell.x*2,0);
    }else if(POS_Y){
        return (int2)(0,GC*2-1-cell.y*2);
    }else if(NEG_Y){
        return (int2)(0,-cell.y-cell.y-1);
    }
#else
    return (int2)(0,0);
#endif
}

float  fetch(__global float* array, uint2 cell){
    return fetch_offset(array, cell, (int2)(0,0));
}
float2  fetch2(__global float** array, uint2 cell){
    return fetch2_offset(array, cell, (int2)(0,0));
}
float3  fetch3(__global float** array, uint2 cell){
    return fetch3_offset(array, cell, (int2)(0,0));
}
float4  fetch4(__global float** array, uint2 cell){
    return fetch4_offset(array, cell, (int2)(0,0));
}
float5  fetch5(__global float** array, uint2 cell){
    return fetch5_offset(array, cell, (int2)(0,0));
}
float6  fetch6(__global float** array, uint2 cell){
    return fetch6_offset(array, cell, (int2)(0,0));
}
float8  fetch8(__global float** array, uint2 cell){
    return fetch8_offset(array, cell, (int2)(0,0));
}

void   store(__global float* array, float value, uint2 cell){
    store_offset(array, value, cell, (int2)(0,0));
}
void   store2(__global float** array, float2 value, uint2 cell){
    store2_offset(array, value, cell, (int2)(0,0));
}
void   store3(__global float** array, float3 value, uint2 cell){
    store3_offset(array, value, cell, (int2)(0,0));
}
void   store4(__global float** array, float4 value, uint2 cell){
    store4_offset(array, value, cell, (int2)(0,0));
}
void   store5(__global float** array, float5 value, uint2 cell){
    store5_offset(array, value, cell, (int2)(0,0));
}
void   store6(__global float** array, float6 value, uint2 cell){
    store6_offset(array, value, cell, (int2)(0,0));
}
void   store8(__global float** array, float8 value, uint2 cell){
    store8_offset(array, value, cell, (int2)(0,0));
}

float  fetch_offset(__global float* array, uint2 cell, int2 offset){
    return array[get_index(cell,offset)];
}
float2  fetch2_offset(__global float** array, uint2 cell, int2 offset){
    size_t index = get_index(cell,offset);
    return (float2)(array[0][index],
                    array[1][index]);
}
float3  fetch3_offset(__global float** array, uint2 cell, int2 offset){
    size_t index = get_index(cell,offset);
    return (float3)(array[0][index],
                    array[1][index],
                    array[2][index]);
}
float4  fetch4_offset(__global float** array, uint2 cell, int2 offset){
    size_t index = get_index(cell,offset);
    return (float4)(array[0][index],
                    array[1][index],
                    array[2][index],
                    array[3][index]);
}
float5  fetch5_offset(__global float** array, uint2 cell, int2 offset){
    size_t index = get_index(cell,offset);
    return (float5)(array[0][index],
                    array[1][index],
                    array[2][index],
                    array[3][index],
                    array[4][index]);
}
float6  fetch6_offset(__global float** array, uint2 cell, int2 offset){
    size_t index = get_index(cell,offset);
    return (float6)(array[0][index],
                    array[1][index],
                    array[2][index],
                    array[3][index],
                    array[4][index],
		    array[5][index]);
}
float8  fetch8_offset(__global float** array, uint2 cell, int2 offset){
    size_t index = get_index(cell,offset);
    return (float8)(array[0][index],
                    array[1][index],
                    array[2][index],
                    array[3][index],
                    array[4][index],
                    array[5][index],
                    array[6][index],
                    array[7][index]);
}

void   store_offset(__global float* array, float value, uint2 cell, int2 offset){
    array[get_index(cell,offset)] = value;
}

void   store2_offset(__global float** array, float2 value, uint2 cell, int2 offset){
    size_t index = get_index(cell,offset);
    array[0][index] = value.s0;
    array[1][index] = value.s1;
}

void   store3_offset(__global float** array, float3 value, uint2 cell, int2 offset){
    size_t index = get_index(cell,offset);
    array[0][index] = value.s0;
    array[1][index] = value.s1;
    array[2][index] = value.s2;
}

void   store4_offset(__global float** array, float4 value, uint2 cell, int2 offset){
    size_t index = get_index(cell,offset);
    array[0][index] = value.s0;
    array[1][index] = value.s1;
    array[2][index] = value.s2;
    array[3][index] = value.s3;
}
void   store5_offset(__global float** array, float5 value, uint2 cell, int2 offset){
    size_t index = get_index(cell,offset);
    array[0][index] = value.s0;
    array[1][index] = value.s1;
    array[2][index] = value.s2;
    array[3][index] = value.s3;
    array[4][index] = value.s4;
}

void   store6_offset(__global float** array, float6 value, uint2 cell, int2 offset){
    size_t index = get_index(cell,offset);
    array[0][index] = value.s0;
    array[1][index] = value.s1;
    array[2][index] = value.s2;
    array[3][index] = value.s3;
    array[4][index] = value.s4;
    array[5][index] = value.s5;
}

void   store8_offset(__global float** array, float8 value, uint2 cell, int2 offset){
    size_t index = get_index(cell,offset);
    array[0][index] = value.s0;
    array[1][index] = value.s1;
    array[2][index] = value.s2;
    array[3][index] = value.s3;
    array[4][index] = value.s4;
    array[5][index] = value.s5;
    array[6][index] = value.s6;
    array[7][index] = value.s7;
}

float fetch_mirror(__global float* array, uint2 cell){
    return fetch_offset(array, cell, get_mirror_cell_offset(cell));
}
float2 fetch2_mirror(__global float** array, uint2 cell){
    return fetch2_offset(array, cell, get_mirror_cell_offset(cell));
}
float3 fetch3_mirror(__global float** array, uint2 cell){
    return fetch3_offset(array, cell, get_mirror_cell_offset(cell));
}
float4 fetch4_mirror(__global float** array, uint2 cell){
    return fetch4_offset(array, cell, get_mirror_cell_offset(cell));
}
float5 fetch5_mirror(__global float** array, uint2 cell){
    return fetch5_offset(array, cell, get_mirror_cell_offset(cell));
}
float6 fetch6_mirror(__global float** array, uint2 cell){
    return fetch6_offset(array, cell, get_mirror_cell_offset(cell));
}

#elif (_DIMENSIONS_ == 3)
size_t get_index(uint3 cell, int3 offset){
#if defined(_BOUNDARY_PROGRAM_)
    int3 b_offset = (int3)(0,0,0);
    if(POS_X){
        b_offset = (int3)(Nx+GC,0,0);
    }
    if(NEG_Y){
        b_offset = (int3)(0,Ny+GC,0);
    }
    if(NEG_Z){
        b_offset = (int3)(0,0,Nz+GC);
    }
    return ((cell.z+offset.z+b_offset.z) * (NyG) * (NxG)) + ((cell.y+offset.y+b_offset.y) * (NxG)) + (cell.x+offset.x+b_offset.x);
#elif defined(_INTEGRATOR_PROGRAM_)
    return ((cell.z+offset.z) * (NyG) * (NxG)) + ((cell.y+offset.y) * (NxG)) + (cell.x+offset.x);
#elif defined(_FLUX_PROGRAM_)
    return ((cell.z+offset.z) * (NyG) * (NxG)) + ((cell.y+offset.y) * (NxG)) + (cell.x+offset.x);
#else
    return ((cell.z+offset.z) * (NyG) * (NxG)) + ((cell.y+offset.y) * (NxG)) + (cell.x+offset.x);
#endif
}

int3 get_mirror_cell_offset(uint3 cell){
#if defined(_BOUNDARY_PROGRAM_)
    if(POS_X){
        return (int3)(-cell.x-cell.x-1,0,0);
    }else if(NEG_X){
        return (int3)(GC*2-1-cell.x*2,0,0);
    }else if(POS_Y){
        return (int3)(0,GC*2-1-cell.y*2,0);
    }else if(NEG_Y){
        return (int3)(0,-cell.y-cell.y-1,0);
    }else if(POS_Z){
        return (int3)(0,0,GC*2-1-cell.z*2);
    }else if(NEG_Z){
        return (int3)(0,0,-cell.z-cell.z-1);
    }
#else
    return (int3)(0,0,0);
#endif
}

float  fetch(__global float* array, uint3 cell){
    return fetch_offset(array, cell,(int3)(0,0,0));
}
float2  fetch2(__global float** array, uint3 cell){
    return fetch2_offset(array, cell, (int3)(0,0,0));
}
float3  fetch3(__global float** array, uint3 cell){
    return fetch3_offset(array, cell, (int3)(0,0,0));
}
float4  fetch4(__global float** array, uint3 cell){
    return fetch4_offset(array, cell, (int3)(0,0,0));
}
float5  fetch5(__global float** array, uint3 cell){
    return fetch5_offset(array, cell, (int3)(0,0,0));
}
float6  fetch6(__global float** array, uint3 cell){
    return fetch6_offset(array, cell, (int3)(0,0,0));
}
float8  fetch8(__global float** array, uint3 cell){
    return fetch8_offset(array, cell, (int3)(0,0,0));
}

void   store(__global float* array, float value, uint3 cell){
    store_offset(array, value, cell,(int3)(0,0,0));
}
void   store2(__global float** array, float2 value, uint3 cell){
    store2_offset(array, value, cell,(int3)(0,0,0));
}
void   store3(__global float** array, float3 value, uint3 cell){
    store3_offset(array, value, cell,(int3)(0,0,0));
}
void   store4(__global float** array, float4 value, uint3 cell){
    store4_offset(array, value, cell,(int3)(0,0,0));
}
void   store5(__global float** array, float5 value, uint3 cell){
    store5_offset(array, value, cell,(int3)(0,0,0));
}
void   store6(__global float** array, float6 value, uint3 cell){
    store6_offset(array, value, cell,(int3)(0,0,0));
}
void   store8(__global float** array, float8 value, uint3 cell){
    store8_offset(array, value, cell,(int3)(0,0,0));
}

float  fetch_offset(__global float* array, uint3 cell, int3 offset){
    return array[get_index(cell,offset)];
}
float2  fetch2_offset(__global float** array, uint3 cell, int3 offset){
    size_t index = get_index(cell,offset);
    return (float2)(array[0][index],
                    array[1][index]);
}
float3  fetch3_offset(__global float** array, uint3 cell, int3 offset){
    size_t index = get_index(cell,offset);
    return (float3)(array[0][index],
                    array[1][index],
                    array[2][index]);
}
float4  fetch4_offset(__global float** array, uint3 cell, int3 offset){
    size_t index = get_index(cell,offset);
    return (float4)(array[0][index],
                    array[1][index],
                    array[2][index],
                    array[3][index]);
}
float5  fetch5_offset(__global float** array, uint3 cell, int3 offset){
    size_t index = get_index(cell,offset);
    return (float5)(array[0][index],
                    array[1][index],
                    array[2][index],
                    array[3][index],
                    array[4][index]);
}
float6  fetch6_offset(__global float** array, uint3 cell, int3 offset){
    size_t index = get_index(cell,offset);
    return (float6)(array[0][index],
                    array[1][index],
                    array[2][index],
                    array[3][index],
                    array[4][index],
		    array[5][index]);
}
float8  fetch8_offset(__global float** array, uint3 cell, int3 offset){
    size_t index = get_index(cell,offset);
    return (float8)(array[0][index],
                    array[1][index],
                    array[2][index],
                    array[3][index],
                    array[4][index],
                    array[5][index],
                    array[6][index],
                    array[7][index]);
}

void   store_offset(__global float* array, float value, uint3 cell, int3 offset){
    array[get_index(cell,offset)] = value;
}

void   store2_offset(__global float** array, float2 value, uint3 cell, int3 offset){
    size_t index = get_index(cell,offset);
    array[0][index] = value.s0;
    array[1][index] = value.s1;
}

void   store3_offset(__global float** array, float3 value, uint3 cell, int3 offset){
    size_t index = get_index(cell,offset);
    array[0][index] = value.s0;
    array[1][index] = value.s1;
    array[2][index] = value.s2;
}

void   store4_offset(__global float** array, float4 value, uint3 cell, int3 offset){
    size_t index = get_index(cell,offset);
    array[0][index] = value.s0;
    array[1][index] = value.s1;
    array[2][index] = value.s2;
    array[3][index] = value.s3;
}
void   store5_offset(__global float** array, float5 value, uint3 cell, int3 offset){
    size_t index = get_index(cell,offset);
    array[0][index] = value.s0;
    array[1][index] = value.s1;
    array[2][index] = value.s2;
    array[3][index] = value.s3;
    array[4][index] = value.s4;
}

void   store6_offset(__global float** array, float6 value, uint3 cell, int3 offset) {
    size_t index = get_index(cell,offset);
    array[0][index] = value.s0;
    array[1][index] = value.s1;
    array[2][index] = value.s2;
    array[3][index] = value.s3;
    array[4][index] = value.s4;
    array[5][index] = value.s5;
}

void   store8_offset(__global float** array, float8 value, uint3 cell, int3 offset){
    size_t index = get_index(cell,offset);
    array[0][index] = value.s0;
    array[1][index] = value.s1;
    array[2][index] = value.s2;
    array[3][index] = value.s3;
    array[4][index] = value.s4;
    array[5][index] = value.s5;
    array[6][index] = value.s6;
    array[7][index] = value.s7;
}

float fetch_mirror(__global float* array, uint3 cell){
    return fetch_offset(array, cell, get_mirror_cell_offset(cell));
}
float2 fetch2_mirror(__global float** array, uint3 cell){
    return fetch2_offset(array, cell, get_mirror_cell_offset(cell));
}
float3 fetch3_mirror(__global float** array, uint3 cell){
    return fetch3_offset(array, cell, get_mirror_cell_offset(cell));
}
float4 fetch4_mirror(__global float** array, uint3 cell){
    return fetch4_offset(array, cell, get_mirror_cell_offset(cell));
}
float5 fetch5_mirror(__global float** array, uint3 cell){
    return fetch5_offset(array, cell, get_mirror_cell_offset(cell));
}
float6 fetch6_mirror(__global float** array, uint3 cell){
    return fetch6_offset(array, cell, get_mirror_cell_offset(cell));
}

#endif

/**
 * Custom functions
 */
float6 __OVERLOADABLE__ sign(float6 data) {
  return (float6)(sign(data.s0),
		  sign(data.s1),
		  sign(data.s2),
		  sign(data.s3),
		  sign(data.s4),
		  sign(data.s5));
}

float6 __OVERLOADABLE__ fabs(float6 data) {
  return (float6)(fabs(data.s0),
		  fabs(data.s1),
		  fabs(data.s2),
		  fabs(data.s3),
		  fabs(data.s4),
		  fabs(data.s5));
}

float6 __OVERLOADABLE__ min(float6 v1, float6 v2) {
  // TODO: check if we should return the smallest vector, or the smallest elements in each vector.
  return (float6)(min(v1.s0, v2.s0),
		  min(v1.s1, v2.s1),
		  min(v1.s2, v2.s2),
		  min(v1.s3, v2.s3),
		  min(v1.s4, v2.s4),
		  min(v1.s5, v2.s5));
}
