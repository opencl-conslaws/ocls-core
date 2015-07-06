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
