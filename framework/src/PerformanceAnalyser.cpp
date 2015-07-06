#include "PerformanceAnalyzer.h"
#include <search.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <iomanip>

#ifdef  __MACH__
#include <mach/mach_time.h>
#include <mach/clock.h>
#include <mach/mach.h>
#endif

namespace ocls {

bool PerformanceAnalyser::m_gatherAnalytics = false;
double PerformanceAnalyser::m_time = 0.0;
double PerformanceAnalyser::m_endtime = 0.0;

std::vector<PerformanceAnalyser::TimelineEntry> PerformanceAnalyser::m_timeline;
std::vector<PerformanceAnalyser::MemoryTimelineEntry> PerformanceAnalyser::m_memoryEvents;
std::vector<PerformanceAnalyser::ProgramTimelineEntry> PerformanceAnalyser::m_programEvents;

void PerformanceAnalyser::start() {
    m_gatherAnalytics = true;
    m_timeline.clear();
    m_memoryEvents.clear();
    m_programEvents.clear();
    m_time = getTime();
}

void PerformanceAnalyser::end() {
    m_gatherAnalytics = false;
    m_endtime = getTime();
}

void PerformanceAnalyser::writeAnalyticsTimeline(std::ostream &out) {
    out << " == Profiling Timeline == " << std::endl << std::endl;
    double tmp = 0.0;
    out << std::fixed << std::setprecision(9);
    out << "Timestamp\t\t\t\tEvent\t\t\t\t\t\t\tDetails" << std::endl << std::endl;
    out << "Time: ["<< tmp <<"]\t\tProfiling started" << std::endl << std::endl;

    for (int i = 0; i < m_timeline.size(); ++i) {
        TimelineEntry entry = m_timeline[i];
        out << "Time: [" << entry.cpu_time << "]\t\t";

        if(entry.type == MEMORY){
            MemoryTimelineEntry mem_entry = m_memoryEvents[entry.index];
            switch (mem_entry.type){
                case HOST_TO_DEVICE:
                    out << "Host to Device Transfer";
                    break;
                case DEVICE_TO_HOST:
                    out << "Device to Host Transfer";
                    break;
                case DEVICE_TO_DEVICE:
                    out << "Device to Device Transfer";
                    break;
            }
            out << "\t\t\t";
            out << "Total time: " << entry.total_time << " s" << std::endl;
            out << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t";
            out << "API overhead: " << entry.api_overhead << " s" << std::endl;
            out << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t";
            out << "Execution time: " << entry.execution_time << " s" << std::endl;
            out << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t";
            out << "Transfer size: " << mem_entry.bytes << " bytes" << std::endl;
            out << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t";
            out << "Transfer rate: " << mem_entry.transfere_rate << " GB/s" << std::endl;
        }
        else if(entry.type == PROGRAM){
            ProgramTimelineEntry prog_entry = m_programEvents[entry.index];
            out << "Kernel Execution";
            out << "\t\t\t\t";
            out << "Kernel name: " << prog_entry.program_name << std::endl;
            out << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t";
            out << "Total time: " << entry.total_time << " s" << std::endl;
            out << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t";
            out << "API overhead: " << entry.api_overhead << " s" << std::endl;
            out << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t";
            out << "Execution time: " << entry.execution_time << " s" << std::endl;
        }
        out << std::endl;
    }
    out << "Time: [" << m_endtime - m_time << "]\t\tProfiling ended" << std::endl << std::endl;
    out << "Total time running: " <<  m_endtime - m_time << " s" << std::endl;
}

void PerformanceAnalyser::writeKernelAnalyse(std::string kernel_name, std::ostream &out) {
    int count = 0;
    float total_total_time = 0.0f;
    float total_api_overhead = 0.0f;
    float total_execution_time = 0.0f;

    for (int i = 0; i < m_timeline.size(); ++i) {
        TimelineEntry entry = m_timeline[i];

        if(entry.type == PROGRAM){
            ProgramTimelineEntry prog_entry = m_programEvents[entry.index];
            if (prog_entry.program_name == kernel_name){
                count++;

                total_total_time += entry.total_time;
                total_api_overhead += entry.api_overhead;
                total_execution_time += entry.execution_time;
            }
        }
    }

    out << " == Profiling Kernel " << kernel_name << " == " << std::endl << std::endl;
    out << std::fixed << std::setprecision(9);

    out << "Times invoked: \t\t" << count << std::endl;
    out << "Total total time: \t\t" << total_total_time << " s" << std::endl;
    out << "Total API overhead: \t\t" << total_api_overhead << " s" << std::endl;
    out << "Total Execution time: \t\t" << total_execution_time << " s" << std::endl;
    out << "Average total time: \t\t" << total_total_time/(float)count << " s" << std::endl;
    out << "Average API overhead: \t\t" << total_api_overhead/(float)count << " s" << std::endl;
    out << "Average Execution time: \t\t" << total_execution_time/(float)count << " s" << std::endl;
}

PerformanceAnalyser::KernelAnalysis PerformanceAnalyser::analyseKernel(std::string kernel_name) {
    PerformanceAnalyser::KernelAnalysis analysis;

    for (int i = 0; i < m_timeline.size(); ++i) {
        TimelineEntry entry = m_timeline[i];

        if(entry.type == PROGRAM){
            ProgramTimelineEntry prog_entry = m_programEvents[entry.index];
            if (prog_entry.program_name == kernel_name){
                analysis.count++;

                analysis.total_time += entry.total_time;
                analysis.total_api_overhead += entry.api_overhead;
                analysis.total_execution_time += entry.execution_time;
            }
        }
    }

    analysis.average_time = analysis.total_time/(float)analysis.count;
    analysis.average_api_overhead = analysis.total_api_overhead/(float)analysis.count;
    analysis.average_execution_time = analysis.total_execution_time/(float)analysis.count;

    return analysis;
}

void PerformanceAnalyser::writeMemoryAnalyse(std::ostream &out) {
    out << " == Profiling Memory Events == " << std::endl << std::endl;
    out << std::fixed << std::setprecision(9);

    int count_host_device = 0;
    double total_total_host_device = 0.0f;
    long long total_bytes_host_device = 0;
    double total_host_device = 0.0f;


    int count_device_host = 0;
    double total_total_device_host = 0.0f;
    long long total_bytes_device_host = 0;
    double total_device_host = 0.0f;

    int count_device_device = 0;
    double total_total_device_device = 0.0f;
    long long total_bytes_device_device = 0;
    double total_device_device = 0.0f;

    for (int i = 0; i < m_timeline.size(); ++i) {
        TimelineEntry entry = m_timeline[i];

        if(entry.type == MEMORY) {
            MemoryTimelineEntry mem_entry = m_memoryEvents[entry.index];
            switch (mem_entry.type) {
                case HOST_TO_DEVICE:
                    count_host_device++;
                    total_total_host_device += entry.total_time;
                    total_bytes_host_device += mem_entry.bytes;
                    total_host_device += mem_entry.transfere_rate;
                    break;
                case DEVICE_TO_HOST:
                    count_device_host++;
                    total_total_device_host += entry.total_time;
                    total_bytes_device_host += mem_entry.bytes;
                    total_device_host += mem_entry.transfere_rate;
                    break;
                case DEVICE_TO_DEVICE:
                    count_device_device++;
                    total_total_device_device += entry.total_time;
                    total_bytes_device_device += mem_entry.bytes;
                    total_device_device += mem_entry.transfere_rate;
                    break;
            }
        }
    }

    out << "HOST-TO-DEVICE" << std::endl;
    out << "Times invoked: \t\t" << count_host_device << std::endl;
    out << "Total Transfer time: \t\t" << total_total_host_device << " s" << std::endl;
    out << "Total Bytes: \t\t" << total_bytes_host_device << " bytes" << std::endl;
    out << "Average Transfer rate: \t\t" << total_host_device/(double)count_host_device << " GB/s" << std::endl;

    out << std::endl;
    out << "DEVICE-TO-HOST" << std::endl;
    out << "Times invoked: \t\t" << count_device_host << std::endl;
    out << "Total Transfer time: \t\t" << total_total_device_host << " s"  << std::endl;
    out << "Total Bytes: \t\t" << total_bytes_device_host << " bytes"  << std::endl;
    out << "Average Transfer rate: \t\t" << total_device_host/(double)count_device_host << " GB/s" << std::endl;

    out << std::endl;
    out << "DEVICE-TO-DEVICE" << std::endl;
    out << "Times invoked: \t\t" << count_device_device << std::endl;
    out << "Total Transfer time: \t\t" << total_total_device_device << " s" << std::endl;
    out << "Total Bytes: \t\t" << total_bytes_device_device << " bytes"  << std::endl;
    out << "Average Transfer rate: \t\t" << total_device_device/(double)count_device_device << " GB/s" << std::endl;
}

PerformanceAnalyser::MemoryAnalysis PerformanceAnalyser::analyseMemory() {
    PerformanceAnalyser::MemoryAnalysis analysis;

    for (int i = 0; i < m_timeline.size(); ++i) {
        TimelineEntry entry = m_timeline[i];

        if(entry.type == MEMORY) {
            MemoryTimelineEntry mem_entry = m_memoryEvents[entry.index];
            switch (mem_entry.type) {
                case HOST_TO_DEVICE:
                    analysis.host_device.count++;
                    analysis.host_device.total_time += entry.total_time;
                    analysis.host_device.total_bytes += mem_entry.bytes;
                    analysis.host_device.total_rate += mem_entry.transfere_rate;
                    break;
                case DEVICE_TO_HOST:
                    analysis.device_host.count++;
                    analysis.device_host.total_time += entry.total_time;
                    analysis.device_host.total_bytes += mem_entry.bytes;
                    analysis.device_host.total_rate += mem_entry.transfere_rate;
                    break;
                case DEVICE_TO_DEVICE:
                    analysis.device_device.count++;
                    analysis.device_device.total_time += entry.total_time;
                    analysis.device_device.total_bytes += mem_entry.bytes;
                    analysis.device_device.total_rate += mem_entry.transfere_rate;
                    break;
            }
        }
    }

    analysis.host_device.average_rate = analysis.host_device.total_rate/(double)analysis.host_device.count;
    analysis.host_device.average_bytes = analysis.host_device.total_bytes/analysis.host_device.count;
    analysis.host_device.average_time = analysis.host_device.total_time/(double)analysis.host_device.count;

    analysis.device_host.average_rate = analysis.device_host.total_rate/(double)analysis.device_host.count;
    analysis.device_host.average_bytes = analysis.device_host.total_bytes/analysis.device_host.count;
    analysis.device_host.average_time = analysis.device_host.total_time/(double)analysis.device_host.count;

    analysis.device_device.average_rate = analysis.device_device.total_rate/(double)analysis.device_device.count;
    analysis.device_device.average_bytes = analysis.device_device.total_bytes/analysis.device_device.count;
    analysis.device_device.average_time = analysis.device_device.total_time/(double)analysis.device_device.count;

    return analysis;
}

void PerformanceAnalyser::analyzeProgramEvent(cl_command_queue* queue, cl_event &event, const std::string &program_name) {
    if (!m_gatherAnalytics) {
        return;
    }

    clFinish(*queue);
    TimelineEntry time = analyzeEvent(event);
    ProgramTimelineEntry prog_entry;
    prog_entry.program_name = program_name;

    m_programEvents.push_back(prog_entry);
    time.type = PROGRAM;
    time.index = m_programEvents.size()-1;
    m_timeline.push_back(time);
}

void PerformanceAnalyser::analyzeMemoryEvent(cl_command_queue* queue, cl_event &event, const size_t &size, PerformanceAnalyser::MemoryEvent const &mem_event) {
    if (!m_gatherAnalytics) {
        return;
    }

    clFinish(*queue);
    TimelineEntry time = analyzeEvent(event);
    MemoryTimelineEntry mem_entry;
    mem_entry.type = mem_event;
    mem_entry.bytes = size;
    mem_entry.transfere_rate = ((double)mem_entry.bytes/1073741824.0)/time.execution_time;

    m_memoryEvents.push_back(mem_entry);
    time.type = MEMORY;
    time.index = m_memoryEvents.size()-1;
    m_timeline.push_back(time);
}

PerformanceAnalyser::TimelineEntry PerformanceAnalyser::analyzeEvent(cl_event &event) {
    // Wait for event information to be ready
    clWaitForEvents(1, &event);
    TimelineEntry entry;

    cl_ulong time;
    clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_QUEUED, sizeof(cl_ulong), &time, NULL);
    entry.start_time = (double) time / 1000000000.0;

    clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &time, NULL);
    entry.end_time = (double) time / 1000000000.0;

    clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &time, NULL);
    double exec_start = ((double) time / 1000000000.0);
    entry.execution_time = entry.end_time - exec_start;
    entry.api_overhead = exec_start - entry.start_time;
    entry.total_time = entry.end_time - entry.start_time;

    entry.cpu_time = (getTime()-m_time)-entry.total_time;

    return entry;
}

double PerformanceAnalyser::getTime() {
    struct timespec ts;
#ifdef __MACH__
    clock_serv_t cclock;
    mach_timespec_t mts;
    host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
    clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);

    ts.tv_sec = mts.tv_sec;
    ts.tv_nsec = mts.tv_nsec;
#elif defined (__linux__)
    clock_gettime(CLOCK_MONOTONIC, &ts);
#else
    LARGE_INTEGER current;
    QueryPerformanceCounter(&current);
    return (double)current.QuadPart * 1e9;
#endif
    return (double)ts.tv_sec + ts.tv_nsec * 1e-9;
}

}


