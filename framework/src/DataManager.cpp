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


#include "DataManager.h"
#include "Framework.h"
#include <iostream>

namespace ocls {
DataManager::DataManager(Framework* framework){
    m_compute_context = framework->getComputeContext();
    m_render_context = framework->getRenderContext();
}

DataManager::~DataManager(){
    logger->log(Logger::DEBUG, "Deleting allocated data on device and host");

    std::vector<Data*>::iterator it;
    
    for (it=m_data.begin(); it!=m_data.end(); ++it){
        delete *it;
    }
}

}
