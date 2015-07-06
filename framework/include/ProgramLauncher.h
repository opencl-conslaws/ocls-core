#ifndef PROGRAM_LAUNCHER_H
#define PROGRAM_LAUNCHER_H

#include "Program.h"
#include "ComputeContext.h"

#include <map>

namespace ocls {

class ProgramLauncher {
public:
    //static ProgramLauncher& instance();
    static void init(ComputeContext* compute_context);
    static void destory();

    static void launch(const Program* program, std::vector<Program::Parameter>& parameters, bool blocking);
    static void launch(const Program* program, std::vector<Program::Parameter>& parameters, const Domain& domain, bool blocking);
    static void launch(const Program* program, std::vector<Program::Parameter>& parameters, size_t dim, size_t* global_size, bool blocking);
    static void launch(const Program* program, std::vector<Program::Parameter>& parameters, size_t dim, size_t* global_size, size_t* local_size, bool blocking);

    static Program* getMemcpyProgram(size_t texture_dim);
    static Program* getUnpaddedToPaddedProgram(size_t dim);
    static Program* getPaddedToUnpaddedProgram(size_t dim);

    static Program* getFindMaximumProgram(DeviceType type);
    static Program* getFindMinimumProgram(DeviceType type);

private:
    //ProgramLauncher() :m_is_initialized(false) {};
    //ProgramLauncher(ProgramLauncher const&);    // Don't Implement
    //void operator=(ProgramLauncher const&);     // Don't implement
private:
    // Utility programs
    static Program* m_memcpy_buf_to_tex_program[3];
    static Program* m_find_maximum_value_program[2];
    static Program* m_find_minimum_value_program[2];

    static Program* m_unpadded_to_padded_program[3];
    static Program* m_padded_to_unpadded_program[3];

    // private members
    static bool m_is_initialized;
    static ComputeContext* m_compute_context;
};


}

#endif