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




#include "c_visualizer.h"
#include "Framework.h"
#include "SimplePlotModule.h"
#include "Framework.h"

extern "C" {

int oclsVizInit(Framework framework, Visualizer* visualizer){
    try {
        *visualizer = reinterpret_cast<PointerHandle> (
                reinterpret_cast<ocls::Framework*>(framework)->getModule<SimplePlot>()
        );
    }catch(std::exception& e){
        *visualizer = NULL;
        strcpy(error_buffer, e.what());
        return ERROR_FATAL;
    }
    return ERROR_OK;
}

int oclsVisPlot(Visualizer visualizer, Data data, Domain domain, Figure* figure){
    try{
        *figure = reinterpret_cast<PointerHandle> (reinterpret_cast<SimplePlot*>(visualizer)->createPlot(
                reinterpret_cast<ocls::Data*>(data),
                reinterpret_cast<ocls::Domain*>(&domain),
                ""));
    }catch(std::exception& e){
        figure = NULL;
        strcpy(error_buffer, e.what());
        return ERROR_FATAL;
    }
    return ERROR_OK;
}


int oclsVisUpdate(Figure figure, Data data){
    try{
        reinterpret_cast<Plot*>(figure)->update(reinterpret_cast<ocls::Data*>(data));
    }catch(std::exception& e){
        strcpy(error_buffer, e.what());
        return ERROR_FATAL;
    }
    return ERROR_OK;
}

}
