#include "ProgramManager.h"
#include "Framework.h"
#include <iostream>

namespace ocls {

ProgramManager::ProgramManager(Framework* framework){
    m_framework = framework;
}

ProgramManager::~ProgramManager(){
    logger->log(Logger::DEBUG, "Deleting managed Programs");

    std::vector<Program*>::iterator it;

    for (it=m_programs.begin(); it!=m_programs.end(); ++it){
        delete *it;
    }
}

}