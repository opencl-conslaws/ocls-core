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


#ifndef FRAMEWORK_H
#define FRAMEWORK_H

#ifndef GLEW_MX
#define GLEW_MX
#endif

#include <map>
#include <cstddef>

#include "Utility.h"
#include "Module.h"
#ifdef OCLS_USE_VIS
#include "RenderContext.h"
#endif
#include "ComputeContext.h"
#include "DataManager.h"
#include "Domain.h"
#include "CLSSourceLoader.h"
#include "CLSSource.h"
#include "Logger.h"
#include "PerformanceAnalyzer.h"
#include "CallableFunction.h"
#include "BoundaryFunction.h"
#include "FluxFunction.h"
#include "IntegratorFunction.h"
#include "Collection.h"
#include "ProgramLauncher.h"
#include "ProgramBuilder.h"
#include "ProgramManager.h"

namespace ocls {

/** \defgroup main_api C++ API
*  @{
*/
class Framework {
public:
    /**
     * Initialize the framework
     */
    Framework(DeviceType type = __GPU__, Logger::Level log_level = Logger::DEBUG);
    
    /**
     * Tear down and clean up the framework
     */
    ~Framework();
    
    /**
     * get a module to the framework
     */
    template <typename T>
    T* getModule(){
        if(!std::is_base_of<Module, T>::value){
            logger->log(Logger::ERROR, "T is not a base class of Module");
        }

        auto it = m_modules.find(typeid(T).name());
        if(it != m_modules.end()){
            return (T*) it->second;
        }

        Module* module = (Module*)new T();
        module->onInitialize(this);
        m_modules[typeid(T).name()] = module;
        return (T*)module;
    }
    //Module* addModule(Module* module, const std::string& name_);
    
    /**
     * Get a registrated module
     */
    //Module* getModule(const std::string& name_);
    
    /**
     * Returns the OpenGL rendering context
     */
    RenderContext* getRenderContext();
    
    /**
     * Returns the OpenCL compute context
     */
    ComputeContext* getComputeContext();
    
    /**
     * Retruns the the source loader
     */
    CLSSourceLoader* getSourceLoader();

    /**
    * Data manager
    */
    DataManager* getDataManager();

    /**
    * Data manager
    */
    ProgramManager* getPrograManager();

    /**
    * Create dataset
    */
    Data* createData(const Domain& domain_, const std::string& reference_);
    
    /**
     * Create dataset
     */
    Data* createData(const Domain& domain_, const std::string& reference_, const CLSSource::Function* function_);

    /**
    * Create a boundary function
    */
    BoundaryFunction createBoundaryFunction(Domain& domain_, CLSSource::Function* function);

    /**
    * Create a flux function
    */
    FluxFunction createFluxFunction(Domain& domain_, CLSSource::Function* function);

    /**
    * Create a Integrator function
    */
    IntegratorFunction createIntegratorFunction(Domain& domain_, CLSSource::Function* function);

private:
    std::map<std::string,Module*> m_modules;
    
    RenderContext* m_render_context;
    ComputeContext* m_compute_context;
    
    DataManager* m_data_manager;
    ProgramManager* m_program_manager;
    CLSSourceLoader* m_source_loader;

    Logger* m_logger;
};

/** @}*/

} //cls


#endif //FRAMEWORK_H
