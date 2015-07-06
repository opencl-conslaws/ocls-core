#ifndef PROGRAM_H
#define PROGRAM_H

#include "ComputeContext.h"
#include "Domain.h"

#include <string>
#include <vector>
#include <typeinfo>

namespace ocls {

/**
 *
 *
 */
class Program{
public:
    struct Parameter{
        Parameter()
            : value(nullptr),
              size(0),
              allocation(0){};
        Parameter(void* value, size_t size)
            : value(value),
              size(size),
              allocation(0){};
        Parameter(void* value, size_t size, std::string type_name)
                : value(value),
                  size(size),
                  allocation(0){};

        ~Parameter(){}

        Parameter ( const Parameter & other)
                : value(other.value),
                  size(other.size),
                  allocation(0){};

        void* value;
        size_t size;
        int allocation;
    };
    /**
     *
     */
    Program(ComputeContext* compute_context, const Domain& domain, const std::string& name_, const std::string& source_,
            const std::string& compiler_arg_);
    /**
     *
     */
    ~Program();

    Domain getExecutionDomain()const;
    std::string getName()const ;
    cl_kernel getEntryPoint()const ;

private:
    void compile(cl_device_id device, const std::string& compiler_arg_);
    
private:
    ComputeContext* m_compute_context;
    Domain m_execution_domain;
    std::string m_name;
    cl_kernel m_entry_point;
    cl_program m_program;
};

} //cls

#endif //PROGRAM_H