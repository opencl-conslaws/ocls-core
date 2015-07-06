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
