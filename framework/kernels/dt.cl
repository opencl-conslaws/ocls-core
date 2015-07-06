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





__kernel computeDt(__global float* E_in, __global float* dt_out){
#if (_DIMENSIONS_ == 1)
    size_t x = get_local_id(0);
    
    __local float* temp[Ny];
    
    size_t i = x;
#elif (_DIMENSIONS_ == 2)
    size_t x = get_local_id(0);
    size_t y = get_local_id(1);
    
    __local float* temp[Ny*Ny];
    
    size_t i = y*Nx+x;
#elif (_DIMENSIONS_ == 3)
    size_t x = get_local_id(0);
    size_t y = get_local_id(1);
    size_t z = get_local_id(2);
    
    __local float* temp[Ny*Ny*Nz];
    
    size_t i = x*Ny*Nz+y*Nz+z;
#endif
    
    temp[i] = r;
    
    for (size_t s = get_local_size(0)/2; s>0; s>>=1) {
        if (i < s) {
            temp[i] = min(temp[i], temp[i + s]);
        }
        barrier(CLK_LOCAL_MEM_FENCE);
    }
    
    if (tid == 0) g_odata[blockIdx.x] = sdata[0];

}

