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
 * Default Pipeline functions
 */

#define __GPU__ 0
#define __CPU__ 1
#define __ACCELERATOR__ 2

#define DEBUG

/**
* http://developer.amd.com/resources/documentation-articles/articles-whitepapers/opencl-optimization-case-study-simple-reductions/
*/

#if (__DEVICE_TYPE__ == __GPU__)
// AMD GPU Optimized Two-Stage reduction
__kernel
void reduce_kernel(
        __global float* buffer,
        __local float* scratch,
        __const int length,
        __global float* result) {

    int global_index = get_global_id(0);

#if FIND_MAX == 1
    float accumulator = -INFINITY;
#elif FIND_MIN == 1
    float accumulator = INFINITY;
#else
    float accumulator = 0.0f;
#endif

    // Loop sequentially over chunks of input vector
    while (global_index < length) {
        float element = buffer[global_index];
#if FIND_MAX == 1
        accumulator = (accumulator > element) ? accumulator : element;
#elif FIND_MIN == 1
        accumulator = (accumulator < element) ? accumulator : element;
#else
        accumulator = (accumulator < element) ? accumulator : element;
#endif
        global_index += get_global_size(0);
    }

    // Perform parallel reduction
    int local_index = get_local_id(0);
    scratch[local_index] = accumulator;
    barrier(CLK_LOCAL_MEM_FENCE);

    for(int offset = get_local_size(0) / 2; offset > 0; offset = offset / 2) {
        if (local_index < offset) {
            float other = scratch[local_index + offset];
            float mine = scratch[local_index];
#if FIND_MAX == 1
            scratch[local_index] = (mine > other) ? mine : other;
#elif FIND_MIN == 1
            scratch[local_index] = (mine < other) ? mine : other;
#else
            scratch[local_index] = (mine < other) ? mine : other;
#endif
        }
        barrier(CLK_LOCAL_MEM_FENCE);
    }

    if (local_index == 0) {
        result[get_group_id(0)] = scratch[0];
    }
}

#elif (__DEVICE_TYPE__ == __CPU__)

// AMD CPU Optimized
__kernel
void reduce_kernel(__global float* buffer,
        __const int block,
        __const int length,
        __global float* result) {

    int global_index = get_global_id(0) * block;

#if FIND_MAX == 1
    float accumulator = -INFINITY;
#elif FIND_MIN == 1
    float accumulator = INFINITY;
#else
    float accumulator = 0.0f;
#endif

    int upper_bound = (get_global_id(0) + 1) * block;
    if (upper_bound > length) upper_bound = length;

    while (global_index < upper_bound) {
        float element = buffer[global_index];
#if FIND_MAX == 1
        accumulator = (accumulator > element) ? accumulator : element;
#elif FIND_MIN == 1
        accumulator = (accumulator < element) ? accumulator : element;
#else
        accumulator = (accumulator < element) ? accumulator : element;
#endif
        global_index++;
    }
    result[get_group_id(0)] = accumulator;
}

#endif

__kernel
void unpadded_to_padded_1D_kernel(__global float* buffer,
        __const int length,
        __global float* result) {

    int x = get_global_id(0);
    if(x >= length){
        result[x] = 0.0f;
    }else{
        result[x] = buffer[x];
    }
}


__kernel
void unpadded_to_padded_2D_kernel(__global float* buffer,
        __const int2 length,
        __const int2 padded_length,
        __global float* result) {

    int x = get_global_id(0);
    int y = get_global_id(1);
    int padded_index    = (padded_length.x * y + x);
    int index           = (length.x * y + x);

    if(x >= length.x || y >= length.y){
        result[padded_index] = 0.0f;
    }else{
        result[padded_index] = buffer[index];
    }

}

__kernel
void unpadded_to_padded_3D_kernel(__global float* buffer,
        __const int3 length,
        __const int3 padded_length,
        __global float* result) {

    int x = get_global_id(0);
    int y = get_global_id(1);
    int z = get_global_id(2);

    int padded_index    = (z * padded_length.y * padded_length.x) + (y * padded_length.x) + x;
    int index           = (z * length.y * length.x) + (y * length.x) + x;

    if(x >= length.x || y >= length.y || z >= length.z){
        result[padded_index] = 0.0f;
    }else{
        result[padded_index] = buffer[index];
    }
}

__kernel
void padded_to_unpadded_1D_kernel(__global float* buffer,
        __const int length,
        __global float* result) {

    int x = get_global_id(0);
    if(x >= length){
        return;
    }else{
        result[x] = buffer[x];
    }
}


__kernel
void padded_to_unpadded_2D_kernel(__global float* buffer,
        __const int2 length,
        __const int2 padded_length,
        __global float* result) {

    int x = get_global_id(0);
    int y = get_global_id(1);

    int padded_index    = (padded_length.x * y + x);
    int index           = (length.x * y + x);

    if(x >= length.x || y >= length.y){
        return;
    }else{
        result[index] = buffer[padded_index];
    }

}

__kernel
void padded_to_unpadded_3D_kernel(__global float* buffer,
        __const int3 length,
        __const int3 padded_length,
        __global float* result) {

    int x = get_global_id(0);
    int y = get_global_id(1);
    int z = get_global_id(2);

    int padded_index    = (z * padded_length.y * padded_length.x) + (y * padded_length.x) + x;
    int index           = (z * length.y * length.x) + (y * length.x) + x;

    if(x >= length.x || y >= length.y || z >= length.z){
        return;
    }else{
        result[index] = buffer[padded_index];
    }

}
