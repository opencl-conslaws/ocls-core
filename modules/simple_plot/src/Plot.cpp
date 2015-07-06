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
