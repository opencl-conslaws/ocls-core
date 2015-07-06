#include "BoundaryFunction.h"
#include <Framework.h>

namespace ocls{

BoundaryFunction::BoundaryFunction(Domain* domain, Framework *framework, CLSSource::Function* function)
                : CallableFunction(domain, framework, function->parameters){

    ProgramManager* man = framework->getPrograManager();

    ProgramBuilder builder(m_framework->getComputeContext());

    m_program[POS_X] = man->manage(builder.createBoundaryProgram(&m_domain, function, POS_X));
    m_program[NEG_X] = man->manage(builder.createBoundaryProgram(&m_domain, function, NEG_X));
    if(m_domain.getDimensions() >= 2){
            m_program[POS_Y] = man->manage(builder.createBoundaryProgram(&m_domain, function, POS_Y));
            m_program[NEG_Y] = man->manage(builder.createBoundaryProgram(&m_domain, function, NEG_Y));
    }
    if(m_domain.getDimensions() == 3) {
            m_program[POS_Z] = man->manage(builder.createBoundaryProgram(&m_domain, function, POS_Z));
            m_program[NEG_Z] = man->manage(builder.createBoundaryProgram(&m_domain, function, NEG_Z));
    }
}

BoundaryFunction::~BoundaryFunction() {
}

ReturnType BoundaryFunction::call(std::vector<Program::Parameter> &params) {
	Domain domain = m_program[POS_X]->getExecutionDomain();
	size_t s[3];
	for (size_t i = 1; i <= 3; i++) {
		s[i-1] = domain.getMesh((Domain::Dim)i);
	}
	ProgramLauncher::launch(m_program[POS_X], params, domain.getDimensions(), &s[0], false);
    ProgramLauncher::launch(m_program[NEG_X], params, domain.getDimensions(), &s[0], false);

    if(m_domain.getDimensions() >= 2){
        domain = m_program[POS_Y]->getExecutionDomain();
        for (size_t i = 1; i <= 3; i++) {
            s[i-1] = domain.getMesh((Domain::Dim)i);
        }
        ProgramLauncher::launch(m_program[POS_Y], params, domain.getDimensions(), &s[0], false);
        ProgramLauncher::launch(m_program[NEG_Y], params, domain.getDimensions(), &s[0], false);
    }
    if(m_domain.getDimensions() == 3) {
        domain = m_program[POS_Z]->getExecutionDomain();
        for (size_t i = 1; i <= 3; i++) {
            s[i-1] = domain.getMesh((Domain::Dim)i);
        }
        ProgramLauncher::launch(m_program[POS_Z], params, domain.getDimensions(), &s[0], false);
        ProgramLauncher::launch(m_program[NEG_Z], params, domain.getDimensions(), &s[0], false);
    }

    // Boundary functions has no return type
    return NULL;
}
}
