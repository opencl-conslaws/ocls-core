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


#ifndef PERFORMANCE_ANALYSER_H
#define PERFORMANCE_ANALYSER_H

#include <iostream>
#include "ComputeContext.h"
#include <list>

namespace ocls {

class PerformanceAnalyser {
public:
    enum MemoryEvent {
        HOST_TO_DEVICE, DEVICE_TO_HOST, DEVICE_TO_DEVICE
    };
    enum TimelineEventType {
        PROGRAM, MEMORY
    };

    struct TimelineEntry {
        double cpu_time;
        double start_time;
        double end_time;

        double api_overhead;
        double execution_time;
        double total_time;

        TimelineEventType type;
        size_t index;
    };

    struct ProgramTimelineEntry {
        std::string program_name;

    };

    struct MemoryTimelineEntry {
        MemoryEvent type;
        size_t bytes;
        double transfere_rate;
    };

    struct KernelAnalysis{
        KernelAnalysis(){
            count = 0;

            total_time = 0.0f;
            total_api_overhead = 0.0f;
            total_execution_time = 0.0f;

            average_time = 0.0f;
            average_api_overhead = 0.0f;
            average_execution_time = 0.0f;
        }

        int count;

        float total_time;
        float total_api_overhead;
        float total_execution_time;

        float average_time;
        float average_api_overhead;
        float average_execution_time;
    };

    struct MemoryAnalysis{
        struct Entry {
            Entry(){
                count = 0;
                total_rate = 0.0f;
                total_bytes = 0;
                total_rate = 0.0f;

                average_rate = 0.0f;
                average_bytes = 0;
                average_time = 0.0f;
            }

            int count;

            double total_time ;
            long long total_bytes;
            double total_rate;

            double average_time;
            long long average_bytes;
            double average_rate;
        };
        Entry device_device;
        Entry device_host;
        Entry host_device;
    };

    static void start();

    static void end();

    static void writeAnalyticsTimeline(std::ostream &out);

    static void writeKernelAnalyse(std::string kernel_name, std::ostream &out);
    static KernelAnalysis analyseKernel(std::string kernel_name);

    static void writeMemoryAnalyse(std::ostream &out);
    static MemoryAnalysis analyseMemory();

    static void analyzeProgramEvent(cl_command_queue* queue, cl_event &event, const std::string &program_name);

    static void analyzeMemoryEvent(cl_command_queue* queue, cl_event &event, const size_t &size, const MemoryEvent &mem_event);

private:
    static TimelineEntry analyzeEvent(cl_event &event);

    static double getTime();

private:
    static bool     m_gatherAnalytics;
    static double   m_time;
    static double   m_endtime;

    static std::vector<TimelineEntry> m_timeline;
    static std::vector<MemoryTimelineEntry> m_memoryEvents;
    static std::vector<ProgramTimelineEntry> m_programEvents;

private:
    PerformanceAnalyser() {}
    PerformanceAnalyser(PerformanceAnalyser const &);
    ~PerformanceAnalyser() {}
    void operator=(PerformanceAnalyser const &);
};
}

#endif //PERFORMANCE_ANALYSER
