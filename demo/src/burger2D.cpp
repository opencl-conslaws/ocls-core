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
* \example burger2D.cpp
* Burgers equation 1D
* \f[ Q_t + QQ_x = 0 \f]
* C device code:
* \include burger2D.cls
* C++ program:
*/

#include "Framework.h"

//visualization module
#include "SimplePlotModule.h"

using namespace ocls;


int main(){
    Framework framework;

    Domain domain(2);
    domain.setBounds(-1.0f, 1.0f, -1.0f, 1.0f);
    domain.setMesh(200,200);
    domain.setGhostCells(1);

    CLSSource *sw_src = framework.getSourceLoader()->loadFromFile("burger2D.cls");

    Data* Q = framework.createData(domain, "q", sw_src->getFunction("gaussian"));

    // Boundary function
    auto boundary = framework.createBoundaryFunction(domain, sw_src->getFunction("on_bounds"));

    // Integrator
    auto lax_friedrichs = framework.createIntegratorFunction(domain, sw_src->getFunction("lax_friedrichs"));

    // Get plotter module
    SimplePlot* plotter = framework.getModule<SimplePlot>();

    // Create plots
    plotter->createPlot(Q, &domain, "q plot");

    float T = 0.0f;
    float dx = domain.getDelta(Domain::Dim::X);
    float dy = domain.getDelta(Domain::Dim::Y);
    float dt = 0.5f*dx/Q->max();

    while (T < 0.1f) {
        // Apply boundary condition
        boundary(Q);

        // Update plots
        plotter->updateAll();

        // Evolve equation
        Data* Q_1 = lax_friedrichs(Q, dt, dx, dy);

        // Prepare variables for next time step
        Q->copy(Q_1);

        T += dt;
    }

    return 0;
}
