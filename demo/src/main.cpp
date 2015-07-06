#include "Framework.h"
//module
#include "SimplePlotModule.h"

using namespace ocls;

void printData(Data* data, Domain domain, const char* name = NULL){
    //return;
    std::cout << std::endl;
    if(name == NULL)
        std::cout << "Dump of: " << data->getName() << std::endl;
    else
        std::cout << "Dump of: " << name << std::endl;
    for (int y = 0; y < domain.getMeshWithGC(Domain::Y); ++y) {
        for (int x = 0; x < domain.getMeshWithGC(Domain::X); ++x) {
            std::cout << std::fixed << std::setprecision(5) << data->at(x,y) <<"\t\t ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

int main(){
    Framework framework;

    Domain domain3(3);
    domain3.setMesh(120,120,120);
    domain3.setBounds(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
    domain3.setGhostCells(1);

    Domain domain2(2);
    domain2.setMesh(120,120);
    domain2.setBounds(-1.0f, 1.0f, -1.0f, 1.0f);
    domain2.setGhostCells(1);

    Domain domain1(1);
    domain1.setMesh(120);
    domain1.setBounds(-1.0f, 1.0f);
    domain1.setGhostCells(1);

    CLSSource *sw_src = framework.getSourceLoader()->loadFromFile("sw.cls");

    Data* q3 = framework.createData(domain3, "h", sw_src->getFunction("gaussian3d"));
    Data* q2 = framework.createData(domain2, "h", sw_src->getFunction("gaussian2d"));
    Data* q1 = framework.createData(domain1, "h", sw_src->getFunction("gaussian1d"));

    // Get plotter module
    SimplePlot* plotter = framework.getModule<SimplePlot>();

    // Create plots
    plotter->createPlot(q3, &domain3, "");
    plotter->createPlot(q2, &domain2, "");
    plotter->createPlot(q1, &domain1, "");

    float t = 0.0f;
    while(t < 15.0f){
        plotter->updateAll();
        t+=0.5f;
    }

    return 0;

#if defined(__GNUC__)
    std::cout << "Compiled with GCC C/C++ " << __GNUC__ << "." << __GNUC_MINOR__ << std::endl;
#elif defined (__clang__)
    std::cout << "Compiled with Clang C++ " << __clang_major__ << "." << __clang_minor__ << std::endl;
#endif

    exit(0);
    try {
        Framework framework;

        Domain domain(2);
        domain.setBounds(-1.0f, 1.0f, -1.0f, 1.0f);
        //domain.setMesh(1000,1000);
        //domain.setMesh(500, 500);
        domain.setMesh(5,5);
        domain.setGhostCells(2);

        CLSSource *sw_src = framework.getSourceLoader()->loadFromFile("sw_clean.cls");
        //PerformanceAnalyser::start();

        //Data* h = framework.createData(domain, "h", sw_src->getFunction("gaussian2d"));
        Data* h = framework.createData(domain, "h", sw_src->getFunction("dam_break"));
        Data* hu = framework.createData(domain, "hu"); // zero initialize
        Data* hv = framework.createData(domain, "hv"); // zero initialize

        Collection Q = Collection::glob(h,hu,hv);

        // Boundary function
        auto boundary = framework.createBoundaryFunction(domain, sw_src->getFunction("on_bounds"));

        // Flux function
        auto flux_func = framework.createFluxFunction(domain, sw_src->getFunction("central_upwind_flux"));
        //auto flux_func = framework.createFluxFunction(domain, sw_src->getFunction("lax_friedrichs"));

        auto stencil_x = framework.createFluxFunction(domain, sw_src->getFunction("dump_stencil_x"));
        auto stencil_y = framework.createFluxFunction(domain, sw_src->getFunction("dump_stencil_y"));

        // Integrator
        auto forward_euler = framework.createIntegratorFunction(domain, sw_src->getFunction("forward_euler"));

        // Get plotter module
        SimplePlot* plotter = framework.getModule<SimplePlot>();

        // Create plots
        plotter->createPlot(Q[0], &domain, "h plot");
        //plotter->createPlot(Q[1], &domain, "hu plot");
        //plotter->createPlot(Q[2], &domain, "hv plot");

        float T = 0.0f;
        float dt = 0.00001f;
        float dx = domain.getDelta(Domain::Dim::X);
        float dy = domain.getDelta(Domain::Dim::Y);
        float CFL = 0.05f;
        uint step = 0;

        Collection Q_1 = Collection::glob(h,hu,hv);


        //while (T < 0.065f) {
        //while (T < 0.005f) {
        //while (T < 0.0001f) {
        while (T < 1.0f) {

            std::cout << "step: " << step++ << std::endl;
            // Collections are expanded into multiple parameters
            // Q becomes 3 parameters inside the script functions.
            // Q -> float* h, float* hu, float* hv
            // or an array
            // Q -> float* Q[3]

            // Apply boundary condition
            boundary(Q);
            //printData(Q[0], domain);
            //printData(Q[1], domain);
            //printData(Q[2], domain);

            // Compute flux functions
            Collection fluxAndE = flux_func(Q);

            printData(fluxAndE[0], domain, "F_h");
            printData(fluxAndE[3], domain, "G_h");
            printData(fluxAndE[1], domain, "F_hu");
            printData(fluxAndE[4], domain, "G_hu");
            printData(fluxAndE[2], domain, "F_hv");
            printData(fluxAndE[5], domain, "G_hv");
            //printData(fluxAndE[6], domain, "E");

            Collection Rx = stencil_x(Q);
            Collection Ry = stencil_y(Q);
/*
            printData(Rx[0], domain, "west");
            printData(Rx[1], domain, "west_plus");
            printData(Rx[2], domain, "west_minus");

            printData(Rx[3], domain, "east");
            printData(Rx[4], domain, "east_plus");
            printData(Rx[5], domain, "east_minus");

            printData(Ry[0], domain, "north");
            printData(Ry[1], domain, "north_plus");
            printData(Ry[2], domain, "north_minus");

            printData(Ry[3], domain, "south");
            printData(Ry[4], domain, "south_plus");
            printData(Ry[5], domain, "south_minus");
*/
            // Compute timestep
            //float max = fluxAndE[6]->max();
            //dt = CFL*glm::min(dx/max,dy/max);

            // Evolve equation
            Q_1 = forward_euler(Q, fluxAndE, dt, dx, dy);

            // Prepare variables for next time step
            Q.copy(Q_1);

            //printData(Q[0], domain);
            //printData(Q[1], domain);
            //printData(Q[2], domain);

            T += dt;

            if(std::isnan(Q[0]->at(2,2))){
                exit(0);
            }

            // Update plots
            plotter->updateAll();

        }

        //PerformanceAnalyser::end();

        //std::ofstream f;
        //f.open("perf", std::ios_base::out);
        //PerformanceAnalyser::writeAnalyticsTimeline(f);
        //f.close();

    }catch (std::exception& e){
        std::cout << "Exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
