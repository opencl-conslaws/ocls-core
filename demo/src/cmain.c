#include <c_framework.h>
#include <c_visualizer.h>

int main(){
    // Define variables
    Framework ctx;
    Domain d;
    CLSSource src;
    Data q;

    // Define functions
    BoundaryFunction on_bounds;
    IntegratorFunction lax_friedrichs;

    // Init API
    oclsInit(&ctx);

    Visualizer viz;
    oclsVizInit(ctx, &viz);

    // Set domain variables
    oclsSetDimensions(&d, 1);
    oclsSetBoundsX(&d, -1.0f, 1.0f);
    oclsSetMeshX(&d, 250);
    oclsSetGhostCells(&d, 1);

    // Load source file
    oclsLoadFromFile(ctx, "burger1D.cls", &src);

    // Create data
    oclsCreateDataFromFunction(ctx, d, "q", src, "gaussian", &q);

    // Create callable functions
    oclsCreateBoundaryFunction(ctx, d, src, "on_bounds", &on_bounds);
    oclsCreateIntegratorFunction(ctx, d, src, "lax_friedrichs", &lax_friedrichs);

    float T = 0.0f;
    float dx = oclsGetDelta(d, X);

    float max;
    oclsFindMax(q, &max);
    float dt = 0.5f*dx/max;

    Parameter on_bounds_params[] = {
            oclsGetParameter(q)
    };

    Parameter lax_friedrichs_params[] = {
            oclsGetParameter(q),
            TO_PARAMETER(dt),
            TO_PARAMETER(dx)
    };

    Figure fig;
    oclsVisPlot(viz, q, d, &fig);

    while (T < 0.5f) {
        // Apply boundary condition
        oclsCall(on_bounds, 1, on_bounds_params, NULL, NULL);

        // Evolve equation
        Data q_1;
        oclsCall(lax_friedrichs, 3, lax_friedrichs_params, &q_1, NULL);

        // Prepare variables for next time step
        oclsCopy(q_1, q);

        oclsVisUpdate(fig, q);

/*
        size_t i;
        printf("\n");
        for (i = 0; i < 25+1; ++i) {
            printf("%f, ", oclsAtX(q, i));
        }
        printf("\n");
*/
        T += dt;

        printf("T: %f\n", T);
    }

    oclsClean(ctx);

    return 0;
}