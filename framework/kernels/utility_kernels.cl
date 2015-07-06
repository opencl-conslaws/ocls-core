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
 Utility
*/
#if (__IMAGE_SUPPORT__ == 1)

#if (_DIMENSIONS_ == 1)
#if(0)//#if (__OPENCL_VERSION__ >= 120) // OpenCL 1.2
__kernel void memcpy_buf_tex_kernel(__global float* in, __write_only image1d_t tex,
          uint3 data_size){
    unsigned int x = get_global_id(0);

    float val = in[x];
    write_imagef(tex, x, (float4)(val,val,val,val));
}
#else // Use 2D image if 1D texture are not supported
__kernel void memcpy_buf_tex_kernel(__global float* in, __write_only image2d_t tex,
          uint3 data_size){
    unsigned int x = get_global_id(0);

    float val = in[x];
    write_imagef(tex, (int2)(x,0), (float4)(val,val,val,val));
}
#endif
#elif (_DIMENSIONS_ == 2)
__kernel void memcpy_buf_tex_kernel(__global float* in, __write_only image2d_t tex,
          uint3 data_size){
    unsigned int x = get_global_id(0);
    unsigned int y = get_global_id(1);

    float val = in[data_size.x * y + x];
    write_imagef(tex, (int2)(x,y), (float4)(val,val,val,val));
}
#elif (_DIMENSIONS_ == 3)
#if cl_khr_3d_image_writes
// Needed to write to 3D textures
#pragma OPENCL EXTENSION cl_khr_3d_image_writes : enable
__kernel void memcpy_buf_tex_kernel(__global float* in, __write_only image3d_t tex,
          uint3 data_size){
    unsigned int x = get_global_id(0);
    unsigned int y = get_global_id(1);
    unsigned int z = get_global_id(2);

    float val = in[(z * data_size.y * data_size.x) + (y * data_size.x) + x];
    write_imagef(tex, (int4)(x,y,z,1), (float4)(val,val,val,val));
}
#endif
#endif
#endif