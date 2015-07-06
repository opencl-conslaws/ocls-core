#include "Framework.h"
#include "KernelResources.h"

//#define OCLS_USE_VIS

namespace ocls {

Logger* logger;

Framework::Framework(DeviceType type, Logger::Level log_level){
    m_logger = new ConsoleLogger(log_level);
    m_logger->chainLogger(
            new FileLogger((Logger::Level)((int)log_level-1)))->chainLogger(
            new CrashLogger((Logger::Level)((int)log_level-2)));
    logger = m_logger;
    logger->log(Logger::DEBUG, "Initializing Framework");

#ifdef OCLS_USE_VIS
	m_render_context = new RenderContext();
#else
    m_render_context = NULL;
#endif
    m_compute_context = new ComputeContext(m_render_context, type);
    m_source_loader = new CLSSourceLoader(this);
    m_data_manager = new DataManager(this);
    m_program_manager = new ProgramManager(this);
    ProgramLauncher::init(m_compute_context);


/*
    std::cout << "Platform Info:" << std::endl
            <<  "-------------- " << std::endl;
    m_compute_context->writePlatformInfo(std::cout);
    std::cout << std::endl;
    
    std::cout << "Device Info:" << std::endl
            <<  "-------------- " << std::endl;
    m_compute_context->writeDeviceInfo(std::cout);
    std::cout << std::endl;
*/
}

Framework::~Framework(){
    logger->log(Logger::DEBUG, "Destroying Framework");
#ifdef OCLS_USE_VIS
    delete m_render_context; // < Application will wait until all windows are closed
#endif
    std::map<std::string, Module*>::iterator it;
    for (it=m_modules.begin(); it!=m_modules.end(); ++it){
        it->second->onQuit(this);
        delete it->second;
    }

    ProgramLauncher::destory();
    delete m_source_loader;
    delete m_data_manager;
    delete m_program_manager;
    delete m_compute_context;
    delete m_logger;
}

/*Module* Framework::addModule(Module* module, const std::string& name_){
    module->onInitialize(this);
    // TODO: Check if module by that name is already registrated
    m_modules[name_] = module;
    return module;
}*/

    /*
Module* Framework::getModule(const std::string& name_){
    //TODO: Check if @name_ is a registrated module name
    return m_modules[name_];
}*/

    
ComputeContext* Framework::getComputeContext(){
    return m_compute_context;
}
    
RenderContext* Framework::getRenderContext(){
    return m_render_context;
}

CLSSourceLoader* Framework::getSourceLoader(){
    return m_source_loader;
}

Data* Framework::createData(const Domain& domain_, const std::string& reference_){
    size_t s[3];
    for (size_t i = 1; i <= 3; i++) {
        s[i-1] = domain_.getMeshWithGC((Domain::Dim)i);
    }
    Data* data = m_data_manager->create(&s[0], reference_);

    return data;
}

Data* Framework::createData(const Domain& domain_, const std::string& reference_, const CLSSource::Function* function_){
    size_t s[3];
    for (size_t i = 1; i <= 3; i++) {
        s[i-1] = domain_.getMeshWithGC((Domain::Dim)i);
    }
    Data* data = m_data_manager->create(&s[0], reference_);

    ProgramBuilder builder(m_compute_context);
    Program* program = builder.createInitialCondProgram(&domain_, function_);

    std::vector<Program::Parameter> parameters;
    parameters.push_back(data->getParameter());

    ProgramLauncher::launch(program, parameters, true);

    delete program;

    return data;
}

/*Pipeline* Framework::createPipeline(const Domain& domain_, const Pipeline::Properties& properties_) {

    Pipeline* pipeline = new Pipeline(this, domain_, properties_);
    m_pipelines.push_back(pipeline);
    return pipeline;
}
*/

DataManager *Framework::getDataManager() {
    return m_data_manager;
}

ProgramManager* Framework::getPrograManager(){
    return m_program_manager;
}

BoundaryFunction Framework::createBoundaryFunction(Domain& domain_, CLSSource::Function *function) {
    BoundaryFunction func(&domain_, this, function);
    return func;
}

FluxFunction Framework::createFluxFunction(Domain& domain_, CLSSource::Function *function) {
    FluxFunction func(&domain_, this, function);
    return func;
}

IntegratorFunction Framework::createIntegratorFunction(Domain& domain_, CLSSource::Function *function) {
    IntegratorFunction func(&domain_, this, function);
    return func;
}
}
