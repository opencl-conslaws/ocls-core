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




#ifndef C_DOMAIN_H
#define C_DOMAIN_H

#ifdef __cplusplus
extern "C" {
#endif
/**
*  \defgroup c_wrap_domain Domain
*  \ingroup c_wrap
*  @{
*/

/**
* Set the number of dimensions
*/
int oclsSetDimensions(Domain* domain, const unsigned short dimensions_);

/**
* @{
* Set the physical domain bounds
*/
int oclsSetBoundsX(Domain* domain, const float left_, const float right_);
int oclsSetBoundsXY(Domain* domain, const float left_, const float right_, const float bottom_, const float top_);
int oclsSetBoundsXYZ(Domain* domain, const float left_, const float right_, const float bottom_, const float top_, const float front_, const float back_);
/** @}*/

/**
* @{
* Set the mesh refinement in cell count
*/
int oclsSetMeshX(Domain* domain, const size_t x_);
int oclsSetMeshXY(Domain* domain, const size_t x_, const size_t y_);
int oclsSetMeshXYZ(Domain* domain, const size_t x_, const size_t y_, const size_t z_);
/** @}*/

/**
* Set the number of ghost cells
*/
int oclsSetGhostCells(Domain* domain, const unsigned short ghost_cells_);

/**
* Find Delta
*/
float oclsGetDelta(Domain domain, const unsigned short dimension);


/** @}*/

#ifdef __cplusplus
}
#endif

#endif
