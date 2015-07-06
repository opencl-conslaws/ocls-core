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