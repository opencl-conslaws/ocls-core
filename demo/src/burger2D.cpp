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