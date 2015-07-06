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



#ifndef SIMPLE_PLOT_MODULE_H
#define SIMPLE_PLOT_MODULE_H

#include "Framework.h"
#include "GLUtils.hpp"
#include <vector>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>
//#include "VirtualTrackball.h"

#include "SurfacePlot.h"
#include "GraphPlot.h"
#include "VolumePlot.h"

class SimplePlot : public ocls::Module{
    
public:
    SimplePlot();

    virtual ~SimplePlot();
    
    /**
     * Called when the module is initialized by the framework
     */
    virtual void onInitialize(ocls::Framework* framework);
    /**
     * Called when the module is destroyed by the framework
     */
    virtual void onQuit(ocls::Framework* framework);
    
    /**
     * [Deprecated]
     * Called at the beginning of every simulation timestep
     */
    //virtual void onTimestepBegin(ocls::Framework* framework);
    
    /**
     * [Deprecated]
     * Called at the end of every simulation timestep
     */
    //virtual void onTimestepEnd(ocls::Framework* framework);

    /**
    * Create a new plot
    */
    Plot *createPlot(ocls::Data *data, ocls::Domain *domain, std::string name);

    /**
    * Update all plots
    */
    void updateAll();

    /**
    * Request repaint on all windows except the one with id
    * Note: Not thread safe!!
    */
    void requestRepaintAllExcept(size_t id);

private:
    ocls::RenderContext* m_render_context;
    std::vector<Plot*> m_plots;
};

#endif // SIMPLE_PLOT_MODULE_H
