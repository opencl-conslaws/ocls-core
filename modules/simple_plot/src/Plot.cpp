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



#include "Plot.h"
#include "SimplePlotModule.h"

size_t Plot::id_seed = 0;
bool Plot::syncing = false;
glm::mat4 Plot::transform = glm::mat4();

Plot::Plot(ocls::Module* module, ocls::RenderContext *context, std::string name, ocls::Data *data, ocls::Domain *domain) :
        m_context(context),
        m_data(data),
        m_domain(domain),
        m_module(module){

    m_id = id_seed++;
}

void Plot::startSync() {
    syncing = true;
}

void Plot::endSync() {
    syncing = false;
}


void Plot::requestRepaintWindow() {
    //m_window->notify();
    //m_context->requestRepaint(m_window);
    m_context->requestRepaintDirty(m_window);
}

size_t Plot::getID() {
    return m_id;
}
