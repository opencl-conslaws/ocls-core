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




%module ocls_visualizer_swig
%include cpointer.i
%include typemaps.i
%include cdata.i
%include exception.i
%include carrays.i
%include cstring.i

%{
#define SWIG_FILE_WITH_INIT
#include "c_visualizer.h"
#include "c_framework.h"
#include "c_domain.h"
#include "c_structures.h"
%}

%include numpy.i
%init %{
import_array();
%}

typedef struct {
    float min;
    float max;
}oclsBoundary;
typedef oclsBoundary Boundary;

typedef struct {
    unsigned short dimensions;
    Boundary bounds[3];
    size_t mesh[3];
    unsigned short ghost_cells;
}oclsDomain;
typedef oclsDomain Domain;

struct oclsPtrHandle{};
typedef struct oclsPtrHandle* PointerHandle;

typedef PointerHandle Data;
typedef PointerHandle Framework;
typedef PointerHandle Visualizer;
typedef PointerHandle Figure;

%rename(oclsVizInit) VizInit;
%inline %{
Visualizer VizInit(Framework framework) {
    Visualizer temp;
    int err = oclsVizInit(framework, &temp);
    if(err != ERROR_OK){
        PyErr_SetString(PyExc_RuntimeError,oclsGetError());
    }

    return temp;
}
%}

%rename(oclsVisPlot) VisPlot;
%inline %{
Figure VisPlot(Visualizer visualizer, Data data, Domain domain) {
    Figure temp;
    int err = oclsVisPlot(visualizer, data, domain, &temp);
    if(err != ERROR_OK){
        PyErr_SetString(PyExc_RuntimeError,oclsGetError());
    }

    return temp;
}
%}



%rename(oclsVisUpdate) VisUpdate;
%inline %{
void VisUpdate(Figure figure, Data data) {
    int err = oclsVisUpdate(figure, data);
    if(err != ERROR_OK){
        PyErr_SetString(PyExc_RuntimeError,oclsGetError());
    }
}
%}