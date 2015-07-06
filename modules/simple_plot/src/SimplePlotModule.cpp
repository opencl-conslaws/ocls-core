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





#include "SimplePlotModule.h"
#include "Framework.h"

SimplePlot::SimplePlot(){
   m_render_context = NULL;
}

SimplePlot::~SimplePlot(){

}


void SimplePlot::onInitialize(ocls::Framework* framework){
    m_render_context = framework->getRenderContext();
}

void SimplePlot::onQuit(ocls::Framework* framework){
    for (int i = 0; i < m_plots.size(); ++i) {
        delete m_plots[i];
    }
    m_plots.clear();
}

/*void SimplePlot::onTimestepBegin(ocls::Framework* framework){
    
}
void SimplePlot::onTimestepEnd(ocls::Framework* framework){
    
}*/

Plot *SimplePlot::createPlot(ocls::Data *data, ocls::Domain *domain, std::string name) {
    Plot* plot = NULL;

    if(domain->getDimensions() == 1) {
        plot = new GraphPlot(this, m_render_context, name, data, domain);
    }
    else if(domain->getDimensions() == 2){
        plot = new SurfacePlot(this, m_render_context, name, data, domain);
    }
    else if(domain->getDimensions() == 3){
        plot = new VolumePlot(this, m_render_context, name, data, domain);
    }
    m_plots.push_back(plot);

    return plot;
}

void SimplePlot::updateAll() {
    for (int i = 0; i < m_plots.size(); ++i) {
        m_plots[i]->update();
    }
}


void SimplePlot::requestRepaintAllExcept(size_t id) {

    for (int i = 0; i < m_plots.size(); ++i) {
        if (m_plots[i]->getID() != id) {
            m_plots[i]->requestRepaintWindow();
        }
    }

}
