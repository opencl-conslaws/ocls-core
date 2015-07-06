#include <Framework.h>
#include "FluxFunction.h"
#include "ProgramBuilder.h"

namespace ocls {
FluxFunction::FluxFunction(Domain* domain, Framework *framework, CLSSource::Function* function) :
        CallableFunction(domain, framework, function->parameters), m_program(NULL)
{
    ProgramManager* man = framework->getPrograManager();

    ProgramBuilder builder(m_framework->getComputeContext());
    size_t return_values = 0;
    m_program = man->manage(builder.createFluxProgram(&m_domain, function, &return_values));

    for (int i = 0; i < return_values; ++i) {
        m_returnValues.push_back(framework->createData(*domain, ""));
    }
}

FluxFunction::~FluxFunction() {
}

ReturnType FluxFunction::call(std::vector<ocls::Program::Parameter> &params) {

    for (int i = 0; i < m_returnValues.size(); ++i) {
        params.push_back(m_returnValues[i]->getParameter());
    }
    ProgramLauncher::launch(m_program, params, false);

    if(m_returnValues.size() == 1)
        return ReturnType(m_returnValues[0]);
    else
        return Collection::glob(m_returnValues);
}
}