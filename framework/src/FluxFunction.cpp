/**
  *  This file is part of the OpenCL-ConsLaws framework
  *  Copyright (C) 2014, 2015 Jens Kristoffer Reitan Markussen
  *
  * This program is free software: you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation, either version 3 of the License, or
  * (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with this program.  If not, see <http://www.gnu.org/licenses/>.
  *
  */


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
