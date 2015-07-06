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





#ifndef C_DOMAIN_H
#define C_DOMAIN_H

#ifdef __cplusplus
extern "C" {
#endif
/**
*  \defgroup c_wrap_analyser Analyser
*  \ingroup c_wrap
*  @{
*/

/**
* Start analying
*/
int oclsAnalyserStart(void);

/**
 * Start analying
 */
int oclsAnalyserEnd(void);

/**
 * print output
 */
int oclsAnalyserPrint(void);

/**
 * write output
 */
int oclsAnalyserRead(char** output);

/**
 * write output
 */
int oclsAnalyserKernelRead(const char* kernel_name, char** output);

/**
 * write output
 */
int oclsAnalyserMemoryRead(char** output);

/**
 * write output
 */
int oclsAnalyserKernel(const char* kernel_name, KernelAnalysis** kernelAnalysis);

/**
 * write output
 */
int oclsAnalyserMemory(MemoryAnalysis** memoryAnalysis);

/** @}*/

#ifdef __cplusplus
}
#endif

#endif
