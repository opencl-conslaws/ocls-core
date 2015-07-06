#ifndef INTEGRATOR_FUNCTION_H
#define INTEGRATOR_FUNCTION_H


#include "CLSSource.h"
#include "ProgramLauncher.h"
#include "CallableFunction.h"

namespace ocls {

/** \ingroup main_api
*  @{
*/
class IntegratorFunction :
        public CallableFunction{
public:
    IntegratorFunction(Domain* domain, Framework* framework, CLSSource::Function* function);
    virtual ~IntegratorFunction();

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