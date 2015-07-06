#ifndef FLUX_FUNCTION_H
#define FLUX_FUNCTION_H


#include "CLSSource.h"
#include "ProgramLauncher.h"
#include "CallableFunction.h"

namespace ocls {

/** \ingroup main_api
*  @{
*/
class FluxFunction :
        public CallableFunction{
public:
    FluxFunction(Domain* domain, Framework* framework, CLSSource::Function* function);
    virtual ~FluxFunction();

protected:
    /**
    * Launch boundary program
    */
    virtual ReturnType call(std::vector<Program::Parameter>& params);

private:
    Program* m_program;

    std::vector<Data*> m_returnValues;
};
/** @} */

}

#endif