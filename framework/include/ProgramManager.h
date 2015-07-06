#ifndef PROGRAM_MANAGER_H
#define PROGRAM_MANAGER_H

#include "Program.h"
#include <map>

namespace ocls {

class Framework;

/** \ingroup main_api
*  @{
*/
class ProgramManager{
public:
    /**
    *
    */
    ProgramManager(Framework* framework);
    /**
    *
    */
    ~ProgramManager();

    /**
    * Manage a program object
    */
    Program* manage(Program* program){
        m_programs.push_back(program);
        return program;
    }

private:
    Framework* m_framework;
    std::vector<Program*> m_programs;

};
/** @} */

} //cls

#endif //