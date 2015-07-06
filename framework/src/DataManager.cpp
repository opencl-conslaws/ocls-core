#include "DataManager.h"
#include "Framework.h"
#include <iostream>

namespace ocls {
DataManager::DataManager(Framework* framework){
    m_compute_context = framework->getComputeContext();
    m_render_context = framework->getRenderContext();
}

DataManager::~DataManager(){
    logger->log(Logger::DEBUG, "Deleting allocated data on device and host");

    std::vector<Data*>::iterator it;
    
    for (it=m_data.begin(); it!=m_data.end(); ++it){
        delete *it;
    }
}

}