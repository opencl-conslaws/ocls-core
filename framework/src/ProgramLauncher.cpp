#include <dirent.h>
#include <fcntl.h>
#include "ProgramLauncher.h"
#include "PerformanceAnalyzer.h"
#include "Utility.h"
#include "ProgramBuilder.h"

namespace ocls {

// Static decleration
Program* ProgramLauncher::m_memcpy_buf_to_tex_program[3];
Program* ProgramLauncher::m_find_maximum_value_program[2];
Program* ProgramLauncher::m_find_minimum_value_program[2];

Program* ProgramLauncher::m_padded_to_unpadded_program[3];
Program* ProgramLauncher::m_unpadded_to_padded_program[3];

// private members
bool ProgramLauncher::m_is_initialized;
ComputeContext* ProgramLauncher::m_compute_context;

/*
ProgramLauncher &ProgramLauncher::instance() {
    static ProgramLauncher instance;
    return instance;
}
*/

void ProgramLauncher::init(ComputeContext *compute_context) {
    m_compute_context = compute_context;
    m_is_initialized = true;

    ProgramBuilder builder(compute_context);


    for (int i = 0; i < 3; ++i) {
        m_unpadded_to_padded_program[i] = builder.createUnpaddedToPaddedProgram(i+1);
        m_padded_to_unpadded_program[i] = builder.createPaddedToUnpaddedProgram(i+1);
        m_memcpy_buf_to_tex_program[i] = builder.createMemcpyBufferToTextureProgram((size_t)(i+1));
    }

    // Find minimum programs
    m_find_maximum_value_program[ocls::DeviceType::__GPU__] = builder.createFindMaximumProgram(ocls::DeviceType::__GPU__);
    m_find_maximum_value_program[ocls::DeviceType::__CPU__] = builder.createFindMaximumProgram(ocls::DeviceType::__CPU__);

    // Find minimum programs
    m_find_minimum_value_program[ocls::DeviceType::__GPU__] = builder.createFindMinimumProgram(ocls::DeviceType::__GPU__);
    m_find_minimum_value_program[ocls::DeviceType::__CPU__] = builder.createFindMinimumProgram(ocls::DeviceType::__CPU__);
}

void ProgramLauncher::destory() {
    m_is_initialized = false;

    for (int i = 0; i < 3; ++i) {
        delete m_unpadded_to_padded_program[i];
        delete m_padded_to_unpadded_program[i];
        delete m_memcpy_buf_to_tex_program[i];
    }

    // Find minimum programs
    delete m_find_maximum_value_program[ocls::DeviceType::__GPU__];
    delete m_find_maximum_value_program[ocls::DeviceType::__CPU__];

    // Find minimum programs
    delete m_find_minimum_value_program[ocls::DeviceType::__GPU__];
    delete m_find_minimum_value_program[ocls::DeviceType::__CPU__];
}

void ProgramLauncher::launch(const Program *program, std::vector<Program::Parameter>& parameters, bool blocking) {
    Domain domain = program->getExecutionDomain();
    launch(program, parameters, domain, blocking);
}

void ProgramLauncher::launch(const Program *program, std::vector<Program::Parameter> &parameters, const Domain &domain, bool blocking) {
	//std::cout << "The program " << program->getName() << std::endl; 

	size_t s[3];
    for (size_t i = 1; i <= 3; i++) {
		//std::cout << "\tis executing on domain size "<< domain.getMeshWithGC((Domain::Dim)i) << std::endl;
        if(i <= (size_t)domain.getDimensions())
			s[i-1] = roundUp(domain.getMeshWithGC((Domain::Dim)i),32);
		else
			s[i-1] = 1;
		//std::cout << "\twhich is padded to " << s[i-1] << std::endl;
    }
    launch(program, parameters, domain.getDimensions(), &s[0], blocking);
}

void ProgramLauncher::launch(const Program *program, std::vector<Program::Parameter> &parameters, size_t dim, size_t *global_size, bool blocking) {
	size_t local[3];
	/*for (size_t i = 0; i < 3; i++) {
		if (global_size[i] % 4 == 0){
			//std::cout << "\tand with block size 4" << std::endl;
			local[i] = 4;
		}
		else{
			//std::cout << "\tand with block size 1" << std::endl;
			local[i] = 1;
		}
    }*/
	launch(program, parameters, dim, global_size, NULL, blocking);
    //launch(program, parameters, dim, global_size, local, blocking);
}

void ProgramLauncher::launch(const Program *program, std::vector<Program::Parameter> &parameters, size_t dim, size_t *global_size, size_t *local_size, bool blocking) {
    if(!m_is_initialized) return;

    cl_kernel kernel    = program->getEntryPoint();

    for (size_t i = 0; i < parameters.size(); i++) {
        CHECK_CL_ERROR(clSetKernelArg (kernel, (cl_uint)i, parameters[i].size, parameters[i].value));
    }

    cl_event event;
    CHECK_CL_ERROR(clEnqueueNDRangeKernel (m_compute_context->queue(), kernel, (cl_uint)dim, NULL, global_size, local_size, 0, NULL, &event));
    //logger->log(Logger::DEBUG, "Running kernel %s with dimensions %d @ [%d,%d,%d]",
    //        program->getName().c_str(), dim, global_size[0],global_size[1],global_size[2]);

    if(blocking){
        CHECK_CL_ERROR(clFinish(m_compute_context->queue()));
    }

    PerformanceAnalyser::analyzeProgramEvent(&m_compute_context->queue(), event, program->getName());
    clReleaseEvent(event);
}

Program *ProgramLauncher::getMemcpyProgram(size_t texture_dim) {
    return m_memcpy_buf_to_tex_program[texture_dim-1];
}

Program *ProgramLauncher::getFindMaximumProgram(DeviceType type) {
    return m_find_maximum_value_program[(int)type];
}

Program *ProgramLauncher::getFindMinimumProgram(DeviceType type) {
    return m_find_minimum_value_program[(int)type];
}

Program *ProgramLauncher::getUnpaddedToPaddedProgram(size_t dim) {
    return m_unpadded_to_padded_program[dim-1];;
}

Program *ProgramLauncher::getPaddedToUnpaddedProgram(size_t dim) {
    return m_padded_to_unpadded_program[dim-1];;
}

};
