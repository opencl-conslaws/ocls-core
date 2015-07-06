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



#include <math.h>
#include "c_structures.h"
#include "c_domain.h"

extern "C" {


int oclsSetDimensions(Domain *domain, const unsigned short dimensions_) {
    domain->dimensions = dimensions_;
    return ERROR_OK;
}

int oclsSetBoundsX(Domain *domain, const float left_, const float right_) {
    oclsSetBoundsXY(domain, left_, right_, 0.0f, 0.0f);
    return ERROR_OK;
}

int oclsSetBoundsXY(Domain *domain, const float left_, const float right_, const float bottom_, const float top_) {
    oclsSetBoundsXYZ(domain, left_, right_, top_, bottom_, 0.0f, 0.0f);
    return ERROR_OK;
}

int oclsSetBoundsXYZ(Domain *domain, const float left_, const float right_, const float bottom_, const float top_, const float front_, const float back_) {
    domain->bounds[0].min = left_;
    domain->bounds[0].max = right_;

    domain->bounds[1].min = bottom_;
    domain->bounds[1].max = top_;

    domain->bounds[2].min = front_;
    domain->bounds[2].max = back_;
    return ERROR_OK;
}

int oclsSetMeshX(Domain *domain, const size_t x_) {
    oclsSetMeshXY(domain, x_, 1);
    return ERROR_OK;
}

int oclsSetMeshXY(Domain *domain, const size_t x_, const size_t y_) {
    oclsSetMeshXYZ(domain, x_, y_, 1);
    return ERROR_OK;
}

int oclsSetMeshXYZ(Domain *domain, const size_t x_, const size_t y_, const size_t z_) {
    domain->mesh[0] = x_;
    domain->mesh[1] = y_;
    domain->mesh[2] = z_;
    return ERROR_OK;
}

int oclsSetGhostCells(Domain *domain, const unsigned short ghost_cells_) {
    domain->ghost_cells = ghost_cells_;
    return ERROR_OK;
}

float oclsGetDelta(Domain domain, const unsigned short dimension) {
    return ((float)fabs(domain.bounds[dimension-1].min)+
            (float)fabs(domain.bounds[dimension-1].max))/
            (float)domain.mesh[dimension-1];
}

}


