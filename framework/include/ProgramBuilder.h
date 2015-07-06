#ifndef PROGRAM_BUILDER_H
#define PROGRAM_BUILDER_H

#include <functional>
#include <sys/wait.h>

#include "BoundaryFunction.h"
#include "Program.h"
#include "CLSSource.h"
#include "Domain.h"
#include "KernelResources.h"

namespace ocls {

// Forward dec.
class Data;

/**
 *
 *
 */
class ProgramBuilder{
public:
    /**
     *
     */
    ProgramBuilder(ComputeContext* compute_context);
    /**
     *
     */
    ~ProgramBuilder();
    
    /**
     *
     */
    Program* createInitialCondProgram(const Domain* domain, const CLSSource::Function* function_);

    /**
    * Create memcpy from buffer to texture program
    */
    Program* createMemcpyBufferToTextureProgram(size_t texture_dim);

    /**
    *
    */
    Program* createFindMaximumProgram(DeviceType type);

    /**
    *
    */
    Program* createFindMinimumProgram(DeviceType type);

    /**
    *
    */
    Program* createUnpaddedToPaddedProgram(int dim);

    /**
    *
    */
    Program* createPaddedToUnpaddedProgram(int dim);

    /**
    *
    */
    Program* createBoundaryProgram(const Domain* domain, const CLSSource::Function* function, OnBoundary boundary);

    /**
    * Create flux evaluator program
    */
    Program* createFluxProgram(const Domain* domain, const CLSSource::Function* function, size_t* return_values);

    /**
    * Create time integrator program
    */
    Program* createTimeIntegratorProgram(const Domain* domain, const CLSSource::Function* function, size_t* return_values);
    
private:

    Program* createBoundaryProgram1D(const CLSSource::Function* function, const Domain* domain, OnBoundary boundary);
    Program* createBoundaryProgram2D(const CLSSource::Function* function, const Domain* domain, OnBoundary boundary);
    Program* createBoundaryProgram3D(const CLSSource::Function* function, const Domain* domain, OnBoundary boundary);
    std::string createKernel(const CLSSource::Function *function, size_t *return_count = nullptr, bool early_exit = false);
    char toHex(int i);

    std::string appendDependencies(const Domain* domain, const CLSSource::Function* function);
    std::string getBoundaryConstantsDefinitions(OnBoundary boundary);
    std::string getDomainDefinitions(const Domain* domain);
    std::string appendGlobalsToBlock(const Domain* domain, std::string block);
    std::string buildSignature(const CLSSource::Function* function);

    bool isPointerType(CLSSource::Variable* variable);
private:
    ComputeContext* m_compute_context;
};

} //cls

#endif //PROGRAM_BUILDER_H